#include "savestation.h"
#include "camera.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../tds_game/game_msg.h"
#include "../save.h"

struct tds_object_type obj_savestation_type = {
	.type_name = "obj_savestation",
	.default_sprite = "spr_savestation",
	.data_size = sizeof(struct obj_savestation_data),
	.func_init = obj_savestation_init,
	.func_destroy = obj_savestation_destroy,
	.func_update = obj_savestation_update,
	.func_draw = obj_savestation_draw,
	.func_msg = obj_savestation_msg,
	.save = 1
};

void obj_savestation_init(struct tds_object* ptr) {
	ptr->cbox_width = 0.3f;
	ptr->cbox_height = 0.9f;

	int* id = tds_object_get_ipart(ptr, HUNTER_SAVESTATION_INDEX_ID);
	if (id) {
		tds_logf(TDS_LOG_DEBUG, "Savestation created; ipart%d = %d\n", HUNTER_SAVESTATION_INDEX_ID, *id);
	} else {
		tds_logf(TDS_LOG_WARNING, "Savestation created; ipart%d not set\n", HUNTER_SAVESTATION_INDEX_ID);
	}
}

void obj_savestation_destroy(struct tds_object* ptr) {
}

void obj_savestation_update(struct tds_object* ptr) {
	struct obj_savestation_data* data = (struct obj_savestation_data*) ptr->object_data;
	struct tds_object* player = tds_engine_get_object_by_type(tds_engine_global, "obj_player");
	struct tds_savestate_entry entry = tds_savestate_get(tds_engine_global->savestate_handle, HUNTER_SAVE_SPAWN_ID);
	int* id = tds_object_get_ipart(ptr, HUNTER_SAVESTATION_INDEX_ID);

	unsigned int* sid = (unsigned int*) entry.data;

	if (id && ((sid && *sid != *id) || !sid)) {
		if (tds_collision_get_overlap(ptr, player)) {
			tds_savestate_set(tds_engine_global->savestate_handle, HUNTER_SAVE_SPAWN_ID, id, sizeof *id);
			tds_savestate_write(tds_engine_global->savestate_handle);
		}
	}

	data->dt_glow += 0.02f;
}

void obj_savestation_draw(struct tds_object* ptr) {
	struct obj_savestation_data* data = (struct obj_savestation_data*) ptr->object_data;

	float glow_mod = ((sin(data->dt_glow) + 1.0f) / 2.0f) * 0.01f;

	struct tds_render_light lt_glow = {TDS_RENDER_LIGHT_POINT, ptr->x, ptr->y, 0.0f, 0.01f + glow_mod, 0.02f + glow_mod * 2.0f, 25.0f, NULL};
	tds_render_submit_light(tds_engine_global->render_handle, lt_glow);
}

void obj_savestation_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	int* id = tds_object_get_ipart(ptr, HUNTER_SAVESTATION_INDEX_ID), t_id = 0;

	switch (msg) {
	case TDS_GAME_MSG_SAVESTATION_QUERY:
		t_id = *((int*) param);
		if (id) {
			if (*id == t_id) {
				tds_engine_broadcast(tds_engine_global, TDS_GAME_MSG_SAVESTATION_START, ptr);
			}
		}
		break;
	}
}
