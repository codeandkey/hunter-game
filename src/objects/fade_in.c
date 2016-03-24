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

	data->factor = 0.0f;
	data->target_factor = 1.0f;
	data->anim_factor = HUNTER_FADEIN_ANIM_FACTOR;
	data->start_point = tds_clock_get_point();
	data->text_alpha = 0.0f;
	data->fadeout_switch = 1;

	float* af = tds_object_get_fpart(ptr, HUNTER_FADEIN_INDEX_ANIM_FACTOR);

	if (af) {
		data->anim_factor = *af;
	}

	data->text = tds_object_get_spart(ptr, HUNTER_FADEIN_INDEX_TEXT);

	if (data->text) {
		data->text_len = strlen(data->text) > TDS_PARAM_VALSIZE ? TDS_PARAM_VALSIZE : strlen(data->text);
		data->text_time = HUNTER_FADEIN_TEXT_BASE_TIME + HUNTER_FADEIN_TEXT_CHAR_TIME * data->text_len;
	}

	data->render_font = tds_font_cache_get(tds_engine_global->fc_handle, HUNTER_FADEIN_TEXT_FONT);

	tds_render_set_fade_factor(tds_engine_global->render_handle, data->factor);
}

void obj_fade_in_destroy(struct tds_object* ptr) {
}

void obj_fade_in_update(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;

	if (data && tds_clock_get_ms(data->start_point) > data->text_time) {
		data->text_alpha = 1.0f - data->factor;
		data->factor = (data->target_factor - data->factor) / data->anim_factor + data->factor;

		if (data->fadeout_switch) {
			tds_engine_broadcast(tds_engine_global, TDS_GAME_MSG_WORLD_READY, NULL);
			data->fadeout_switch = 0;
		}
	} else {
		data->text_alpha = (1.0f - data->text_alpha) / HUNTER_FADEIN_TEXT_ANIM_FACTOR + data->text_alpha;
	}
}

void obj_fade_in_draw(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;

	tds_render_set_fade_factor(tds_engine_global->render_handle, data->factor);

	if (data->text) {
		tds_render_flat_set_mode(tds_engine_global->render_flat_overlay_handle, TDS_RENDER_COORD_REL_SCREENSPACE);
		tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 1.0f, 1.0f, 1.0f, data->text_alpha);
		tds_render_flat_text(tds_engine_global->render_flat_overlay_handle, data->render_font, data->text, data->text_len, 0.0f, 0.0f);
	}
}

void obj_fade_in_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
