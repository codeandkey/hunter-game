#include "ladder.h"
#include "camera.h"
#include "player.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

/* This is pretty much a complete copy of trigger_dialog, except with an index for a sprite. */

struct tds_object_type obj_ladder_type = {
	.type_name = "obj_ladder",
	.default_sprite = "spr_ladder_wooden",
	.data_size = sizeof(struct obj_ladder_data),
	.func_init = obj_ladder_init,
	.func_destroy = obj_ladder_destroy,
	.func_update = obj_ladder_update,
	.func_draw = obj_ladder_draw,
	.func_msg = obj_ladder_msg,
	.save = 1,
};

void obj_ladder_init(struct tds_object* ptr) {
	struct obj_ladder_data* data = (struct obj_ladder_data*) ptr->object_data;

	ptr->cbox_width = 0.5f;
	ptr->cbox_height  = 10.0f;

	char* spr = tds_object_get_spart(ptr, HUNTER_LADDER_INDEX_SPR);

	if (spr) {
		tds_object_set_sprite(ptr, tds_sprite_cache_get(ptr->smgr, spr));
	}
}

void obj_ladder_destroy(struct tds_object* ptr) {
	struct obj_ladder_data* data = (struct obj_ladder_data*) ptr->object_data;
}

void obj_ladder_update(struct tds_object* ptr) {
	struct obj_ladder_data* data = (struct obj_ladder_data*) ptr->object_data;
}

void obj_ladder_draw(struct tds_object* ptr) {
	struct obj_ladder_data* data = (struct obj_ladder_data*) ptr->object_data;
}

void obj_ladder_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_ladder_data* data = (struct obj_ladder_data*) ptr->object_data;

	switch (msg) {
	case MSG_PLAYER_ACTION:
		if (data->player && tds_collision_get_overlap(ptr, data->player)) {
			tds_engine_broadcast(tds_engine_global, MSG_LADDER_ACK, ptr);
		}
		break;
	}
}
