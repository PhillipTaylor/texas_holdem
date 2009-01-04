
/*
     Phillip Taylor B.Sc. (HONS) Software Engineering. Texas Hold'em Poker Software
     Copyright (C) 2008  Phillip Taylor

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "config.h"
#include "logging.h"
#include "card.h"
#include "stack.h"
#include "player.h"

#define VERSION "0.1"

#define NUM_PLAYERS 4

int main(int argc, char **argv)
{
	int i;
	stack *deck;
	card *c;
	char *tmp;
	player players[NUM_PLAYERS];

	printf("------------ PRT POKER -----------\n");
	printf("A texas holdem poker server\n");
	printf("~~~~Version: %s~~~~~~~\n", VERSION);

	config_load("poker.conf");
	logging_init();

	//In this scenario we have 4 players on the same table.
	deck = generate_new_deck();

	for (i = 0; i < NUM_PLAYERS; i++)
		players[i] = player_new();

//	while (!stack_empty(deck))
//	{
//		c = (card*) stack_pop(deck);
//		tmp = card_tostring(c);
//		printf("Card is a %s\n", tmp);
//		free(tmp);
//	}

	logging_shutdown();
	printf("Application ended\n");
	return 0;
}

