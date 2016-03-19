#pragma once

#include <tds/object.h>

#define HUNTER_FADEIN_ANIM_FACTOR 128.0f
#define HUNTER_FADEIN_INDEX_ANIM_FACTOR 0

struct tds_object_type obj_fade_in_type;

void obj_fade_in_init(struct tds_object* ptr);
void obj_fade_in_destroy(struct tds_object* ptr);
void obj_fade_in_update(struct tds_object* ptr);
void obj_fade_in_draw(struct tds_object* ptr);
void obj_fade_in_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_fade_in_data {
	float factor, target_factor, anim_factor;
};
