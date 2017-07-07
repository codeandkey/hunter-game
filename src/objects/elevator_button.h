#pragma once

#include <tds/object.h>
#include <tds/clock.h>
#include <tds/sound_source.h>

#define HUNTER_ELEVATOR_BUTTON_INDEX_STOPINDEX 0 /* integer: id for the stop location */

struct tds_object_type obj_elevator_button_type;

void obj_elevator_button_init(struct tds_object* ptr);
void obj_elevator_button_destroy(struct tds_object* ptr);
void obj_elevator_button_update(struct tds_object* ptr);
void obj_elevator_button_draw(struct tds_object* ptr);
void obj_elevator_button_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_elevator_button_data {
	struct tds_object* player;
	int index, active, used;
	struct tds_sound_source* src;
};
