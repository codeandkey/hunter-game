#pragma once
#include <tds/object.h>
#include <tds/clock.h>

/*
 * obj_spawn
 * i0 : spawn save index
 * i1 : direction (1 : right, -1 : left)
 */

#define HUNTER_SPAWN_INDEX_ID 0
#define HUNTER_SPAWN_INDEX_DIR 1

struct tds_object_type obj_spawn_type;

void obj_spawn_init(struct tds_object* ptr);
void obj_spawn_destroy(struct tds_object* ptr);
void obj_spawn_update(struct tds_object* ptr);
void obj_spawn_draw(struct tds_object* ptr);
void obj_spawn_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_spawn_data {
	int dir;
};
