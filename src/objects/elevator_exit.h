#pragma once

#include <tds/object.h>
#include <tds/clock.h>

/* this object is pretty much just a marker */

#define HUNTER_ELEVATOR_EXIT_INDEX_STOPINDEX 0 /* integer: id for the stop location */

struct tds_object_type obj_elevator_exit_type;

void obj_elevator_exit_init(struct tds_object* ptr);
void obj_elevator_exit_destroy(struct tds_object* ptr);
void obj_elevator_exit_update(struct tds_object* ptr);
void obj_elevator_exit_draw(struct tds_object* ptr);
void obj_elevator_exit_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_elevator_exit_data {
	struct tds_object* player;
	int index, opened;
};
