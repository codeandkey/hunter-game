#include "savestation.h"
#include "camera.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../tds_game/game_msg.h"

struct tds_object_type obj_savestation_type = {
	.type_name = "obj_savestation",
	.default_sprite = "spr_savestation",
	.default_params = 0,
	.default_params_size = 0,
	.data_size = sizeof(struct obj_savestation_data),
	.func_init = obj_savestation_init,
	.func_destroy = obj_savestation_destroy,
	.func_update = obj_savestation_update,
	.func_draw = obj_savestation_draw,
	.func_msg = obj_savestation_msg,
	.func_import = obj_savestation_import,
	.func_export = obj_savestation_export,
	.save = 1
};

void obj_savestation_init(struct tds_object* ptr) {
	struct obj_savestation_data* data = (struct obj_savestation_data*) ptr->object_data;

	ptr->cbox_width = 0.3f;
	ptr->cbox_height = 0.9f;
}

void obj_savestation_destroy(struct tds_object* ptr) {
}

void obj_savestation_update(struct tds_object* ptr) {
	struct obj_savestation_data* data = (struct obj_savestation_data*) ptr->object_data;
}

void obj_savestation_draw(struct tds_object* ptr) {
	struct obj_savestation_data* data = (struct obj_savestation_data*) ptr->object_data;
}

void obj_savestation_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_savestation_data* data = (struct obj_savestation_data*) ptr->object_data;
}

void obj_savestation_import(struct tds_object* ptr, struct tds_object_param* param) {
}

struct tds_object_param* obj_savestation_export(struct tds_object* ptr) {
	return 0;
}
