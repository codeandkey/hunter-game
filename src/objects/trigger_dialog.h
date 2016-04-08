#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_DIALOG_INDEX_SEQUENCE 0 /* string:  the name of the dialog sequence to trigger on collision */
#define HUNTER_DIALOG_INDEX_USE      1 /* boolean: whether to make the player USE the trigger */
#define HUNTER_DIALOG_INDEX_ONESHOT  2 /* boolean: whether to only fire on the first collision, otherwise, will fire on every collision event (no consecutive events) */

struct tds_object_type obj_trigger_dialog_type;

void obj_trigger_dialog_init(struct tds_object* ptr);
void obj_trigger_dialog_destroy(struct tds_object* ptr);
void obj_trigger_dialog_update(struct tds_object* ptr);
void obj_trigger_dialog_draw(struct tds_object* ptr);
void obj_trigger_dialog_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_trigger_dialog_data {
	struct tds_object* player;
	char* name;
	int oneshot;
	int activated;
	int use;
};
