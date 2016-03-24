#pragma once

#include <tds/object.h>
#include <tds/clock.h>

/* The slope is per-millisecond. */
#define HUNTER_FADET_SLOPE_DEFAULT 0.001f

#define HUNTER_FADET_INDEX_DEST  0
#define HUNTER_FADET_INDEX_SLOPE 1

struct tds_object_type obj_fade_transition_type;

void obj_fade_transition_init(struct tds_object* ptr);
void obj_fade_transition_destroy(struct tds_object* ptr);
void obj_fade_transition_update(struct tds_object* ptr);
void obj_fade_transition_draw(struct tds_object* ptr);
void obj_fade_transition_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_fade_transition_data {
	float slope, fade_factor;
	char* dest_world;
	tds_clock_point start_point;
	int activated;
	struct tds_object* player;
};
