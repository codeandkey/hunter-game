#pragma once
#include <tds/object.h>

#define TDS_OBJ_GHOST_AGGRO_DIST 10.0f
#define TDS_OBJ_GHOST_SPEED 0.01f
#define TDS_OBJ_GHOST_HIT 0.01f

struct tds_object_type obj_ghost_type;

void obj_ghost_init(struct tds_object* ptr);
void obj_ghost_destroy(struct tds_object* ptr);
void obj_ghost_update(struct tds_object* ptr);
void obj_ghost_draw(struct tds_object* ptr);
void obj_ghost_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_ghost_data {
	struct tds_object* track;
	int facing;
	float dt;
};
