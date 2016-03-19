#pragma once

#include <tds/effect.h>
#include <tds/clock.h>

#define EFFECT_RAIN_DROPWIDTH 0.005f
#define EFFECT_RAIN_DROPHEIGHT 3.0f
#define EFFECT_RAIN_DROPCOUNT 200
#define EFFECT_RAIN_SPEED 0.2f
#define EFFECT_RAIN_INTERVAL 14
#define EFFECT_RAIN_INTERVAL_VARIANCE 5
#define EFFECT_RAIN_ALPHA 0.1f
#define EFFECT_RAIN_ALPHA_VARIANCE 0.2f

extern struct tds_effect_type effect_rain_type;

struct tds_effect_particle_state effect_rain_init(void);
void effect_rain_free(struct tds_effect_particle_state* state);
void effect_rain_update(struct tds_effect_particle_state* state);
void effect_rain_render(struct tds_effect_particle_state* state);

struct effect_rain_data {
	tds_clock_point interval;
	int on_screen, variance;
};
