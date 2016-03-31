#pragma once
#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_PLAYER_MOVE_DEADZONE 0.5f
#define HUNTER_PLAYER_MOVE_MAXSPEED 0.035f
#define HUNTER_PLAYER_MOVE_ACCEL 0.01f
#define HUNTER_PLAYER_MOVE_DECEL 1.1f
#define HUNTER_PLAYER_GRAVITY -0.002f
#define HUNTER_PLAYER_JUMP 0.08f
#define HUNTER_PLAYER_HIT_RECOVERY 3000.0f
#define HUNTER_PLAYER_HIT_VEL 0.08f
#define HUNTER_PLAYER_HIT_FLASH 333.0f
#define HUNTER_PLAYER_SLOPE_PADDING 0.01f
#define HUNTER_PLAYER_SLOPE_CORRECT_OFFSET 0.0f 

#define HUNTER_PLAYER_MIN_JUMP_ANIM_YSPEED 0.05f
#define HUNTER_PLAYER_MAX_FALL_ANIM_YSPEED -0.05f

struct tds_object_type obj_player_type;

void obj_player_init(struct tds_object* ptr);
void obj_player_destroy(struct tds_object* ptr);
void obj_player_update(struct tds_object* ptr);
void obj_player_draw(struct tds_object* ptr);
void obj_player_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_player_data {
	int can_jump, direction, movement_direction, input_enabled;
	float spawn_x, spawn_y;
	int state_hit, state_hit_hurt;
	tds_clock_point timer_hit_recover, timer_hit_flash;
	int hit_flash, look_up;
	float csx, csy, csw, csh;
	float should_correct, collision_slope;
	int collision_x, collision_y, collision_xy;
};
