#pragma once

#include <tds/object.h>
#include <tds/clock.h>

/*
 * env
 * s0: text to display on init
 */

#define HUNTER_ENV_INDEX_WNAME 0
#define HUNTER_ENV_INDEX_FONT  1

#define HUNTER_ENV_FONT_DEFAULT "debug"

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
	char* wname;
	int wname_pos;
	tds_clock_point wname_alpha_timer, wname_interval_timer, wname_wait_timer;
	int wname_enable;
	float wname_alpha;
	struct tds_font* render_font;
};
