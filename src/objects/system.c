#include "system.h"

#include <tds/console.h>
#include <tds/engine.h>
#include <tds/memory.h>

#include <stdlib.h>
#include <string.h>

struct tds_object_type obj_system_type = {
	.type_name = "system",
	.default_sprite = (const char*) 0,
	.default_params = (struct tds_object_param*) 0,
	.default_params_size = 0,
	.data_size = 0,
	.func_init = obj_system_init,
	.func_destroy = obj_system_destroy,
	.func_update = obj_system_update,
	.func_draw = (void*) 0,
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
