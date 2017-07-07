#include "world_mistlake.h"
#include "camera.h"

#include <tds/tds.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_world_mistlake_type = {
	.type_name = "obj_world_mistlake",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_world_mistlake_data),
	.func_init = obj_world_mistlake_init,
	.func_destroy = obj_world_mistlake_destroy,
	.func_update = obj_world_mistlake_update,
	.func_draw = obj_world_mistlake_draw,
	.func_msg = obj_world_mistlake_msg,
	.save = 1,
};

void obj_world_mistlake_init(struct tds_object* ptr) {
	struct obj_world_mistlake_data* data = (struct obj_world_mistlake_data*) ptr->object_data;

	tds_bg_push(tds_engine_global->bg_handle, 1, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_mistlake_640x640.png", 640, 640, 1, 1), 1, 0);
	tds_bg_push(tds_engine_global->bg_handle, 3, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_graytrees_640x640.png", 640, 640, 1, 1), 1, 0);
	tds_bg_push(tds_engine_global->bg_handle, 4, tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/bg_cloud_640x640.png", 640, 640, 1, 1), 1, 0);

	tds_render_set_ambient_brightness(tds_engine_global->render_handle, 0.6f);
}

void obj_world_mistlake_destroy(struct tds_object* ptr) {
}

void obj_world_mistlake_update(struct tds_object* ptr) {
}

void obj_world_mistlake_draw(struct tds_object* ptr) {
}

void obj_world_mistlake_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
