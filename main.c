#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#include "utf8.h"
#include "player.h"
#include "game.h"
#include "stringbuilder.h"
#include "gamedefs.h"
#include "display.h"


void display_board(game g)
{
	stringbuilder result = create_stringbuilder();

//	u_int32_t* bars = malloc(sizeof(u_int32_t) * 16);
//	if(!bars) exit(1);
//	bars[15] = 0;
//	for(int i = 0; i < 16; i++)
//	{
//		if(i == 0 || i == 14) bars[i] = (i) ? 0xE2948C : 0xE29490;
//		else bars[i] = (i % 2) ?  0xE29480 : 0xE294AC;
//	}

	sb_append_ch(result, " A B C D E F G \n");
	for(int i = 0; i < 15; i++)
		if(i ==0 || i == 14) sb_append_ch(result, (i) ? "\\u2510" : "\\u250c");
		else sb_append_ch(result, (i % 2) ? "\\u2500" : "\\u252c");
	sb_append_ch(result, "\n");

//	sb_append(result, bars);

	for(int row = 0; row < 6; row++)
	{
		sb_append_ch(result, "\\u2502");
		for(int col = 0; col < 7; col++)
		{
			board_space space = g->board[row][col];
			if(space == EMPTY) sb_append_ch(result, "\\u25cb");
			else
			{
				sb_append_ch(result, (space == OC_RED) ? "\\u001b[31m" : "\\u001b[34m");
				sb_append_ch(result, "\\u25cf\\u001b[0m");
			}

			sb_append_ch(result, "\\u2502");
		}
		sb_append_ch(result, "\n");
	}

	for(int i = 0; i < 15; i++)
		if(i ==0 || i == 14) sb_append_ch(result, (i) ? "\\u2518" : "\\u2514");
		else sb_append_ch(result, (i % 2) ? "\\u2500" : "\\u2534");
	sb_append_ch(result, "\nType the letter of the column you'd like to place into.\n");

	char* pstr = sb_concat(result), *pbuf;
	destroy_stringbuilder(result);

	size_t plen = u8_strlen(pstr);
	pbuf = malloc(sizeof(char) * (plen + 1));
	u8_unescape(pbuf, plen + 1, pstr);
	printf(pbuf);
	free(pstr); free(pbuf);
}


void display_stats(game g)
{
	if(g)
	{
		stringbuilder stats = create_stringbuilder();
		char* buf;

		sb_append_ch(stats, "Connect 4 Statistics:\n");

		buf = calloc(19, sizeof(char));
		if(!buf) exit(1);

		sprintf(buf, "Round: %lu\n", g->round);
		sb_append_ch(stats, buf);

		for(int p = 0; p < 2; p++)
		{
			buf = realloc(buf, sizeof(char) * (28 + strlen(g->players[p]->name) + 1));
			if(!buf) exit(1);

			sprintf(buf, "Player %d: %s, Score: %lu\n", p + 1, g->players[p]->name, g->players[p]->score);
			sb_append_ch(stats, buf);
		}

		free(buf);

		char* sbstr = sb_concat(stats);
		destroy_stringbuilder(stats);

		char* pstr = boxed_text(sbstr);
		printf("%s\n", pstr);
		free(pstr);
	}
}

void take_turn(game g)
{
	while(1)
	{
		clear_screen();
		display_stats(g);
		display_board(g);

		printf("%s: Please enter the letter of the column you'd like to select: ", game_get_current_player(g)->name);
		char* column = readline("");

		if(strlen(column) > 1 || ((column[0] < 'a' || column[0] > 'g' && !(column[0] >= 'A' || column[0] <= 'G')) ||
					 (column[0] < 'A' || column[0] > 'G') && !(column[0] >= 'a' || column[0] <= 'g')))
		{
			printf("Please select a single column (a-g)\n");
			free(column);
			pause();
			continue;
		}

		unsigned col_index = column[0] - ((column[0] >= 'a' && column[0] <= 'g') ? 'a' : 'A');

		if(game_piece_drop(g, col_index))
		{
			printf("Sorry, that column (%c) is either full, or invalid, try again\n", col_index + 'a');
			free(column);
			pause();
			continue;
		}

		free(column);

		break;
	}
}

unsigned char coin_toss()
{
	return rand() % 2;
}


int main() {
	setlocale(LC_ALL, "UTF-8");
	clear_screen();
	srand(time(0));

	char *p1name, *p2name, *newline;
	p1name = readline("Please enter a name for player 1: ");
	//newline = strchr(p1name, '\n');
	//*newline = 0;

	p2name = readline("Please enter a name for player 2: ");
	//newline = strchr(p2name, '\n');
	//*newline = 0;

//	printf("Hello World\n");
	player p1 = create_player(p1name, RED), p2 = create_player(p2name, BLACK);
	free(p1name); free(p2name);

	game g = create_game(p1, p2, RED);

	unsigned char first = 1, done = 0;
	while(!done)
	{
		game_reset(g);	// Clears the board

		checker_color c_init = (first) ? ((coin_toss()) ? RED : BLACK) : ((g->turn_color == RED) ? BLACK : RED);	// Either flip a coin, or use the loser from the last round
		printf("%s is going first!\n", game_get_current_player(g)->name);

		if(first) first = 0;

		while(1)
		{
			player p = game_get_current_player(g);
			take_turn(g);
			if(game_is_win(g, p->color))
			{
				clear_screen();
//				display_stats(g);
				display_board(g);

				printf("%s Wins!\n", p->name);
				p->score++;
				break;
			}
			else if(g->fullness == 42)
			{
				clear_screen();
				display_board(g);

				printf("It's a draw!\n");
				break;
			}
		}

		while(1)
		{
			printf("Play again?(y/n) ");
			char* resp = readline("");

			if(strlen(resp) > 1 || (resp[0] != 'y' && resp[0] != 'Y' && resp[0] != 'n' && resp[0] != 'N'))
			{
				printf("Please enter either 'y' or 'n'.\n");
				free(resp);
				pause();
				continue;
			}

			if(resp[0] == 'y' || resp[0] == 'Y')
			{
				g->round++;
				free(resp);
				break;
			}
			else
			{
				done = 1;
				free(resp);
				break;
			}

			free(resp);
		}
	}

	// Cleanup
	destroy_game(g);
	destroy_player(p1);
	destroy_player(p2);
}
