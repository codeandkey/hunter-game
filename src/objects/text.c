#include "text.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render_flat.h>

#include <stdlib.h>

struct tds_object_type obj_text_type = {
	.type_name = "obj_text",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_text_data),
	.func_init = obj_text_init,
	.func_destroy = obj_text_destroy,
	.func_update = obj_text_update,
	.func_draw = obj_text_draw,
	.func_msg = obj_text_msg,
	.save = 1,
};

void obj_text_init(struct tds_object* ptr) {
	struct obj_text_data* data = (struct obj_text_data*) ptr->object_data;

	data->text = tds_object_get_spart(ptr, HUNTER_TEXT_INDEX_BUFFER);

	if (!data->text) {
		data->text = (char*) "unset";
	}

	data->text_len = strlen(data->text);

	if (data->text_len > TDS_PARAM_VALSIZE) {
		data->text_len = TDS_PARAM_VALSIZE;
	}

	data->r = data->g = data->b = data->a = 1.0f;

	float* r, *g, *b, *a;

	r = tds_object_get_fpart(ptr, HUNTER_TEXT_INDEX_R);
	g = tds_object_get_fpart(ptr, HUNTER_TEXT_INDEX_G);
	b = tds_object_get_fpart(ptr, HUNTER_TEXT_INDEX_B);
	a = tds_object_get_fpart(ptr, HUNTER_TEXT_INDEX_A);

	if (r) {
		data->r = *r;
	}

	if (g) {
		data->g = *g;
	}

	if (b) {
		data->b = *b;
	}

	if (a) {
		data->a = *a;
	}

	data->overlay = 0;

	int* overlay = tds_object_get_ipart(ptr, HUNTER_TEXT_INDEX_OVERLAY);

	if (overlay) {
		data->overlay = *overlay;
	}

	const char* font = tds_object_get_spart(ptr, HUNTER_TEXT_INDEX_FONT);

	if (!font) {
		font = HUNTER_TEXT_FONT_DEFAULT;
	}

	data->font = tds_font_cache_get(tds_engine_global->fc_handle, font);
	data->fade = 0;

	float* fade = tds_object_get_fpart(ptr, HUNTER_TEXT_INDEX_FADE);

	if (fade) {
		data->fade = *fade;
	}
}

void obj_text_destroy(struct tds_object* ptr) {
}

void obj_text_update(struct tds_object* ptr) {
}

void obj_text_draw(struct tds_object* ptr) {
	struct obj_text_data* data = (struct obj_text_data*) ptr->object_data;
	struct tds_render_flat* flat = tds_engine_global->render_flat_world_handle;

	float fade_factor = 1.0f;

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");
	}

	if (data->fade == 0.0f) {
		fade_factor = 1.0f;
	} else if (data->player) {
		float dist = sqrt(pow(data->player->x - ptr->x, 2) + pow(data->player->y - ptr->y, 2));
		fade_factor = 1.0f - (dist / data->fade);

		if (fade_factor < 0.0f) {
			fade_factor = 0.0f;
		}
	} else {
		fade_factor = 0.0f;
	}

	if (data->overlay) {
		flat = tds_engine_global->render_flat_overlay_handle;
	}

	tds_render_flat_set_mode(flat, TDS_RENDER_COORD_WORLDSPACE);
	tds_render_flat_set_color(flat, data->r, data->g, data->b, data->a * fade_factor);

	tds_render_flat_text(flat, data->font, data->text, data->text_len, ptr->x, ptr->y);
}

void obj_text_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
