#include "env.h"
#include "../tds_game/game_msg.h"

#include <tds/engine.h>
#include <tds/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct tds_object_type obj_env_type = {
	.type_name = "env",
	.default_sprite = (const char*) 0,
	.data_size = sizeof (struct obj_env_data),
	.func_init = obj_env_init,
	.func_destroy = obj_env_destroy,
	.func_update = obj_env_update,
	.func_draw = obj_env_draw,
	.func_msg = obj_env_msg,
	.save = 0
};

void obj_env_init(struct tds_object* ptr) {
	struct obj_env_data* data = (struct obj_env_data*) ptr->object_data;

	char* wname = tds_object_get_spart(ptr, HUNTER_ENV_INDEX_WNAME_SID);
	int* s_index = tds_object_get_ipart(ptr, HUNTER_ENV_INDEX_WNAME_SID_INDEX);
	int wname_len = strlen(wname);

	if (wname_len > TDS_PARAM_VALSIZE) {
		wname_len = TDS_PARAM_VALSIZE;
	}

	if (wname) {
		data->str = tds_stringdb_get(tds_engine_global->stringdb_handle, wname, wname_len, s_index ? *s_index : 0);
	} else {
		data->str = NULL;
	}

	data->wname_pos = 0;
	data->wname_alpha = 1.0f;
	data->wname_enable = 0;

	const char* font = tds_object_get_spart(ptr, HUNTER_ENV_INDEX_FONT);

	if (!font) {
		font = HUNTER_ENV_FONT_DEFAULT;
	} else {
		data->render_font = tds_font_cache_get(tds_engine_global->fc_handle, font);
	}
}

void obj_env_destroy(struct tds_object* ptr) {
}

void obj_env_update(struct tds_object* ptr) {
	struct obj_env_data* data = (struct obj_env_data*) ptr->object_data;

	if (data->str && data->wname_pos == data->str->len - 1 && tds_clock_get_ms(data->wname_alpha_timer) >= HUNTER_ENV_WNAME_ALPHA_TIME && data->wname_enable) {
		data->wname_alpha -= HUNTER_ENV_WNAME_ALPHA_DECAY;
	}
}

void obj_env_draw(struct tds_object* ptr) {
	struct obj_env_data* data = (struct obj_env_data*) ptr->object_data;

	if (!data->str) {
		return;
	}

	if (data->wname_enable && tds_clock_get_ms(data->wname_wait_timer) >= HUNTER_ENV_WNAME_WAIT) {
		int len = data->str->len;

		if (data->wname_pos < len - 1) {
			if (tds_clock_get_ms(data->wname_interval_timer) > HUNTER_ENV_WNAME_INTERVAL) {
				if (++data->wname_pos >= len - 1) {
					data->wname_alpha_timer = tds_clock_get_point();
				}

				data->wname_interval_timer = tds_clock_get_point();
			}
		}

		tds_render_flat_set_mode(tds_engine_global->render_flat_overlay_handle, TDS_RENDER_COORD_REL_SCREENSPACE);
		tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 1.0f, 1.0f, 1.0f, data->wname_alpha);
		tds_render_flat_text(tds_engine_global->render_flat_overlay_handle, data->render_font, data->str->data, data->wname_pos + 1, 0.0f, 0.9f, TDS_RENDER_CALIGN);
	}
}

void obj_env_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_env_data* data = (struct obj_env_data*) ptr->object_data;

	switch (msg) {
	case TDS_GAME_MSG_WORLD_READY:
		data->wname_enable = 1;
		data->wname_wait_timer = tds_clock_get_point();
		data->wname_interval_timer = tds_clock_get_point();
		tds_logf(TDS_LOG_WARNING, "recv world ready\n");
		break;
	}
}
