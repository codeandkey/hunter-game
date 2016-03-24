#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_FADEIN_ANIM_FACTOR 128.0f
#define HUNTER_FADEIN_TEXT_ANIM_FACTOR 32.0f
#define HUNTER_FADEIN_TEXT_FONT "game"

#define HUNTER_FADEIN_TEXT_BASE_TIME 2500.0f
#define HUNTER_FADEIN_TEXT_CHAR_TIME 100.0f

#define HUNTER_FADEIN_INDEX_ANIM_FACTOR 0
#define HUNTER_FADEIN_INDEX_TEXT 1

struct tds_object_type obj_fade_in_type;

void obj_fade_in_init(struct tds_object* ptr);
void obj_fade_in_destroy(struct tds_object* ptr);
void obj_fade_in_update(struct tds_object* ptr);
void obj_fade_in_draw(struct tds_object* ptr);
void obj_fade_in_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_fade_in_data {
	char* text;
	int text_len;
	float factor, target_factor, anim_factor, text_time;
	float text_alpha;
	tds_clock_point start_point;
	struct tds_font* render_font;
	int fadeout_switch;
};
