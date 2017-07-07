#include "elevator_stop.h"
#include "camera.h"
#include "player.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

/* This is pretty much a complete copy of trigger_dialog, except with an index for a sprite. */

struct tds_object_type obj_elevator_stop_type = {
	.type_name = "obj_elevator_stop",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_elevator_stop_data),
	.func_init = obj_elevator_stop_init,
	.func_destroy = obj_elevator_stop_destroy,
	.func_update = obj_elevator_stop_update,
	.func_draw = obj_elevator_stop_draw,
	.func_msg = obj_elevator_stop_msg,
	.save = 1,
};

void obj_elevator_stop_init(struct tds_object* ptr) {
	struct obj_elevator_stop_data* data = (struct obj_elevator_stop_data*) ptr->object_data;

	data->index = *tds_object_get_ipart(ptr, HUNTER_ELEVATOR_STOP_INDEX_STOPINDEX);
	data->stop_broadcast = 0;

	tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_STOP_BROADCAST, ptr);
}

void obj_elevator_stop_destroy(struct tds_object* ptr) {
}

void obj_elevator_stop_update(struct tds_object* ptr) {
	struct obj_elevator_stop_data* data = (struct obj_elevator_stop_data*) ptr->object_data;

	if (!data->stop_broadcast) tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_STOP_BROADCAST, ptr);

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

		if (!data->player) return;
	}

	struct obj_player_data* pdata = (struct obj_player_data*) data->player->object_data;

	if (!data->enabled && tds_collision_get_overlap(data->player, ptr) && !pdata->in_elevator) {
		/* if the stop is disabled, we teleport the player out. the doors should lock by themselves */
		struct tds_engine_object_list exit_list = tds_engine_get_object_list_by_type(tds_engine_global, "obj_elevator_exit");
		struct tds_object* target_exit = NULL;
		float target_dist;

		for (int i = 0; i < exit_list.size; ++i) {
			if (!target_exit) {
				target_exit = exit_list.buffer[i];
				target_dist = sqrt(pow(ptr->x - target_exit->x, 2) + pow(ptr->y - target_exit->y, 2));
				continue;
			}

			float d = sqrt(pow(ptr->x - exit_list.buffer[i]->x, 2) + pow(ptr->y - exit_list.buffer[i]->y, 2));

			if (d < target_dist) {
				target_dist = d;
				target_exit = exit_list.buffer[i];
			}
		}

		if (!target_exit) {
			tds_logf(TDS_LOG_WARNING, "No viable exits to place player!\n");
			return;
		}
	} else if (tds_collision_get_overlap(data->player, ptr) && !pdata->in_elevator) {
		tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_START_SEQ, ptr);
	}
}

void obj_elevator_stop_draw(struct tds_object* ptr) {
}

void obj_elevator_stop_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_elevator_stop_data* data = (struct obj_elevator_stop_data*) ptr->object_data;
	int target_index;

	switch (msg) {
	case MSG_ELEVATOR_STOP_ACK:
		if (param == ptr) data->stop_broadcast = 1;
		break;
	case MSG_ELEVATOR_START_SEQ:
		if (param == ptr) data->enabled = 0;
	case MSG_ELEVATOR_STOP_SEQ:
		if (param == ptr) data->enabled = 1;
		break;
	}
}
