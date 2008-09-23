
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

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "card.h"
#include "stack.h"
#include "configuration.h"

#define NUM_CARDS 52

card *card_new(int value, char suit)
{
	card *c = (card*) malloc (sizeof(card));
	c->value = value;
	c->suit = suit;

	return c;
}

void card_free(card* c)
{
	free(c);
}

char *card_tostring(card* c)
{
    char *cardValue;
    char *cardSuit;
    char *retval = NULL;

    switch (c->value)
    {
	case 2:
		cardValue = "Two";
		break;
	case 3:
		cardValue = "Three";
		break;
	case 4:
		cardValue = "Four";
		break;
	case 5:
		cardValue = "Five";
		break;
	case 6:
		cardValue = "Six";
		break;
	case 7:
		cardValue = "Seven";
		break;
	case 8:
		cardValue = "Eight";
		break;
	case 9:
		cardValue = "Nine";
		break;
	case 10:
		cardValue = "Ten";
		break;
	case 11:
		cardValue = "Jack";
		break;
	case 12:
		cardValue = "Queen";
		break;
	case 13:
		cardValue = "King";
		break;
	case 14:
		cardValue = "Ace";
		break;
    }

    if (c->suit == 'x') cardSuit = "Unset";
    if (c->suit == 'd') cardSuit = "Diamonds";
    if (c->suit == 'c') cardSuit = "Clubs";
    if (c->suit == 's') cardSuit = "Spades";
    if (c->suit == 'h') cardSuit = "Hearts";

    retval = (char*) malloc (sizeof(char) * (strlen(cardValue) + strlen(cardSuit) + 5)); //magic number 5 is to put in " of " and \0.

    strcpy(retval, cardValue);
    strcat(retval, " of ");
    strcat(retval, cardSuit);

    //*(retval + strlen(cardValue) + strlen(cardSuit) + 4) = '\0';

    return retval;
};

stack *generate_new_deck()
{
	int i, o;
	card *deckArray[NUM_CARDS];
	stack *s;
	int a, b;
	card *tmp;

	debug (3,"Generating a new deck...","");

	s = (stack*)stack_new();

	for (i = 2, o = 0; i < 15; i++)
	{
		deckArray[o++] = card_new(i,'d');
		deckArray[o++] = card_new(i,'c');
		deckArray[o++] = card_new(i,'h');
		deckArray[o++] = card_new(i,'s');

		//printf("s addr = %x\n",s);
		//push(&s,card_new(i,'d'));
		//push(&s,card_new(i,'c'));
		//push(&s,card_new(i,'h'));
		//push(&s,card_new(i,'s'));
	}

	//we load the data into an array first because it makes it easier to sort that way
	//initialise the random number generator
	srand(time(0));

	//This is an implementation of the Fisher-Yates algorithm.
	//It is unbias (assuming rand() mod 52 isn't biased) and
	//effecient.

	b = NUM_CARDS;

	while (b > 0)
	{
		a = rand() % 52;

		b--;
		tmp = deckArray[b];
		deckArray[b] = deckArray[a];
		deckArray[a] = tmp;

		debug (5, "i=%i,a=%i,b=%i\n",i,a,b);
	}

	//now build the stack
	for (i = 0; i < 52; i++)
		push(&s,deckArray[i]);

	return s;

}

