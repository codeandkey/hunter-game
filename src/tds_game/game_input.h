#pragma once

#include <tds/key_map.h>

struct tds_key_map_template* hunter_get_game_input(void);
int hunter_get_game_input_size(void);

/* Declare all game inputs here, define them in the C source. */

int TDS_GAME_INPUT_QUIT;
int TDS_GAME_INPUT_MOVE_LEFT, TDS_GAME_INPUT_MOVE_RIGHT, TDS_GAME_INPUT_MOVE_UP, TDS_GAME_INPUT_MOVE_DOWN;
int TDS_GAME_INPUT_ATTACK;
