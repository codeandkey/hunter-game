#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_LIGHT_POINT_INDEX_R 0
#define HUNTER_LIGHT_POINT_INDEX_G 1
#define HUNTER_LIGHT_POINT_INDEX_B 2
#define HUNTER_LIGHT_POINT_INDEX_DIST 3
#define HUNTER_LIGHT_POINT_INDEX_FLICKER 4

struct tds_object_type obj_light_point_type;

void obj_light_point_init(struct tds_object* ptr);
void obj_light_point_destroy(struct tds_object* ptr);
void obj_light_point_update(struct tds_object* ptr);
void obj_light_point_draw(struct tds_object* ptr);
void obj_light_point_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_light_point_data {
	float r, g, b;
	float dist;
	float flicker;
};
