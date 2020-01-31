#include <stdlib.h>

#include "game.h"
#include "gamedefs.h"
#include "player.h"


game create_game(player p1, player p2, checker_color init_turn)
{
	game g = calloc(1, sizeof(game_t));
	if(!g) exit(1);

	g->players[0] = create_player(p1->name, p1->color);
	g->players[1] = create_player(p2->name, p2->color);

	g->turn_color = init_turn;

	g = game_reset(g);

	return g;
}

void destroy_game(game g)
{
	if(g)
	{
		if(g->players[0]) destroy_player(g->players[0]);
		if(g->players[1]) destroy_player(g->players[1]);
		free(g);
	}
}

game game_reset(game g)
{
	if(g)
	{
		for(int row = 0; row < 6; row++)
			for(int col = 0; col < 7; col++)
				g->board[row][col] = EMPTY;
	}

	return g;
}

player game_get_current_player(game g)
{
	if(g)
	{
		for(int i = 0; i < 2; i++)
			if(g->players[i]->color == g->turn_color) return g->players[i];
	}

	return 0;
}

unsigned game_piece_drop(game g, unsigned col)
{
	if(g && g->fullness < 42 && col < 7)
	{
		int r_final = 5;
		for(int row = 1; row < 6; row++)
		{
			if(g->board[row][col] != EMPTY)
				if(g->board[row - 1][col] != EMPTY) return 1;
				else
				{
					r_final = row - 1;
					break;
				}
		}

		player current_p = game_get_current_player(g);
		if(!current_p) return 2;

		g->board[r_final][col] = (current_p->color == RED) ? OC_RED : OC_BLACK;
		g->turn++;
		g->fullness++;
		g->turn_color = (g->turn_color == RED) ? BLACK : RED;

		return 0;
	}

	return 3;
}

unsigned char game_is_win(game g, checker_color color)
{
	// Check diagonals (both directions, right and left), and horizontal, and vertical
	board_space target = (color == RED) ? OC_RED : OC_BLACK;

	for(int row = 0; row < 6; row++)
	{
		for(int col = 0; col < 7; col++)
		{
			if(g->board[row][col] == target)
			{
				if(row > 2 && g->board[row - 3][col] == target && g->board[row - 2][col] == target && g->board[row - 1][col] == target) return 1;
				if(col > 2 && g->board[row][col - 3] == target && g->board[row][col - 2] == target && g->board[row][col - 1] == target) return 1;
				if(row > 2 && col > 2 && g->board[row - 3][col - 3] == target && g->board[row - 2][col - 2] == target && g->board[row - 1][col - 1] == target) return 1;
				if(row > 2 && col < 4 && g->board[row - 3][col + 3] == target && g->board[row - 2][col + 2] == target && g->board[row - 1][col + 1] == target) return 1;
			}
		}
	}

	return 0;
}
