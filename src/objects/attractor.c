#include "attractor.h"

#include <tds/tds.h>

#include "../tds_game/game_input.h"
#include "../tds_game/game_msg.h"

struct tds_object_type obj_attractor_type = {
	.type_name = "obj_attractor",
	.default_sprite = NULL,
	.data_size = sizeof(struct obj_attractor_data),
	.func_init = obj_attractor_init,
	.func_destroy = obj_attractor_destroy,
	.func_update = obj_attractor_update,
	.func_draw = obj_attractor_draw,
	.func_msg = obj_attractor_msg,
	.save = 0
};

void obj_attractor_init(struct tds_object* ptr) {
	struct obj_attractor_data* data = (struct obj_attractor_data*) ptr->object_data;

	float* dist = tds_object_get_fpart(ptr, HUNTER_ATTRACTOR_INDEX_DIST);
	float* factor = tds_object_get_fpart(ptr, HUNTER_ATTRACTOR_INDEX_FACTOR);

	data->dist = dist ? *dist : 5.0f;
	data->factor = factor ? *factor : 0.5f;
}

void obj_attractor_destroy(struct tds_object* ptr) {
}

void obj_attractor_update(struct tds_object* ptr) {
}

void obj_attractor_draw(struct tds_object* ptr) {
}

void obj_attractor_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param) {
}
