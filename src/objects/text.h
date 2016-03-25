#pragma once

#include <tds/object.h>

/* obj_text
 * a static text object in the world -- good for labeling
 *
 * s0: string database group name
 * i1: string database group offset index
 * s2: font name
 * f3: fade distance
 * i4: boolean value (0 or 1) 0 -> display text in world space, is affected by lighting and fadein/fadeout, 1 -> display text in overlay space, not affected by lights or fades
 * f5-f8: floating-point values for RGBA text color, each component between 0.0f and 1.0f
 */

#define HUNTER_TEXT_INDEX_SID 0
#define HUNTER_TEXT_INDEX_SID_INDEX 1
#define HUNTER_TEXT_INDEX_FONT 2
#define HUNTER_TEXT_INDEX_FADE 3
#define HUNTER_TEXT_INDEX_OVERLAY 4
#define HUNTER_TEXT_INDEX_R 5
#define HUNTER_TEXT_INDEX_G 6
#define HUNTER_TEXT_INDEX_B 7
#define HUNTER_TEXT_INDEX_A 8

#define HUNTER_TEXT_FONT_DEFAULT "debug"

struct tds_object_type obj_text_type;

void obj_text_init(struct tds_object* ptr);
void obj_text_destroy(struct tds_object* ptr);
void obj_text_update(struct tds_object* ptr);
void obj_text_draw(struct tds_object* ptr);
void obj_text_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_text_data {
	int overlay;
	float r, g, b, a;
	float fade;
	struct tds_font* font;
	struct tds_object* player;
	struct tds_string* str;
};
