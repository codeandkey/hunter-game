#include "fade.h"

struct tds_module_template mod_fade_type = {
	.name = "fade",
	.data_size = sizeof(struct mod_fade),
	.init = mod_fade_init,
	.destroy = mod_fade_destroy,
	.update = mod_fade_update,
	.draw = mod_fade_draw,
	.msg = mod_fade_msg
};

void mod_fade_init(void* data) {
	struct mod_fade* ptr = (struct mod_fade*) data;

	ptr->fade = 0.0f;
	ptr->text_fade = 0.0f;
	ptr->state = MOD_FADE_STATE_BLACK;
	ptr->resp_object = NULL;

	ptr->display_font = tds_font_cache_get(tds_engine_global->fc_handle, MOD_FADE_FONT);
	ptr->display_text = NULL;
}

void mod_fade_destroy(void* data) {}

void mod_fade_update(void* data) {
	struct mod_fade* ptr = (struct mod_fade*) data;

	switch (ptr->state) {
	case MOD_FADE_STATE_BLACK:
		ptr->fade = 0.0f;

		if ((ptr->text_fade += MOD_FADE_TEXT_SLOPE) > 1.0f) {
			ptr->text_fade = 1.0f;
		}
		break;
	case MOD_FADE_STATE_NONE:
		ptr->fade = 1.0f;

		if ((ptr->text_fade -= MOD_FADE_TEXT_SLOPE) < 0.0f) {
			ptr->text_fade = 0.0f;
		}
		break;
	case MOD_FADE_STATE_TO_NONE:
		if (!ptr->display_text || tds_clock_get_ms(ptr->cp) >= MOD_FADE_TEXT_DELAY) {
			if ((ptr->fade += MOD_FADE_SLOPE) > 1.0f) {
				ptr->fade = 1.0f;
				ptr->state = MOD_FADE_STATE_NONE;
				ptr->display_text = NULL; /* In the future, this may be left to objects to do manually, but I can't think of any uses for it */
				tds_engine_broadcast(tds_engine_global, MSG_FADE_NONE, ptr->resp_object);
			}

			ptr->text_fade = 1.0f - ptr->fade;
		} else if (ptr->display_text) {
			if ((ptr->text_fade += MOD_FADE_TEXT_SLOPE) > 1.0f) {
				ptr->text_fade = 1.0f;
			}
		}
		break;
	case MOD_FADE_STATE_TO_BLACK:
		if ((ptr->fade -= MOD_FADE_SLOPE) < 0.0f) {
			ptr->fade = 0.0f;
			ptr->state = MOD_FADE_STATE_BLACK;
			tds_engine_broadcast(tds_engine_global, MSG_FADE_BLACK, ptr->resp_object);
		}

		ptr->text_fade = 0.0f;
		break;
	}
}

void mod_fade_draw(void* data) {
	struct mod_fade* ptr = (struct mod_fade*) data;

	tds_render_set_fade_factor(tds_engine_global->render_handle, ptr->fade);
	tds_render_flat_set_mode(tds_engine_global->render_flat_overlay_handle, TDS_RENDER_COORD_REL_SCREENSPACE);
	tds_render_flat_set_color(tds_engine_global->render_flat_overlay_handle, 1.0f, 1.0f, 1.0f, ptr->text_fade);

	if (ptr->display_text) {
		tds_render_flat_text(tds_engine_global->render_flat_overlay_handle, ptr->display_font, ptr->display_text->data, ptr->display_text->len, 0.0f, 0.0f, TDS_RENDER_CALIGN, ptr->display_text->formats);
	}
}

void mod_fade_msg(void* data, int msg, void* param) {
	struct mod_fade* ptr = (struct mod_fade*) data;

	switch (msg) {
	case MSG_FADE_REQ_BLACK:
		ptr->resp_object = (struct tds_object*) param;
		if (ptr->state == MOD_FADE_STATE_NONE) {
			ptr->resp_object = (struct tds_object*) param;
			ptr->state = MOD_FADE_STATE_TO_BLACK;
			tds_engine_broadcast(tds_engine_global, MSG_FADE_START_BLACK, param);
		}
		break;
	case MSG_FADE_REQ_NONE:
		if (ptr->state == MOD_FADE_STATE_BLACK) {
			ptr->resp_object = (struct tds_object*) param;
			ptr->state = MOD_FADE_STATE_TO_NONE;
			tds_engine_broadcast(tds_engine_global, MSG_FADE_START_NONE, param);

			if (ptr->display_text) {
				ptr->cp = tds_clock_get_point();
			}
		}
		break;
	case MSG_FADE_REQ_TEXT:
		ptr->display_text = (struct tds_string*) param;

		if (!param) {
			ptr->text_fade = 0.0f;
		}
		break;
	}
}
