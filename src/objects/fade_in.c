#include "fade_in.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../save.h"
#include "../msg.h"

#include <stdlib.h>

struct tds_object_type obj_fade_in_type = {
	.type_name = "obj_fade_in",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_fade_in_data),
	.func_init = obj_fade_in_init,
	.func_destroy = obj_fade_in_destroy,
	.func_update = obj_fade_in_update,
	.func_draw = obj_fade_in_draw,
	.func_msg = obj_fade_in_msg,
	.save = 1,
};

void obj_fade_in_init(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;

	char* sid = tds_object_get_spart(ptr, HUNTER_FADEIN_INDEX_SID);
	int* sid_index = tds_object_get_ipart(ptr, HUNTER_FADEIN_INDEX_SID_INDEX);

	struct tds_string* str = NULL;

	int sid_val = 0;

	if (sid_index) {
		sid_val = *sid_index;
	}

	if (sid) {
		str = tds_stringdb_get(tds_engine_global->stringdb_handle, sid, strlen(sid), sid_val);
	}

	tds_engine_broadcast(tds_engine_global, MSG_FADE_REQ_TEXT, str);
	tds_engine_broadcast(tds_engine_global, MSG_FADE_REQ_NONE, ptr);
}

void obj_fade_in_destroy(struct tds_object* ptr) {
}

void obj_fade_in_update(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;
}

void obj_fade_in_draw(struct tds_object* ptr) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;
}

void obj_fade_in_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_fade_in_data* data = (struct obj_fade_in_data*) ptr->object_data;

	switch (msg) {
	case MSG_FADE_NONE:
		if (param == ptr) {
			tds_engine_broadcast(tds_engine_global, MSG_FADE_REQ_TEXT, NULL);
		}
		break;
	}
}
