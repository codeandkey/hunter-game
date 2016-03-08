#include "effect_rain.h"

#include <stdlib.h>
#include <tds/tds.h>

struct tds_effect_type effect_rain_type = {
	.name = "rain",
	.func_init = effect_rain_init,
	.func_free = effect_rain_free,
	.func_update = effect_rain_update,
	.func_render = effect_rain_render
};

struct tds_effect_particle_state effect_rain_init(void) {
	/* We don't actually need to have any userdata for the rain effect -- we only randomize positions. */

	struct tds_effect_particle_state output;
	output.tex = tds_texture_cache_get(tds_engine_global->tc_handle, "res/sprites/effect_rain_1x6.png", 1, 6, 0, 0);

	struct tds_vertex verts[] = {
		{ -EFFECT_RAIN_DROPWIDTH / 2.0f, EFFECT_RAIN_DROPHEIGHT / 2.0f, 0.0f, 0.0f, 1.0f },
		{ EFFECT_RAIN_DROPWIDTH / 2.0f, -EFFECT_RAIN_DROPHEIGHT / 2.0f, 0.0f, 1.0f, 0.0f },
		{ EFFECT_RAIN_DROPWIDTH / 2.0f, EFFECT_RAIN_DROPHEIGHT / 2.0f, 0.0f, 1.0f, 1.0f },
		{ -EFFECT_RAIN_DROPWIDTH / 2.0f, EFFECT_RAIN_DROPHEIGHT / 2.0f, 0.0f, 0.0f, 1.0f },
		{ EFFECT_RAIN_DROPWIDTH / 2.0f, -EFFECT_RAIN_DROPHEIGHT / 2.0f, 0.0f, 1.0f, 0.0f },
		{ -EFFECT_RAIN_DROPWIDTH / 2.0f, -EFFECT_RAIN_DROPHEIGHT / 2.0f, 0.0f, 0.0f, 0.0f }
	};

	output.vb = tds_vertex_buffer_create(verts, sizeof verts / sizeof *verts, GL_TRIANGLES);
	output.part_count = EFFECT_RAIN_DROPCOUNT;

	output.part_buf = tds_malloc(sizeof(output.part_buf[0]) * output.part_count);
	output.data = tds_malloc(sizeof(struct effect_rain_data));

	((struct effect_rain_data*) output.data)->interval = tds_clock_get_point();

	for (int i = 0; i < output.part_count; ++i) {
		output.part_buf[i].r = output.part_buf[i].g = output.part_buf[i].b = output.part_buf[i].a = 1.0f;
	}

	return output;
}

void effect_rain_free(struct tds_effect_particle_state* state) {
	tds_vertex_buffer_free(state->vb);
	tds_free(state->data);
	tds_free(state->part_buf);
}

void effect_rain_update(struct tds_effect_particle_state* state) {
	struct effect_rain_data* data = (struct effect_rain_data*) state->data;

	if (tds_clock_get_ms(data->interval) >= EFFECT_RAIN_INTERVAL && data->on_screen < state->part_count) {
		data->interval = tds_clock_get_point();

		state->part_buf[data->on_screen++].y = /* TODO */
	}

	float camera_width = tds_engine_global->camera_handle->width;
	float camera_height = tds_engine_global->camera_handle->height;
	float camera_left = tds_engine_global->camera_handle->x - camera_width / 2.0f;
	float camera_bottom = tds_engine_global->camera_handle->y - camera_height / 2.0f;

	const int precision = 1000;

	for (int i = 0; i < state->part_count; ++i) {
		state->part_buf[i].x = ((rand() % precision) / (float) precision) * camera_width + camera_left;
		state->part_buf[i].y += EFFECT_RAIN_SPEED;
		state->part_buf[i].a = ((rand() % precision) / (float) precision) / 2.0f + 0.5f;
	}
}

void effect_rain_render(struct tds_effect_particle_state* state) {
}
