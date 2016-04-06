#include "ghost.h"
#include "player.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../msg.h"

struct tds_object_type obj_ghost_type = {
	.type_name = "obj_ghost",
	.default_sprite = "spr_ghost_left",
	.data_size = sizeof(struct obj_ghost_data),
	.func_init = obj_ghost_init,
	.func_destroy = obj_ghost_destroy,
	.func_update = obj_ghost_update,
	.func_draw = obj_ghost_draw,
	.func_msg = obj_ghost_msg,
	.save = 1
};

void obj_ghost_init(struct tds_object* ptr) {
	struct obj_ghost_data* data = (struct obj_ghost_data*) ptr->object_data;

	data->track = NULL;
	data->dt = 0.0f;
}

void obj_ghost_destroy(struct tds_object* ptr) {
}

void obj_ghost_update(struct tds_object* ptr) {
	/* We will not allow slower movement than normal, we will use the axis and implement a basic deadzone */
	struct obj_ghost_data* data = (struct obj_ghost_data*) ptr->object_data;
	struct tds_object* player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

	if (player) {
		if (sqrt(pow(player->x - ptr->x, 2) + pow(player->y - ptr->y, 2)) <= TDS_OBJ_GHOST_AGGRO_DIST) {
			data->track = player;
		} else {
			data->track = NULL;
		}
	}

	if (!data->track) {
		return;
	}

	if (ptr->x < data->track->x) {
		ptr->x += TDS_OBJ_GHOST_SPEED;
		data->facing = 1;
	} else if (ptr->x > data->track->x) {
		ptr->x -= TDS_OBJ_GHOST_SPEED;
		data->facing = -1;
	}

	if (ptr->y < data->track->y) {
		ptr->y += TDS_OBJ_GHOST_SPEED;
	} else if (ptr->y > data->track->y) {
		ptr->y -= TDS_OBJ_GHOST_SPEED;
	}

	if (tds_collision_get_overlap(ptr, player)) {
		tds_object_msg(player, ptr, MSG_PLAYER_HIT, NULL);
	}
}

void obj_ghost_draw(struct tds_object* ptr) {
	struct obj_ghost_data* data = (struct obj_ghost_data*) ptr->object_data;

	if (data->facing > 0) {
		tds_object_set_sprite(ptr, tds_sprite_cache_get(ptr->smgr, "spr_ghost_right"));
	} else {
		tds_object_set_sprite(ptr, tds_sprite_cache_get(ptr->smgr, "spr_ghost_left"));
	}

	struct tds_render_light lt = {TDS_RENDER_LIGHT_POINT, ptr->x, ptr->y, 0.25f, 0.0f, 0.5f, 4.0f, NULL};
	tds_render_submit_light(tds_engine_global->render_handle, lt);
}

void obj_ghost_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
