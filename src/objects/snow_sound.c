#include "snow_sound.h"

#include <tds/console.h>
#include <tds/engine.h>
#include <tds/memory.h>

#include <stdlib.h>
#include <string.h>

struct tds_object_type obj_snow_sound_type = {
	.type_name = "snow_sound",
	.default_sprite = (const char*) 0,
	.default_params = (struct tds_object_param*) 0,
	.default_params_size = 0,
	.data_size = 0,
	.func_init = obj_snow_sound_init,
	.func_destroy = obj_snow_sound_destroy,
	.func_update = obj_snow_sound_update,
	.func_draw = (void*) 0,
	.func_msg = (void*) 0,
	.func_import = (void*) 0,
	.func_export = (void*) 0,
	.save = 0
};

void obj_snow_sound_init(struct tds_object* ptr) {
	tds_console_print(tds_engine_global->console_handle, "snow_sound is starting up..\n");
}

void obj_snow_sound_destroy(struct tds_object* ptr) {
}

void obj_snow_sound_update(struct tds_object* ptr) {
}

void obj_snow_sound_import(struct tds_object* ptr, struct tds_object_param* param_list) {
}

struct tds_object_param* obj_snow_sound_export(struct tds_object* ptr) {
	return NULL;
}
