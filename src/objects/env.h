#pragma once

#include <tds/object.h>
#include <tds/clock.h>

/*
 * env
 * s0: string database groupname for text to display on world ready
 * i1: string database group offset
 * s2: font index
 */

#define HUNTER_ENV_INDEX_WNAME_SID 0
#define HUNTER_ENV_INDEX_WNAME_SID_INDEX 1
#define HUNTER_ENV_INDEX_FONT  2

#define HUNTER_ENV_FONT_DEFAULT "env"

#define HUNTER_ENV_WNAME_INTERVAL 100.0f
#define HUNTER_ENV_WNAME_WAIT 2000.0f
#define HUNTER_ENV_WNAME_ALPHA_TIME 3000.0f
#define HUNTER_ENV_WNAME_ALPHA_DECAY 0.02f

extern struct tds_object_type obj_env_type;

void obj_env_update(struct tds_object* ptr);
void obj_env_draw(struct tds_object* ptr);
void obj_env_init(struct tds_object* ptr);
void obj_env_destroy(struct tds_object* ptr);
void obj_env_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_env_data {
	int wname_pos;
	struct tds_string* str;
	tds_clock_point wname_alpha_timer, wname_interval_timer, wname_wait_timer;
	int wname_enable;
	float wname_alpha;
	struct tds_font* render_font;
};
