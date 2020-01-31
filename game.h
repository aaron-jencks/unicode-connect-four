#pragma once

#include <stddef.h>

#include "player.h"
#include "gamedefs.h"

typedef struct _game_t {
	player players[2];
	board_space board[6][7];
	checker_color turn_color;
	unsigned char fullness: 7;
	size_t turn;
	size_t round;
} game_t;

typedef game_t* game;


game create_game(player p1, player p2, checker_color init_turn);

void destroy_game(game g);

game game_reset(game g);

player game_get_current_player(game g);

unsigned game_piece_drop(game g, unsigned col);

unsigned char game_is_win(game g, checker_color color);
