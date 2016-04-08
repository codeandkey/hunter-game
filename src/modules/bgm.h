#pragma once

#include <tds/tds.h>

#define MOD_BGM_STATE_STOPPED 0
#define MOD_BGM_STATE_FADINGIN 1
#define MOD_BGM_STATE_PLAYING 2
#define MOD_BGM_STATE_FADINGOUT 3

#define MOD_BGM_VOLUME_SLOPE 0.005f

struct mod_bgm_entry {
	char* name;
	int state;
	float volume;
	tds_clock_point cp;
	int loop;
	struct tds_sound_source* snd_src;
	struct mod_bgm_entry* next;
};

struct mod_bgm {
	struct mod_bgm_entry* entries;
};

void mod_bgm_init(void* data);
void mod_bgm_destroy(void* data);
void mod_bgm_update(void* data);
void mod_bgm_draw(void* data);
void mod_bgm_msg(void* data, int msg, void* param);

struct mod_bgm_entry* mod_bgm_get_entry(struct mod_bgm* ptr, char* name);

struct tds_module_template mod_bgm_type;
