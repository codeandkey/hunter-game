#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_TRIGGERSON_INDEX_NAME 0

struct tds_object_type obj_trigger_soundon_type;

void obj_trigger_soundon_init(struct tds_object* ptr);
void obj_trigger_soundon_destroy(struct tds_object* ptr);
void obj_trigger_soundon_update(struct tds_object* ptr);
void obj_trigger_soundon_draw(struct tds_object* ptr);
void obj_trigger_soundon_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_trigger_soundon_data {
	struct tds_object* player;
	char* name;
	int activated;
};
