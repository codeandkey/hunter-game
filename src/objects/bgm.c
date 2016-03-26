#include "bgm.h"
#include "camera.h"

#include <tds/tds.h>

#include "../tds_game/game_msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_bgm_type = {
	.type_name = "obj_bgm",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_bgm_data),
	.func_init = obj_bgm_init,
	.func_destroy = obj_bgm_destroy,
	.func_update = obj_bgm_update,
	.func_draw = obj_bgm_draw,
	.func_msg = obj_bgm_msg,
	.save = 1,
};

void obj_bgm_init(struct tds_object* ptr) {
	struct obj_bgm_data* data = (struct obj_bgm_data*) ptr->object_data;
	char* sndname = tds_object_get_spart(ptr, HUNTER_BGM_INDEX_SOUND);
	int* dontstart = tds_object_get_ipart(ptr, HUNTER_BGM_INDEX_DONTSTART);
	int* control_id = tds_object_get_ipart(ptr, HUNTER_BGM_INDEX_CONTROL_INDEX);

	if (dontstart && *dontstart) {
		data->should_play = 0;
	} else {
		data->should_play = 1;
	}

	if (control_id) {
		data->control_id = *control_id;
	} else {
		data->control_id = 0;
	}

	if (sndname) {
		tds_sound_source_load_buffer(ptr->snd_src, tds_sound_cache_get(tds_engine_global->sndc_handle, sndname));
		ptr->snd_loop = 1;
		ptr->snd_volume = 0.0f;
		tds_sound_source_play(ptr->snd_src);
	} else {
		tds_logf(TDS_LOG_WARNING, "No sound parameter passed to BGM object. Won't play anything.\n");
	}
}

void obj_bgm_destroy(struct tds_object* ptr) {
}

void obj_bgm_update(struct tds_object* ptr) {
	struct obj_bgm_data* data = (struct obj_bgm_data*) ptr->object_data;

	if (data->should_play) {
		if (ptr->snd_volume < 1.0f) {
			ptr->snd_volume += HUNTER_BGM_VOLUME_SLOPE;
		} else {
			ptr->snd_volume = 1.0f;
		}
	} else {
		if (ptr->snd_volume > 0.0f) {
			ptr->snd_volume -= HUNTER_BGM_VOLUME_SLOPE;
		} else {
			ptr->snd_volume = 0.0f;
		}
	}
}

void obj_bgm_draw(struct tds_object* ptr) {
}

void obj_bgm_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_bgm_data* data = (struct obj_bgm_data*) ptr->object_data;

	unsigned int control = 0;

	if (param) {
		control = *((unsigned int*) param);
	}

	if (data->control_id && data->control_id != control) {
		return;
	}

	switch (msg) {
	case TDS_GAME_MSG_BGM_START:
		data->should_play = 1;
		break;
	case TDS_GAME_MSG_BGM_STOP:
		data->should_play = 0;
		break;
	}
}
