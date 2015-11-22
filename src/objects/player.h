#pragma once
#include <tds/object.h>

#define HUNTER_PLAYER_MOVE_DEADZONE 0.5f
#define HUNTER_PLAYER_MOVE_MAXSPEED 0.035f
#define HUNTER_PLAYER_MOVE_ACCEL 0.01f
#define HUNTER_PLAYER_MOVE_DECEL 1.1f
#define HUNTER_PLAYER_GRAVITY -0.002f
#define HUNTER_PLAYER_JUMP 0.08f

struct tds_object_type obj_player_type;

void obj_player_init(struct tds_object* ptr);
void obj_player_destroy(struct tds_object* ptr);
void obj_player_update(struct tds_object* ptr);
void obj_player_draw(struct tds_object* ptr);
void obj_player_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);
void obj_player_import(struct tds_object* ptr, struct tds_object_param* p);
struct tds_object_param* obj_player_export(struct tds_object* ptr);

struct obj_player_data {
	int can_jump, direction, movement_direction;
	int unused;
};
