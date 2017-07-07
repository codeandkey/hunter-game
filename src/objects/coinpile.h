#pragma once

#include <tds/object.h>
#include <tds/clock.h>
#include <tds/font.h>
#include <tds/sound_source.h>

#define HUNTER_COINPILE_INDEX_AMT 0
#define HUNTER_COINPILE_INDEX_SAVE 1

#define HUNTER_COINPILE_DEFAULT_AMT 5
#define HUNTER_COINPILE_SPEED 0.03
#define HUNTER_COINPILE_ALPHA_DECAY 0.02
#define HUNTER_COINPILE_TEXT_SPEED 0.03
#define HUNTER_COINPILE_TEXT_ALPHA_DECAY 0.008

struct tds_object_type obj_coinpile_type;

void obj_coinpile_init(struct tds_object* ptr);
void obj_coinpile_destroy(struct tds_object* ptr);
void obj_coinpile_update(struct tds_object* ptr);
void obj_coinpile_draw(struct tds_object* ptr);
void obj_coinpile_msg(struct tds_object* ptr, struct tds_object* sender, int msg, void* param);

struct obj_coinpile_data {
	struct tds_object* player;
	int amt, save, picked;
	struct tds_sound_source* src;
	struct tds_font* font;
	float text_a, text_y;
};
