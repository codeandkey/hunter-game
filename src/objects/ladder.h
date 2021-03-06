#pragma once

#include <tds/object.h>
#include <tds/clock.h>

struct tds_object_type obj_ladder_type;

void obj_ladder_init(struct tds_object* ptr);
void obj_ladder_destroy(struct tds_object* ptr);
void obj_ladder_update(struct tds_object* ptr);
void obj_ladder_draw(struct tds_object* ptr);
void obj_ladder_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_ladder_data {
	struct tds_object* player;
};
