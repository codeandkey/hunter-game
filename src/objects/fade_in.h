#pragma once

#include <tds/object.h>
#include <tds/clock.h>

/*
 * The usage of obj_fade_in has changed a bit -- this object no longer does any of the actual work, but just relays fadein text info from the map to the fade module.
 */

#define HUNTER_FADEIN_INDEX_SID 0
#define HUNTER_FADEIN_INDEX_SID_INDEX 1

struct tds_object_type obj_fade_in_type;

void obj_fade_in_init(struct tds_object* ptr);
void obj_fade_in_destroy(struct tds_object* ptr);
void obj_fade_in_update(struct tds_object* ptr);
void obj_fade_in_draw(struct tds_object* ptr);
void obj_fade_in_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_fade_in_data {
	int unused;
};
