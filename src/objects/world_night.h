#pragma once

#include <tds/object.h>

struct tds_object_type obj_world_night_type;

void obj_world_night_init(struct tds_object* ptr);
void obj_world_night_destroy(struct tds_object* ptr);
void obj_world_night_update(struct tds_object* ptr);
void obj_world_night_draw(struct tds_object* ptr);
void obj_world_night_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_world_night_data {
	int unused;
};
