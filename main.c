
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
#include "card.h"
#include "logging.h"
#include "stack.h"
#include <stdlib.h>

#define VERSION "0.1"

int main(int argc, char **argv)
{

	stack *my_deck;
	card *current_card;
	int i;

	printf("------------ PRT POKER -----------\nA fully functional texas holdem poker server\n~~~~Version: %s~~~~~~~\n", VERSION);

	config_load("poker.conf");
	logging_init();

	my_deck = generate_new_deck();

	logging_debug_low("deck generated\n");

	for (i = 0; i < 52; i++)
	{
		current_card = (card*) stack_pop(my_deck);

		printf("card: %s\n", card_tostring(current_card));

		free(current_card);

	}

	printf("application ended\n");

}
