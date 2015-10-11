#include "player.h"

struct tds_object_type obj_player_type = {
	.type_name = "obj_player",
	.default_sprite = "spr_player",
	.default_params = 0,
	.default_params_size = 0,
	.data_size = sizeof(struct obj_player_data),
	.func_init = obj_player_init,
	.func_destroy = obj_player_destroy,
	.func_update = obj_player_update,
	.func_draw = obj_player_draw,
	.func_msg = obj_player_msg,
	.func_import = obj_player_import,
	.func_export = obj_player_export,
	.save = 1
};

void obj_player_init(struct tds_object* ptr) {
}

void obj_player_destroy(struct tds_object* ptr) {
}

void obj_player_update(struct tds_object* ptr) {
}

void obj_player_draw(struct tds_object* ptr) {
}

void obj_player_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}

void obj_player_import(struct tds_object* ptr, struct tds_object_param* param) {
}

struct tds_object_param* obj_player_export(struct tds_object* ptr) {
	return 0;
}
