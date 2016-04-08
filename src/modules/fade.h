#pragma once

/* this object controls and animates the fade level */
/* it also floods the message queue with useless junk */

#include <tds/tds.h>

#include "../msg.h"

#define MOD_FADE_STATE_BLACK 0
#define MOD_FADE_STATE_NONE 1
#define MOD_FADE_STATE_TO_BLACK 2
#define MOD_FADE_STATE_TO_NONE 3

#define MOD_FADE_SLOPE 0.008f
#define MOD_FADE_TEXT_SLOPE 0.008f
#define MOD_FADE_TEXT_DELAY 2500

#define MOD_FADE_FONT "fade"

struct mod_fade {
	float fade, text_fade;
	struct tds_object* resp_object;
	struct tds_string* display_text;
	struct tds_font* display_font;
	int state;
	tds_clock_point cp;
};

void mod_fade_init(void* data);
void mod_fade_destroy(void* data);
void mod_fade_update(void* data);
void mod_fade_draw(void* data);
void mod_fade_msg(void* data, int msg, void* param);

struct tds_module_template mod_fade_type;
