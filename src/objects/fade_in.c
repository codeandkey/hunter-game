#include "fade_in.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../tds_game/game_msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_fade_in_type = {
	.type_name = "obj_fade_in",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_fade_in_data),
	.func_init = obj_fade_in_init,
	.func_destroy = obj_fade_in_destroy,
	.func_update = obj_fade_in_update,
	.func_draw = obj_fade_in_draw,
	.func_msg = obj_fade_in_msg,
	.save = 1,
};

void obj_fade_in_init(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;

	data->fade_factor = data->text_alpha = 0.0f;
	data->fade_slope = HUNTER_FADEIN_FADE_SLOPE;
	data->text_slope = HUNTER_FADEIN_TEXT_SLOPE;
	data->start_point = tds_clock_get_point();
	data->fadeout_switch = 1;

	float* slope = tds_object_get_fpart(ptr, HUNTER_FADEIN_INDEX_FADE_SLOPE);

	if (slope) {
		data->fade_slope = *slope;
	}

	slope = tds_object_get_fpart(ptr, HUNTER_FADEIN_INDEX_TEXT_SLOPE);

	if (slope) {
		data->text_slope = *slope;
	}

	char* sid = tds_object_get_spart(ptr, HUNTER_FADEIN_INDEX_SID);
	int sid_len = strlen(sid);

	if (sid_len > TDS_PARAM_VALSIZE) {
		sid_len = TDS_PARAM_VALSIZE;
	}

	int* sid_index = tds_object_get_ipart(ptr, HUNTER_FADEIN_INDEX_SID_INDEX);

	if (sid) {
		data->str = tds_stringdb_get(tds_engine_global->stringdb_handle, sid, sid_len, sid_index ? *sid_index : 0);
		data->text_time = HUNTER_FADEIN_TEXT_BASE_TIME + HUNTER_FADEIN_TEXT_CHAR_TIME * data->str->len;
	} else {
		data->str = NULL;
		data->text_time = HUNTER_FADEIN_TEXT_BASE_TIME;
	}

	data->render_font = tds_font_cache_get(tds_engine_global->fc_handle, HUNTER_FADEIN_TEXT_FONT);

	tds_render_set_fade_factor(tds_engine_global->render_handle, data->fade_factor);
}

void obj_fade_in_destroy(struct tds_object* ptr) {
}

void obj_fade_in_update(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;

	if (data && tds_clock_get_ms(data->start_point) > data->text_time) {
		data->text_alpha = 1.0f - data->fade_factor;
		data->fade_factor = tds_clock_get_ms(data->fadeout_start_point) * data->fade_slope;

		if (data->fade_factor > 1.0f) {
			data->fade_factor = 1.0f;
		}

		if (data->fadeout_switch) {
			tds_engine_broadcast(tds_engine_global, TDS_GAME_MSG_WORLD_READY, NULL);
			data->fadeout_switch = 0;
		}
	} else {
		data->text_alpha = tds_clock_get_ms(data->start_point) * data->text_slope;

		if (data->text_alpha > 1.0f) {
			data->text_alpha = 1.0f;
		}

		data->fadeout_start_point = tds_clock_get_point();
	}
}

void obj_fade_in_draw(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;

	tds_render_set_fade_factor(tds_engine_global->render_handle, data->fade_factor);

	if (data->str) {
		tds_render_flat_set_mode(tds_engine_global->render_flat_overlay_handle, TDS_RENDER_COORD_REL_SCREENSPACE);
		tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 1.0f, 1.0f, 1.0f, data->text_alpha);
		tds_render_flat_text(tds_engine_global->render_flat_overlay_handle, data->render_font, data->str->data, data->str->len, 0.0f, 0.0f, TDS_RENDER_CALIGN, data->str->formats);
	}
}

void obj_fade_in_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
