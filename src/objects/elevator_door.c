#include "elevator_door.h"
#include "camera.h"
#include "player.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

/* This is pretty much a complete copy of trigger_dialog, except with an index for a sprite. */

struct tds_object_type obj_elevator_door_type = {
	.type_name = "obj_elevator_door",
	.default_sprite = "spr_elevator_door_open",
	.data_size = sizeof(struct obj_elevator_door_data),
	.func_init = obj_elevator_door_init,
	.func_destroy = obj_elevator_door_destroy,
	.func_update = obj_elevator_door_update,
	.func_draw = obj_elevator_door_draw,
	.func_msg = obj_elevator_door_msg,
	.save = 1,
};

void obj_elevator_door_init(struct tds_object* ptr) {
	struct obj_elevator_door_data* data = (struct obj_elevator_door_data*) ptr->object_data;

	data->index = *tds_object_get_ipart(ptr, HUNTER_ELEVATOR_DOOR_INDEX_STOPINDEX);
	data->opened = 1;
	ptr->anim_oneshot = 1;
}

void obj_elevator_door_destroy(struct tds_object* ptr) {
}

void obj_elevator_door_update(struct tds_object* ptr) {
	struct obj_elevator_door_data* data = (struct obj_elevator_door_data*) ptr->object_data;

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

		if (!data->player) return;
	}

	struct obj_player_data* pdata = (struct obj_player_data*) data->player->object_data;
}

void obj_elevator_door_draw(struct tds_object* ptr) {
	struct obj_elevator_door_data* data = (struct obj_elevator_door_data*) ptr->object_data;
}

void obj_elevator_door_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_elevator_door_data* data = (struct obj_elevator_door_data*) ptr->object_data;
	int target_index;

	switch (msg) {
	case MSG_ELEVATOR_LOCK_STOP:
		target_index = *((int*) param);
		if (target_index == data->index) {
			data->opened = 0;
			tds_object_set_sprite(ptr, tds_sprite_cache_get(ptr->smgr, "spr_elevator_door_close"));
			tds_object_anim_start(ptr);
		}
		break;
	case MSG_ELEVATOR_UNLOCK_STOP:
		target_index = *((int*) param);
		if (target_index == data->index) {
			data->opened = 1;
			tds_object_set_sprite(ptr, tds_sprite_cache_get(ptr->smgr, "spr_elevator_door_open"));
			tds_object_anim_start(ptr);
		}
		break;
	}
}
