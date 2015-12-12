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
	.default_params = (struct tds_object_param*) 0,
	.default_params_size = 0,
	.data_size = sizeof (struct obj_system_data),
	.func_init = obj_system_init,
	.func_destroy = obj_system_destroy,
	.func_update = obj_system_update,
	.func_draw = obj_system_draw,
	.func_msg = (void*) 0,
	.func_import = obj_system_import,
	.func_export = obj_system_export,
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
	struct obj_system_data* data = (struct obj_system_data*) ptr->object_data;
	struct tds_text_batch tb = {0};

	tb.str = data->fps_str;
	tb.str_len = sizeof data->fps_str / sizeof *(data->fps_str);
	tb.font = tds_sprite_cache_get(ptr->smgr, "font_debug");

	tb.layer = 1000;
	tb.r = tb.g = tb.b = tb.a = 1.0f;
	tb.x = tds_engine_global->camera_handle->x - tds_engine_global->camera_handle->width / 2.0f + 1.0f;
	tb.y = tds_engine_global->camera_handle->y - tds_engine_global->camera_handle->height / 2.0f + 1.0f;

	snprintf(data->fps_str, sizeof data->fps_str / sizeof *(data->fps_str), "FPS %f", tds_engine_global->state.fps);

	tds_text_submit(tds_engine_global->text_handle, &tb);
}

void obj_system_import(struct tds_object* ptr, struct tds_object_param* param_list) {
	if (param_list) {
		while (param_list) {
			tds_console_print(tds_engine_global->console_handle, "loaded parameter ");
			tds_console_print(tds_engine_global->console_handle, param_list->key);
			tds_console_print(tds_engine_global->console_handle, " : ");
			tds_console_print(tds_engine_global->console_handle, param_list->value);
			tds_console_print(tds_engine_global->console_handle, "\n");

			param_list = param_list->next;
		}
	}
}

struct tds_object_param* obj_system_export(struct tds_object* ptr) {
	struct tds_object_param* first = tds_malloc(sizeof(struct tds_object_param));

	const char* key = "test_key";
	const char* value = "test_value";

	memcpy(first->key, key, strlen(key));
	memcpy(first->value, value, strlen(value));

	first->type = TDS_PARAM_STRING;
	first->next = NULL;

	return first;
}
