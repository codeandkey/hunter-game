#include "elevator.h"
#include "elevator_stop.h"
#include "camera.h"
#include "player.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

/* This is pretty much a complete copy of trigger_dialog, except with an index for a sprite. */

struct tds_object_type obj_elevator_type = {
	.type_name = "obj_elevator",
	.default_sprite = "spr_elevator_idle",
	.data_size = sizeof(struct obj_elevator_data),
	.func_init = obj_elevator_init,
	.func_destroy = obj_elevator_destroy,
	.func_update = obj_elevator_update,
	.func_draw = obj_elevator_draw,
	.func_msg = obj_elevator_msg,
	.save = 1,
};

void obj_elevator_init(struct tds_object* ptr) {
	struct obj_elevator_data* data = (struct obj_elevator_data*) ptr->object_data;

	data->index_stop1 = *tds_object_get_ipart(ptr, HUNTER_ELEVATOR_INDEX_STOP1);
	data->index_stop2 = *tds_object_get_ipart(ptr, HUNTER_ELEVATOR_INDEX_STOP2);

	int* save = tds_object_get_ipart(ptr, HUNTER_ELEVATOR_INDEX_SAVE);

	data->save = -1;
	if (save) data->save = *save;

	data->stop1 = NULL; /* we have to wait for the stops to initialize before we can use anything */
	data->stop2 = NULL;

	data->state = HUNTER_ELEVATOR_STATE_IDLE;
}

void obj_elevator_destroy(struct tds_object* ptr) {
}

void obj_elevator_update(struct tds_object* ptr) {
	struct obj_elevator_data* data = (struct obj_elevator_data*) ptr->object_data;

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

		if (!data->player) return;
	}

	if (data->state != HUNTER_ELEVATOR_STATE_IDLE) {
		data->player->x = ptr->x;
		data->player->y = ptr->y + ptr->cbox_height / 2.0f + data->player->cbox_height / 2.0f;
	}

	switch (data->state) {
	case HUNTER_ELEVATOR_STATE_PREMOVE:
		if (tds_clock_get_ms(data->rest_timer) >= HUNTER_ELEVATOR_REST_TIME) data->state++;
		break;
	case HUNTER_ELEVATOR_STATE_MOVE:
		{
			struct tds_object* src = (data->at_stop1 ? data->stop1 : data->stop2), *dst = (data->at_stop1 ? data->stop2 : data->stop1);

			ptr->yspeed = HUNTER_ELEVATOR_SPEED;
			if (dst->y < src->y) ptr->yspeed = -ptr->yspeed;

			if ((ptr->yspeed < 0.0f && ptr->y + ptr->cbox_height / 2.0f <= dst->y - dst->cbox_height / 2.0f) || (ptr->yspeed > 0.0f && ptr->y + ptr->cbox_height / 2.0f >= dst->y - dst->cbox_height / 2.0f)) {
				data->state++;
				data->rest_timer = tds_clock_get_point();
				ptr->yspeed = 0.0f;

				/* lock the appropriate doors */

				tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_LOCK_STOP, data->at_stop1 ? &data->index_stop1 : &data->index_stop2);
				tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_UNLOCK_STOP, data->at_stop1 ? &data->index_stop2 : &data->index_stop1);
			}
		}
		break;
	case HUNTER_ELEVATOR_STATE_POSTMOVE:
		if (tds_clock_get_ms(data->rest_timer) >= HUNTER_ELEVATOR_REST_TIME) {
			/* done -- send out the seq finish and get the player out  */
			tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_STOP_SEQ, data->at_stop1 ? data->stop2 : data->stop1);

			data->at_stop1 = !data->at_stop1;
			data->state++;

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

			data->player->x = target_exit->x;
			data->player->y = target_exit->y;
		}
		break;
	case HUNTER_ELEVATOR_STATE_IDLE:
		break;
	}
}

void obj_elevator_draw(struct tds_object* ptr) {
}

void obj_elevator_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_elevator_data* data = (struct obj_elevator_data*) ptr->object_data;

	switch(msg) {
	case MSG_ELEVATOR_STOP_BROADCAST:
	{
		struct obj_elevator_stop_data* pdata = (struct obj_elevator_stop_data*) ((struct tds_object*) param)->object_data;
		int ind = pdata->index;
		if (ind == data->index_stop1) {
			if (data->stop1) {
				tds_logf(TDS_LOG_WARNING, "Elevator stop %d linked multiple times!\n", data->index_stop1);
			}
			data->stop1 = param;
			tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_STOP_ACK, param);
		}
		if (ind == data->index_stop2) {
			if (data->stop2) {
				tds_logf(TDS_LOG_WARNING, "Elevator stop %d linked multiple times!\n", data->index_stop2);
			}
			data->stop2 = param;
			tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_STOP_ACK, param);
		}
		if (data->stop1 && data->stop2) {
			tds_logf(TDS_LOG_MESSAGE, "Constructed elevator, linked to stops %d and %d\n", data->index_stop1, data->index_stop2);

			/* place the elevator at the nearest stop and prepare the state */

			float stop1_dist = sqrt(pow(ptr->x - data->stop1->x, 2) + pow(ptr->y - data->stop1->y, 2));
			float stop2_dist = sqrt(pow(ptr->x - data->stop2->x, 2) + pow(ptr->y - data->stop2->y, 2));

			data->at_stop1 = (stop1_dist < stop2_dist);

			/* quickly check if we have a savestate and load it up */
			if (data->save >= 0) {
				struct tds_savestate_entry e = tds_savestate_get(tds_engine_global->savestate_handle, HUNTER_SAVE_ELEV_OFFSET + data->save);

				if (e.data) {
					data->at_stop1 = *((int*) e.data);
				}
			}

			if (data->at_stop1) {
				ptr->x = data->stop1->x;
				ptr->y = data->stop1->y - data->stop1->cbox_height / 2.0f - ptr->cbox_height / 2.0f;
				tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_LOCK_STOP, &data->index_stop2);
				tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_UNLOCK_STOP, &data->index_stop1);
				tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_STOP_SEQ, data->stop1);
			} else {
				ptr->x = data->stop2->x;
				ptr->y = data->stop2->y - data->stop2->cbox_height / 2.0f - ptr->cbox_height / 2.0f;
				tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_LOCK_STOP, &data->index_stop1);
				tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_UNLOCK_STOP, &data->index_stop2);
				tds_engine_broadcast(tds_engine_global, MSG_ELEVATOR_STOP_SEQ, data->stop2);
			}
		}
	}
		break;
	case MSG_ELEVATOR_START_SEQ:
		/* player just touched a live stop. teleport the player to the elevator and start the move sequence. */
		if (param == data->stop1 || param == data->stop2) {
			data->rest_timer = tds_clock_get_point();
			data->state = HUNTER_ELEVATOR_STATE_PREMOVE;
		}
		break;
	case MSG_ELEVATOR_STOP_SEQ:
		break;
	case MSG_ELEVATOR_SAVE_ALL:
		if (data->save >= 0) {
			tds_savestate_set(tds_engine_global->savestate_handle, HUNTER_SAVE_ELEV_OFFSET + data->save, &data->at_stop1, sizeof data->at_stop1);
		}
		break;
	}
}
