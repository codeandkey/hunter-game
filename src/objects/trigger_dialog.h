#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_TRIGGERSOFF_INDEX_CONTROL 0

struct tds_object_type obj_trigger_dialog_type;

void obj_trigger_dialog_init(struct tds_object* ptr);
void obj_trigger_dialog_destroy(struct tds_object* ptr);
void obj_trigger_dialog_update(struct tds_object* ptr);
void obj_trigger_dialog_draw(struct tds_object* ptr);
void obj_trigger_dialog_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_trigger_dialog_data {
	struct tds_object* player;
	int activated;
	int control_id;
};
