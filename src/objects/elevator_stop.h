#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_ELEVATOR_STOP_INDEX_STOPINDEX 0 /* integer: id for the stop location */

struct tds_object_type obj_elevator_stop_type;

void obj_elevator_stop_init(struct tds_object* ptr);
void obj_elevator_stop_destroy(struct tds_object* ptr);
void obj_elevator_stop_update(struct tds_object* ptr);
void obj_elevator_stop_draw(struct tds_object* ptr);
void obj_elevator_stop_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_elevator_stop_data {
	struct tds_object* player;
	int index, enabled, stop_broadcast;
};
