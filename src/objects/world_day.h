#pragma once

#include <tds/object.h>

struct tds_object_type obj_world_day_type;

void obj_world_day_init(struct tds_object* ptr);
void obj_world_day_destroy(struct tds_object* ptr);
void obj_world_day_update(struct tds_object* ptr);
void obj_world_day_draw(struct tds_object* ptr);
void obj_world_day_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_world_day_data {
	int unused;
};
