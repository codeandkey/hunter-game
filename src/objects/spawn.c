#include "spawn.h"
#include "camera.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../msg.h"
#include "../save.h"

struct tds_object_type obj_spawn_type = {
	.type_name = "obj_spawn",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_spawn_data),
	.func_init = obj_spawn_init,
	.func_destroy = obj_spawn_destroy,
	.func_update = obj_spawn_update,
	.func_draw = obj_spawn_draw,
	.func_msg = obj_spawn_msg,
	.save = 1
};

void obj_spawn_init(struct tds_object* ptr) {
}

void obj_spawn_destroy(struct tds_object* ptr) {
}

void obj_spawn_update(struct tds_object* ptr) {
}

void obj_spawn_draw(struct tds_object* ptr) {
}

void obj_spawn_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
	int* id = tds_object_get_ipart(ptr, HUNTER_SPAWN_INDEX_ID), t_id = 0;

	switch (msg) {
	case MSG_SAVESTATION_QUERY:
		t_id = *((int*) param);
		if (id) {
			if (*id == t_id) {
				tds_engine_broadcast(tds_engine_global, MSG_SAVESTATION_START, ptr);
			}
		}
		break;
	}
}
