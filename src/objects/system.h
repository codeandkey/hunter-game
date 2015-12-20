#pragma once

#include <tds/object.h>

extern struct tds_object_type obj_system_type;

void obj_system_update(struct tds_object* ptr);
void obj_system_draw(struct tds_object* ptr);
void obj_system_init(struct tds_object* ptr);
void obj_system_destroy(struct tds_object* ptr);

struct obj_system_data {
	char fps_str[64];
};
