#pragma once
#include <tds/object.h>
#include <tds/clock.h>

struct tds_object_type obj_savestation_type;

void obj_savestation_init(struct tds_object* ptr);
void obj_savestation_destroy(struct tds_object* ptr);
void obj_savestation_update(struct tds_object* ptr);
void obj_savestation_draw(struct tds_object* ptr);
void obj_savestation_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);
void obj_savestation_import(struct tds_object* ptr, struct tds_object_param* p);
struct tds_object_param* obj_savestation_export(struct tds_object* ptr);

struct obj_savestation_data {
	int unused;
};
