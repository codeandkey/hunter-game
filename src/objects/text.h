#pragma once

#include <tds/object.h>

/* obj_text
 * a static text object in the world -- good for labeling
 *
 * s0: string up to TDS_PARAM_VALSIZE (usually 32) characters in length : the text to display (left-aligned)
 * i1: boolean value (0 or 1) 0 -> display text in world space, is affected by lighting and fadein/fadeout, 1 -> display text in overlay space, not affected by lights or fades
 * f2-f5: floating-point values for RGBA text color, each component between 0.0f and 1.0f
 */

#define HUNTER_TEXT_INDEX_BUFFER 0
#define HUNTER_TEXT_INDEX_OVERLAY 1
#define HUNTER_TEXT_INDEX_R 2
#define HUNTER_TEXT_INDEX_G 3
#define HUNTER_TEXT_INDEX_B 4
#define HUNTER_TEXT_INDEX_A 5

struct tds_object_type obj_text_type;

void obj_text_init(struct tds_object* ptr);
void obj_text_destroy(struct tds_object* ptr);
void obj_text_update(struct tds_object* ptr);
void obj_text_draw(struct tds_object* ptr);
void obj_text_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_text_data {
	char* text;
	int text_len;
	int overlay;
	float r, g, b, a;
};
