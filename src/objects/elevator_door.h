#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_ELEVATOR_DOOR_INDEX_STOPINDEX 0 /* integer: id for the stop location */

struct tds_object_type obj_elevator_door_type;

void obj_elevator_door_init(struct tds_object* ptr);
void obj_elevator_door_destroy(struct tds_object* ptr);
void obj_elevator_door_update(struct tds_object* ptr);
void obj_elevator_door_draw(struct tds_object* ptr);
void obj_elevator_door_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_elevator_door_data {
	struct tds_object* player;
	int index, opened;
};
