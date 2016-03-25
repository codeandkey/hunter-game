#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_FADEIN_FADE_SLOPE 0.0003f
#define HUNTER_FADEIN_TEXT_SLOPE 0.0003f
#define HUNTER_FADEIN_TEXT_FONT "game"

#define HUNTER_FADEIN_TEXT_BASE_TIME 3500.0f
#define HUNTER_FADEIN_TEXT_CHAR_TIME 10.0f

#define HUNTER_FADEIN_INDEX_SID 0
#define HUNTER_FADEIN_INDEX_SID_INDEX 1
#define HUNTER_FADEIN_INDEX_FADE_SLOPE 2
#define HUNTER_FADEIN_INDEX_TEXT_SLOPE 3

struct tds_object_type obj_fade_in_type;

void obj_fade_in_init(struct tds_object* ptr);
void obj_fade_in_destroy(struct tds_object* ptr);
void obj_fade_in_update(struct tds_object* ptr);
void obj_fade_in_draw(struct tds_object* ptr);
void obj_fade_in_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_fade_in_data {
	struct tds_string* str;
	float fade_factor, fade_slope;
	float text_alpha, text_time, text_slope;
	tds_clock_point start_point, fadeout_start_point;
	struct tds_font* render_font;
	int fadeout_switch;
};
