#include "light_dir.h"
#include "camera.h"

#include <tds/tds.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_light_dir_type = {
	.type_name = "obj_light_dir",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_light_dir_data),
	.func_init = obj_light_dir_init,
	.func_destroy = obj_light_dir_destroy,
	.func_update = obj_light_dir_update,
	.func_draw = obj_light_dir_draw,
	.func_msg = obj_light_dir_msg,
	.save = 1,
};

void obj_light_dir_init(struct tds_object* ptr) {
	struct obj_light_dir_data* data = (struct obj_light_dir_data*) ptr->object_data;

	float* dx = tds_object_get_fpart(ptr, HUNTER_LIGHT_DIR_INDEX_DX);
	float* dy = tds_object_get_fpart(ptr, HUNTER_LIGHT_DIR_INDEX_DY);
	float* r = tds_object_get_fpart(ptr, HUNTER_LIGHT_DIR_INDEX_R);
	float* g = tds_object_get_fpart(ptr, HUNTER_LIGHT_DIR_INDEX_G);
	float* b = tds_object_get_fpart(ptr, HUNTER_LIGHT_DIR_INDEX_B);
	float* flicker = tds_object_get_fpart(ptr, HUNTER_LIGHT_DIR_INDEX_FLICKER);

	data->dir_x = dx ? *dx : 1.0f;
	data->dir_y = dy ? *dy : -1.0f;
	data->r = r ? *r : 0.02f;
	data->g = g ? *g : 0.02f;
	data->b = b ? *b : 0.02f;
	data->flicker = flicker ? *flicker : 0.0f;

	tds_logf(TDS_LOG_DEBUG, "Creating light!\n");
}

void obj_light_dir_destroy(struct tds_object* ptr) {
}

void obj_light_dir_update(struct tds_object* ptr) {
}

void obj_light_dir_draw(struct tds_object* ptr) {
	struct obj_light_dir_data* data = (struct obj_light_dir_data*) ptr->object_data;

	float flick = ((float) (rand() % 100) / 100.0f) * data->flicker;

	struct tds_render_light lt = {TDS_RENDER_LIGHT_DIRECTIONAL, data->dir_x, data->dir_y, data->r + flick, data->g + flick, data->b + flick, 0.0f, NULL};

	tds_render_submit_light(tds_engine_global->render_handle, lt);
}

void obj_light_dir_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
