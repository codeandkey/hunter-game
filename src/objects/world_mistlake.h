#pragma once

#include <tds/object.h>

struct tds_object_type obj_world_mistlake_type;

void obj_world_mistlake_init(struct tds_object* ptr);
void obj_world_mistlake_destroy(struct tds_object* ptr);
void obj_world_mistlake_update(struct tds_object* ptr);
void obj_world_mistlake_draw(struct tds_object* ptr);
void obj_world_mistlake_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_world_mistlake_data {
	int unused;
};
