#pragma once

#include <stddef.h>

#include "gamedefs.h"


typedef struct _player_t {
	char* name;
	size_t score;
	checker_color color;
} player_t;

typedef player_t* player;


player create_player(const char* name, const checker_color color);

void destroy_player(player p);
