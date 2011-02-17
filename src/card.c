
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

#include <stdio.h> //for sprintf
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "card.h"
#include "stack.h"
#include "logging.h"

#define NUM_CARDS 52

card *card_new(char suit, int value);

char *card_tostring(card* c)
{
    char *card_value;
    char *card_suit;
    char *retval = NULL;

    switch (c->value)
    {
	case 2:
		card_value = "Two";
		break;
	case 3:
		card_value = "Three";
		break;
	case 4:
		card_value = "Four";
		break;
	case 5:
		card_value = "Five";
		break;
	case 6:
		card_value = "Six";
		break;
	case 7:
		card_value = "Seven";
		break;
	case 8:
		card_value = "Eight";
		break;
	case 9:
		card_value = "Nine";
		break;
	case 10:
		card_value = "Ten";
		break;
	case 11:
		card_value = "Jack";
		break;
	case 12:
		card_value = "Queen";
		break;
	case 13:
		card_value = "King";
		break;
	case 14:
		card_value = "Ace";
		break;
    }

    if (c->suit == 'd') card_suit = "Diamonds";
    if (c->suit == 'c') card_suit = "Clubs";
    if (c->suit == 's') card_suit = "Spades";
    if (c->suit == 'h') card_suit = "Hearts";

    retval = (char*) malloc (sizeof(char) * (strlen(card_value) + strlen(card_suit) + 5)); //magic number 5 is to put in " of " and \0.
    sprintf(retval, "%s of %s", card_value, card_suit);

    return retval;
};

stack *generate_new_deck()
{
	int i, o;
	card *deck_array[NUM_CARDS];
	stack *s;
	int a, b;
	card *tmp;

	logging_debug("Generating a new deck...");

	s = (stack*)stack_new();

	for (i = 2, o = 0; i < 15; i++)
	{
		deck_array[o++] = card_new('d',i);
		deck_array[o++] = card_new('c',i);
		deck_array[o++] = card_new('h',i);
		deck_array[o++] = card_new('s',i);

	}

	//initialise random number generator
	srand(time(0));

	//This is an implementation of the Fisher-Yates algorithm.
	//It is unbias (assuming rand() mod 52 isn't biased) and
	//effecient.

	b = NUM_CARDS;

	while (b > 0)
	{
		a = rand() % NUM_CARDS;

		b--;
		tmp = deck_array[b];
		deck_array[b] = deck_array[a];
		deck_array[a] = tmp;

	}

	//turn the array into a stack.

	for (i = 0; i < NUM_CARDS; i++)
	{
		stack_push(s, deck_array[i]);
	}

	return s;

}

card *card_new(char suit, int value)
{
	card *c = (card*) malloc (sizeof(card));

	c->suit = suit;
	c->value = value;

	return c;
}
