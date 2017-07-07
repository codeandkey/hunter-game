#include "player.h"
#include "player_camera.h"
#include "camera.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../msg.h"

#include "../save.h"

#include <stdio.h>

struct tds_object_type obj_player_camera_type = {
	.type_name = "obj_player_camera",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_player_camera_data),
	.func_init = obj_player_camera_init,
	.func_destroy = obj_player_camera_destroy,
	.func_update = obj_player_camera_update,
	.func_draw = obj_player_camera_draw,
	.func_msg = obj_player_camera_msg,
	.save = 0
};

void obj_player_camera_init(struct tds_object* ptr) {
	struct obj_player_camera_data* data = (struct obj_player_camera_data*) ptr->object_data;

	data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

	if (!data->player) {
		tds_logf(TDS_LOG_WARNING, "obj_player_camera can't find a player object.\n");
	}

	struct tds_object* camera = tds_engine_get_object_by_type(tds_engine_global, "obj_camera");

	if (!camera) {
		camera = tds_object_create(&obj_camera_type, ptr->hmgr, ptr->smgr, ptr->x, ptr->y, 0.0f, NULL);
	}

	tds_object_msg(camera, ptr, MSG_CAMERA_TRACK, ptr);

	ptr->cbox_width = 1.0f;
	ptr->cbox_height = 1.0f;

	ptr->layer = 10;
}

void obj_player_camera_destroy(struct tds_object* ptr) {
}

void obj_player_camera_update(struct tds_object* ptr) {
	struct obj_player_camera_data* data = (struct obj_player_camera_data*) ptr->object_data;
	struct obj_player_data* pdata = (struct obj_player_data*) data->player->object_data;

	ptr->x = data->player->x;

	if (pdata->look_up) {
		ptr->y = ptr->y + (data->player->y + HUNTER_PLAYER_CAMERA_LOOKUP - ptr->y) / HUNTER_PLAYER_CAMERA_ANIM_FACTOR;
	} else {
		ptr->y = ptr->y + (data->player->y - ptr->y) / HUNTER_PLAYER_CAMERA_ANIM_RET_FACTOR;
	}
}

void obj_player_camera_draw(struct tds_object* ptr) {
}

void obj_player_camera_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
