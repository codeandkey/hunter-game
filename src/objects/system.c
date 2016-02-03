#include "system.h"

#include <tds/console.h>
#include <tds/engine.h>
#include <tds/memory.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct tds_object_type obj_system_type = {
	.type_name = "system",
	.default_sprite = (const char*) 0,
	.data_size = sizeof (struct obj_system_data),
	.func_init = obj_system_init,
	.func_destroy = obj_system_destroy,
	.func_update = obj_system_update,
	.func_draw = obj_system_draw,
	.func_msg = (void*) 0,
	.save = 0
};

void obj_system_init(struct tds_object* ptr) {
	tds_console_print(tds_engine_global->console_handle, "system is starting up..\n");
}

void obj_system_destroy(struct tds_object* ptr) {
}

void obj_system_update(struct tds_object* ptr) {
}

void obj_system_draw(struct tds_object* ptr) {
	struct tds_render_light lt = {TDS_RENDER_LIGHT_DIRECTIONAL, 1.0f, -1.0f, 0.14f, 0.14f, 0.14f, 0.0f, NULL};
	tds_render_submit_light(tds_engine_global->render_handle, lt);
}
