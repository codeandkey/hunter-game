#include "trigger_soundoff.h"
#include "camera.h"

#include <tds/tds.h>
#include <tds/render.h>

#include "../msg.h"
#include "../save.h"

#include <stdlib.h>

struct tds_object_type obj_trigger_soundoff_type = {
	.type_name = "obj_trigger_soundoff",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_trigger_soundoff_data),
	.func_init = obj_trigger_soundoff_init,
	.func_destroy = obj_trigger_soundoff_destroy,
	.func_update = obj_trigger_soundoff_update,
	.func_draw = obj_trigger_soundoff_draw,
	.func_msg = obj_trigger_soundoff_msg,
	.save = 1,
};

void obj_trigger_soundoff_init(struct tds_object* ptr) {
	struct obj_trigger_soundoff_data* data = (struct obj_trigger_soundoff_data*) ptr->object_data;

	data->name = tds_object_get_spart(ptr, HUNTER_TRIGGERSOFF_INDEX_NAME);
	data->activated = 0;
}

void obj_trigger_soundoff_destroy(struct tds_object* ptr) {
}

void obj_trigger_soundoff_update(struct tds_object* ptr) {
	struct obj_trigger_soundoff_data* data = (struct obj_trigger_soundoff_data*) ptr->object_data;

	if (!data->activated) {
		return;
	}

	if (!data->player) {
		data->player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");

		if (!data->player) {
			tds_logf(TDS_LOG_WARNING, "Failed to locate player object. Will try again next frame.\n");
			return;
		}
	}

	if (tds_collision_get_overlap(ptr, data->player)) {
		tds_logf(TDS_LOG_DEBUG, "Triggered, sending BGM stop for %s\n", data->name);
		tds_engine_broadcast(tds_engine_global, MSG_BGM_REQ_STOP, data->name);
		data->activated = 0;
	}
}

void obj_trigger_soundoff_draw(struct tds_object* ptr) {
}

void obj_trigger_soundoff_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	struct obj_trigger_soundoff_data* data = (struct obj_trigger_soundoff_data*) ptr->object_data;

	switch(msg) {
	case MSG_BGM_START:
		data->activated = 1;
		break;
	}
}
