#pragma once

#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_SCENERY_INDEX_SPRITE  0
#define HUNTER_SCENERY_INDEX_SEQUENCE 1 /* string:  the name of the scenery sequence to trigger on collision */
#define HUNTER_SCENERY_INDEX_USE      2 /* boolean: whether to make the player USE the trigger */
#define HUNTER_SCENERY_INDEX_ONESHOT  3 /* boolean: whether to only fire on the first collision, otherwise, will fire on every collision event (no consecutive events) */

struct tds_object_type obj_scenery_type;

void obj_scenery_init(struct tds_object* ptr);
void obj_scenery_destroy(struct tds_object* ptr);
void obj_scenery_update(struct tds_object* ptr);
void obj_scenery_draw(struct tds_object* ptr);
void obj_scenery_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_scenery_data {
	struct tds_object* player;
	char* name;
	int oneshot;
	int activated;
	int use;
};
