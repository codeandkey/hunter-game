#include "camera.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../msg.h"
#include "attractor.h"

struct tds_object_type obj_camera_type = {
	.type_name = "obj_camera",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_camera_data),
	.func_init = obj_camera_init,
	.func_destroy = obj_camera_destroy,
	.func_update = obj_camera_update,
	.func_draw = obj_camera_draw,
	.func_msg = obj_camera_msg,
	.save = 0
};

void obj_camera_init(struct tds_object* ptr) {
	struct obj_camera_data* data = (struct obj_camera_data*) ptr->object_data;

	data->track = NULL;
	data->shake = 0.0f;
	data->size = data->target_size = (tds_engine_global->display_handle->desc.height / 32.0f) / TDS_OBJ_CAMERA_SIZE_DIV;

	tds_camera_set(tds_engine_global->camera_handle, data->size, ptr->x, ptr->y);
}

void obj_camera_destroy(struct tds_object* ptr) {
}

void obj_camera_update(struct tds_object* ptr) {
	/* We will not allow slower movement than normal, we will use the axis and implement a basic deadzone */
	struct obj_camera_data* data = (struct obj_camera_data*) ptr->object_data;

	if (!data->track) {
		tds_logf(TDS_LOG_WARNING, "Camera not assigned to track any object!\n");
		return;
	}

	ptr->x = (data->track->x - ptr->x) / TDS_OBJ_CAMERA_POS_FACTOR + ptr->x;
	ptr->y = (data->track->y - ptr->y) / TDS_OBJ_CAMERA_POS_FACTOR + ptr->y;

	data->size = (data->target_size - data->size) / TDS_OBJ_CAMERA_SIZE_FACTOR + data->size;

	float final_cx = ptr->x, final_cy = ptr->y;
	struct tds_engine_object_list attractor_list = tds_engine_get_object_list_by_type(tds_engine_global, "obj_attractor");

	for (int i = 0; i < attractor_list.size; ++i) {
		float dist = sqrt(pow(ptr->x - attractor_list.buffer[i]->x, 2) + pow(ptr->y - attractor_list.buffer[i]->y, 2));
		struct obj_attractor_data* data = (struct obj_attractor_data*) attractor_list.buffer[i]->object_data;

		if (dist < data->dist) {
			float dist_factor = 1.0f - (dist / data->dist);
			final_cx += dist_factor * (attractor_list.buffer[i]->x - ptr->x) * data->factor;
			final_cy += dist_factor * (attractor_list.buffer[i]->y - ptr->y) * data->factor;
		}
	}

	tds_camera_set(tds_engine_global->camera_handle, data->size, final_cx, final_cy);
}

void obj_camera_draw(struct tds_object* ptr) {
}

void obj_camera_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_camera_data* data = (struct obj_camera_data*) ptr->object_data;

	switch(msg) {
	case MSG_CAMERA_TRACK:
		data->track = (struct tds_object*) param;
		break;
	case MSG_CAMERA_SHAKE:
		data->shake = *((float*) param);
		break;
	case MSG_CAMERA_SIZE:
		data->size = *((float*) param);
		break;
	}
}
