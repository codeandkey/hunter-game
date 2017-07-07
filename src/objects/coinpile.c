#include "coinpile.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_coinpile_type = {
	.type_name = "obj_coinpile",
	.default_sprite = "spr_coinpile",
	.data_size = sizeof(struct obj_coinpile_data),
	.func_init = obj_coinpile_init,
	.func_destroy = obj_coinpile_destroy,
	.func_update = obj_coinpile_update,
	.func_draw = obj_coinpile_draw,
	.func_msg = obj_coinpile_msg,
	.save = 1,
};

void obj_coinpile_init(struct tds_object* ptr) {
	struct obj_coinpile_data* data = (struct obj_coinpile_data*) ptr->object_data;

	int* amt = tds_object_get_ipart(ptr, HUNTER_COINPILE_INDEX_AMT);

	data->amt = HUNTER_COINPILE_DEFAULT_AMT;
	if (amt) data->amt = *amt;

	int* save = tds_object_get_ipart(ptr, HUNTER_COINPILE_INDEX_SAVE);

	data->save = -1;
	if (save) data->save = *save;

	data->picked = 0;
	ptr->layer = 10;
	data->text_a = 1.0f;

	if (data->save >= 0) {
		struct tds_savestate_entry esave = tds_savestate_get(tds_engine_global->savestate_handle, HUNTER_SAVE_COINPILE_OFFSET + data->save);

		if (esave.data) data->picked = *((int*) esave.data);
		if (data->picked) ptr->a = 0.0f;
	}

	data->src = tds_sound_source_create();
	tds_sound_source_set_pos(data->src, ptr->x, ptr->y);
	tds_sound_source_load_buffer(data->src, tds_sound_cache_get(tds_engine_global->sndc_handle, "fx_coin"));

	data->font = tds_font_cache_get(tds_engine_global->fc_handle, "font_tiny");
}

void obj_coinpile_destroy(struct tds_object* ptr) {
	struct obj_coinpile_data* data = (struct obj_coinpile_data*) ptr->object_data;

	tds_sound_source_free(data->src);
}

void obj_coinpile_update(struct tds_object* ptr) {
	struct obj_coinpile_data* data = (struct obj_coinpile_data*) ptr->object_data;

	if (data->picked) {
		ptr->yspeed = HUNTER_COINPILE_SPEED;
		data->text_y += HUNTER_COINPILE_TEXT_SPEED;
		ptr->a -= HUNTER_COINPILE_ALPHA_DECAY;
		data->text_a -= HUNTER_COINPILE_TEXT_ALPHA_DECAY;
	}

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

		if (!data->player) {
			tds_logf(TDS_LOG_WARNING, "Failed to locate player object. Will try again next frame.\n");
			return;
		}
	}
}

void obj_coinpile_draw(struct tds_object* ptr) {
	struct obj_coinpile_data* data = (struct obj_coinpile_data*) ptr->object_data;

	char buf[16] = {0};
	snprintf(buf, 16, "+ %d", data->amt);

	if (data->picked) {
		tds_render_flat_set_mode(tds_engine_global->render_flat_overlay_handle, TDS_RENDER_COORD_WORLDSPACE);
		tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 1.0f, 0.9f, 0.0f, data->text_a);
		tds_render_flat_text(tds_engine_global->render_flat_overlay_handle, data->font, buf, strlen(buf), ptr->x, data->text_y, TDS_RENDER_CALIGN, NULL);
	}
}

void obj_coinpile_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_coinpile_data* data = (struct obj_coinpile_data*) ptr->object_data;

	switch(msg) {
	case MSG_PLAYER_ACTION:
		if (data->player && !data->picked && tds_collision_get_overlap(ptr, data->player)) {
			data->picked = 1;
			data->text_y = ptr->y;
			tds_sound_source_play(data->src);
		}
		break;
	case MSG_COINPILE_SAVE_ALL:
		if (data->save >= 0) {
			tds_savestate_set(tds_engine_global->savestate_handle, HUNTER_SAVE_COINPILE_OFFSET + data->save, &data->picked, sizeof data->picked);
		}
		break;
	}
}
