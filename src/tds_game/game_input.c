#include "game_input.h"

int TDS_GAME_INPUT_QUIT = 0;
int TDS_GAME_INPUT_MOVE_LEFT = 1;
int TDS_GAME_INPUT_MOVE_RIGHT = 2;
int TDS_GAME_INPUT_MOVE_UP = 3;
int TDS_GAME_INPUT_MOVE_DOWN = 4;
int TDS_GAME_INPUT_JUMP = 5;
int TDS_GAME_INPUT_ATTACK = 6;
int TDS_GAME_INPUT_AXIS_MOVEMENT = 7;
int TDS_GAME_INPUT_RESET = 8;

struct tds_key_map_template _tds_game_input[] = {
	{"quit", "escape"},
	{"left", "leftarrow"},
	{"right", "rightarrow"},
	{"up", "uparrow"},
	{"down", "downarrow"},
	{"jump", "Z"},
	{"attack", "leftmouse"},
	{"axis_movement", "axis0"},
	{"reset", "Q"},
};

struct tds_key_map_template* hunter_get_game_input(void) {
	return _tds_game_input;
}

int hunter_get_game_input_size(void) {
	return sizeof(_tds_game_input) / sizeof(struct tds_key_map_template);
}
