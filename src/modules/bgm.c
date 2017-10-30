#include "bgm.h"
#include "../msg.h"

struct tds_module_template mod_bgm_type = {
	.name = "bgm",
	.data_size = sizeof(struct mod_bgm),
	.init = mod_bgm_init,
	.destroy = mod_bgm_destroy,
	.update = mod_bgm_update,
	.draw = mod_bgm_draw,
	.msg = mod_bgm_msg
};

void mod_bgm_init(void* data) {
	struct mod_bgm* ptr = (struct mod_bgm*) data;

	ptr->entries = NULL;
}

void mod_bgm_destroy(void* data) {
	struct mod_bgm* ptr = (struct mod_bgm*) data;
	struct mod_bgm_entry* cur_entry = ptr->entries, *tmp = NULL;

	while (cur_entry) {
		tmp = cur_entry->next;
		tds_sound_source_free(cur_entry->snd_src);
		tds_free(cur_entry->name);
		tds_free(cur_entry);
		cur_entry = tmp;
	}
}

void mod_bgm_update(void* data) {
	struct mod_bgm* ptr = (struct mod_bgm*) data;
	struct tds_camera* cam_handle = tds_engine_global->camera_handle;

	struct mod_bgm_entry* cur_entry = ptr->entries;

	while (cur_entry) {
		switch(cur_entry->state) {
		case MOD_BGM_STATE_STOPPED:
			cur_entry->volume = 0.0f;
			break;
		case MOD_BGM_STATE_PLAYING:
			cur_entry->volume = 1.0f;
			break;
		case MOD_BGM_STATE_FADINGOUT:
			if ((cur_entry->volume -= MOD_BGM_VOLUME_SLOPE) < 0.0f) {
				cur_entry->volume = 0.0f;
				cur_entry->state = MOD_BGM_STATE_STOPPED;
				tds_sound_source_stop(cur_entry->snd_src);
			}
			break;
		case MOD_BGM_STATE_FADINGIN:
			if ((cur_entry->volume += MOD_BGM_VOLUME_SLOPE) > 1.0f) {
				cur_entry->volume = 1.0f;
				cur_entry->state = MOD_BGM_STATE_PLAYING;
			}
			break;
		}

		tds_sound_source_set_pos(cur_entry->snd_src, cam_handle->pos.x / 16.0f, cam_handle->pos.y / 16.0f);
		tds_sound_source_set_vel(cur_entry->snd_src, 0.0f, 0.0f);
		tds_sound_source_set_vol(cur_entry->snd_src, cur_entry->volume);
		tds_sound_source_set_loop(cur_entry->snd_src, cur_entry->loop);

		cur_entry = cur_entry->next;
	}
}

void mod_bgm_draw(void* data) {
}

struct mod_bgm_entry* mod_bgm_get_entry(struct mod_bgm* ptr, char* name) {
	struct mod_bgm_entry* cur = ptr->entries;

	while (cur) {
		if (!strcmp(cur->name, name)) {
			return cur;
		}

		cur = cur->next;
	}

	cur = tds_malloc(sizeof *cur);

	cur->name = tds_malloc(strlen(name) + 1);
	memcpy(cur->name, name, strlen(name));
	cur->name[strlen(name)] = 0;

	cur->state = MOD_BGM_STATE_STOPPED;
	cur->volume = 0.0f;
	cur->loop = 1;
	cur->snd_src = tds_sound_source_create();
	tds_sound_source_load_buffer(cur->snd_src, tds_sound_cache_get(tds_engine_global->sndc_handle, name));
	cur->next = ptr->entries;
	ptr->entries = cur;

	return cur;
}

void mod_bgm_msg(void* data, int msg, void* param) {
	struct mod_bgm* ptr = (struct mod_bgm*) data;
	struct mod_bgm_entry* target = NULL;

	switch (msg) {
	case MSG_BGM_REQ_START:
		target = mod_bgm_get_entry(ptr, (char*) param);
		if (target->state != MOD_BGM_STATE_PLAYING && target->state != MOD_BGM_STATE_FADINGIN) {
			target->state = MOD_BGM_STATE_FADINGIN;
			tds_sound_source_play(target->snd_src);
			tds_engine_broadcast(tds_engine_global, MSG_BGM_START, (char*) param);
		}
		break;
	case MSG_BGM_REQ_STOP:
		target = mod_bgm_get_entry(ptr, (char*) param);
		if (target->state != MOD_BGM_STATE_STOPPED && target->state != MOD_BGM_STATE_FADINGOUT) {
			target->state = MOD_BGM_STATE_FADINGOUT;
			tds_engine_broadcast(tds_engine_global, MSG_BGM_STOP, (char*) param);
		}
		break;
	}
}
