#include "light_point.h"
#include "camera.h"

#include <tds/tds.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_light_point_type = {
	.type_name = "obj_light_point",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_light_point_data),
	.func_init = obj_light_point_init,
	.func_destroy = obj_light_point_destroy,
	.func_update = obj_light_point_update,
	.func_draw = obj_light_point_draw,
	.func_msg = obj_light_point_msg,
	.save = 1,
};

void obj_light_point_init(struct tds_object* ptr) {
	struct obj_light_point_data* data = (struct obj_light_point_data*) ptr->object_data;

	float* r = tds_object_get_fpart(ptr, HUNTER_LIGHT_POINT_INDEX_R);
	float* g = tds_object_get_fpart(ptr, HUNTER_LIGHT_POINT_INDEX_G);
	float* b = tds_object_get_fpart(ptr, HUNTER_LIGHT_POINT_INDEX_B);
	float* dist = tds_object_get_fpart(ptr, HUNTER_LIGHT_POINT_INDEX_DIST);
	float* flicker = tds_object_get_fpart(ptr, HUNTER_LIGHT_POINT_INDEX_FLICKER);

	data->dist = dist ? *dist : 10.0f;
	data->r = r ? *r : 0.02f;
	data->g = g ? *g : 0.02f;
	data->b = b ? *b : 0.02f;
	data->flicker = flicker ? *flicker : 0.0f;

	tds_logf(TDS_LOG_DEBUG, "Creating light!\n");
}

void obj_light_point_destroy(struct tds_object* ptr) {
}

void obj_light_point_update(struct tds_object* ptr) {
}

void obj_light_point_draw(struct tds_object* ptr) {
	struct obj_light_point_data* data = (struct obj_light_point_data*) ptr->object_data;

	float flick = ((float) (rand() % 100) / 100.0f) * data->flicker;

	struct tds_render_light lt = {TDS_RENDER_LIGHT_POINT, ptr->x, ptr->y, data->r + flick, data->g + flick, data->b + flick, data->dist, NULL};

	tds_render_submit_light(tds_engine_global->render_handle, lt);
}

void obj_light_point_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
