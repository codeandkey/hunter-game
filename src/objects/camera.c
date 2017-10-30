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
	data->size = data->target_size = (tds_engine_global->display_handle->desc.height / 2);

	ptr->cbox.x = 1; /* one-pixel abstract entity */
	ptr->cbox.y = 1;

	tds_camera_set(tds_engine_global->camera_handle, ptr->pos, data->size);
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

	ptr->pos.x = ((float) data->track->pos.x + (float) data->track->cbox.x / 2.0f - (float) ptr->pos.x) / (float) TDS_OBJ_CAMERA_POS_FACTOR + (float) ptr->pos.x;
	ptr->pos.y = ((float) data->track->pos.y + (float) data->track->cbox.y / 2.0f - (float) ptr->pos.y) / (float) TDS_OBJ_CAMERA_POS_FACTOR + (float) ptr->pos.y;

	ptr->pos.x = data->track->pos.x;
	ptr->pos.y = data->track->pos.y;

	// data->size = (data->target_size - data->size) / TDS_OBJ_CAMERA_SIZE_FACTOR + data->size;

	tds_bcp final_cx = ptr->pos;
	struct tds_engine_object_list attractor_list = tds_engine_get_object_list_by_type(tds_engine_global, "obj_attractor");

	for (int i = 0; i < attractor_list.size; ++i) {
		float dist = sqrt(pow(ptr->pos.x - attractor_list.buffer[i]->pos.x, 2) + pow(ptr->pos.y - attractor_list.buffer[i]->pos.y, 2));
		struct obj_attractor_data* data = (struct obj_attractor_data*) attractor_list.buffer[i]->object_data;

		if (dist < data->dist) {
			float dist_factor = 1.0f - (dist / data->dist);
			final_cx.x += dist_factor * (attractor_list.buffer[i]->pos.x - ptr->pos.x) * data->factor;
			final_cx.y += dist_factor * (attractor_list.buffer[i]->pos.y - ptr->pos.y) * data->factor;
		}
	}

	tds_bcp camera_bl;
	camera_bl.x = ptr->pos.x - tds_engine_global->camera_handle->dim.x / 2;
	camera_bl.y = ptr->pos.y - tds_engine_global->camera_handle->dim.y / 2;

	/* prevent overflow cases */

	if (ptr->pos.x < tds_engine_global->camera_handle->dim.x / 2) {
		camera_bl.x = 0;
	}

	if (ptr->pos.y < tds_engine_global->camera_handle->dim.y / 2) {
		camera_bl.y = 0;
	}

	tds_camera_set(tds_engine_global->camera_handle, camera_bl, data->size);
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
