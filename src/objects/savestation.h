#pragma once
#include <tds/object.h>
#include <tds/clock.h>

#define HUNTER_SAVESTATION_INDEX_ID 0

struct tds_object_type obj_savestation_type;

void obj_savestation_init(struct tds_object* ptr);
void obj_savestation_destroy(struct tds_object* ptr);
void obj_savestation_update(struct tds_object* ptr);
void obj_savestation_draw(struct tds_object* ptr);
void obj_savestation_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_savestation_data {
	int unused;
	float dt_glow;
};
