#pragma once
#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_PLAYER_CAMERA_LOOKUP 4.0f
#define HUNTER_PLAYER_CAMERA_ANIM_FACTOR 32.0f;
#define HUNTER_PLAYER_CAMERA_ANIM_RET_FACTOR 16.0f;

struct tds_object_type obj_player_camera_type;

void obj_player_camera_init(struct tds_object* ptr);
void obj_player_camera_destroy(struct tds_object* ptr);
void obj_player_camera_update(struct tds_object* ptr);
void obj_player_camera_draw(struct tds_object* ptr);
void obj_player_camera_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_player_camera_data {
	struct tds_object* player;
};
