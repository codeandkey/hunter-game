#pragma once
#include <tds/object.h>

struct tds_object_type obj_player_type;

void obj_player_init(struct tds_object* ptr);
void obj_player_destroy(struct tds_object* ptr);
void obj_player_update(struct tds_object* ptr);
void obj_player_draw(struct tds_object* ptr);
void obj_player_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);
void obj_player_import(struct tds_object* ptr, struct tds_object_param* p);
struct tds_object_param* obj_player_export(struct tds_object* ptr);

struct obj_player_data {
	int unused;
};
