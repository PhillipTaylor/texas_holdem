
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
#include <string.h>
#include <stdarg.h>

#include "card.h"
#include "player.h"
#include "hand.h"
#include "simplecardalgorithms.h"
#include "handdetection.h"
#include "linkedlist.h"
#include "configuration.h"

//functions in this file, presented in the order in which they are written

int main(int argc, char **argv);
void table_start(linkedlist *players, int smallBlind);
void broadcast_message(linkedlist *players, char *message);
bool round_of_betting(linkedlist *players,linkedlist *position, int bigBlind);
bool active_contributions_equal(linkedlist *players);
linkedlist *move_to_next_active_player(linkedlist *players, linkedlist *iter);

int main(int argc, char **argv)
{

	printf("Texas Hold'em Poker Server (v. 1.0)\n");
	printf("Written by Phillip Taylor B.Sc. Hons Software Engineering\n");
	printf("Southampton Solent University 2005\n");

	return 1;

}

int old_main(int argc, char **argv)
{
	linkedlist *players = NULL;
	card* c = NULL;
	int numberOfPlayers = 0, initialCash = 0, smallBlind = 0, i = 0;

	char *nameBuff = NULL;
	table *tbl = NULL;
	char buff[255];

	linkedlist *iter = NULL; //iterator
	players = NULL;
	player *p = NULL;
	tbl = NULL;
	
	debugThreshold = 0;

	/* parse command line arguments (which currently only includes debug leel)*/

	for (i = 0; i < argc; i++)
	{
		if ((!strcmp(*(argv + i), "-d")) && (i < (argc - 1)))
				debugThreshold = atoi(*(argv + i + 1));
	}

	log_init();

	debug(1,"debug threshold set to %i\n",debugThreshold);

	printf("Welcome to PRT Poker [rev: %s]\n",BUILD_VERSION);

	load_configuration();

	printf("Enter the number of players: ");
	scanf("%i", &numberOfPlayers);

	printf("Enter the starting amount for each player: ");
	scanf("%i", &initialCash);

	printf("Enter the small blind: ");
	scanf("%i", &smallBlind);

	printf("Please note, the system takes 1 percent of all winnings\n");

	flush_stdin_macro; //flush before switching from scanf to fgets.

	nameBuff = (char*) malloc (sizeof (char) * 255); //get 255 bytes of mem for buffer

	for (i = 0; i < numberOfPlayers; i++)
	{
		printf("Player #%i, Enter your name: ",(i+1));
		fgets(nameBuff,254,stdin);
		
		*strrchr(nameBuff,'\n') = '\0'; //replace newline with end of string

		p = player_new(nameBuff,initialCash);
		//linkedlist_add(&player,p);

	}

	//now check the size of the linkedlist
	iter = players;
	i = 0;

	while (iter != NULL)
	{
		sprintf(buff,"Player %i is called: %s [%x]\n",++i, ((player*)(iter->data))->name, iter);
		broadcast_message(players,buff);
		iter = iter->next;
	}

	broadcast_message(players,"Please get ready to play....PRT Poker!!!!\n");

	table_start(players,smallBlind);

	return 0;
}

void table_start(linkedlist *players, int smallBlind)
{
	int i, minBid, bidBlind, tmp;
	card *c;
	linkedlist *iter;
	player *ply;
	char choice;
	bool betsEqual, escapeGameLoop;
	char buff[255];

	int bigBlind = smallBlind * 2;

	//ok lets start the game!
	table *t = table_new(players, smallBlind);

	//ok, let's generate a deck ofw cards and shuffle it.
	debug(3, "generating and shuffling cards","");

	stack *deck = generate_new_deck();

	//start dealing the cards. (burn the first card purposely)
	free(pop(&deck));

	for (i = 0; i < 2; i++)
	{
		iter = players;

		while (iter != NULL)
		{
			ply = (player*)iter->data;

			if (i == 0)
			{
				ply->cardOne = pop(&deck);
				sprintf(buff,"Your first card is the %s [%x]\n",card_tostring(ply->cardOne),ply->cardOne);
				player_message(ply,buff);
			}
			else
			{
				ply->cardTwo = pop(&deck);
				sprintf(buff,"Your second card is the %s [%x]\n", card_tostring(ply->cardTwo),ply->cardTwo);
				player_message(ply,buff);
			}

			iter = iter->next;
		}
	}

	//start with a round of betting
	round_of_betting(players,iter,bigBlind);

	//burn another card
	free(pop(&deck));

	//generate the table cards.
	t->flop1 = pop(&deck);
	t->flop2 = pop(&deck);
	t->flop3 = pop(&deck);

	sprintf(buff,"The flop consists of:\nflop1: %s\nflop2: %s\nflop3: %s\n",card_tostring(t->flop1),card_tostring(t->flop2),card_tostring(t->flop3));
	printf(buff);
	broadcast_message(players,buff);

	//start a round of betting
	iter = players;

	betsEqual = false;
	minBid = smallBlind;

	round_of_betting(players,iter,bigBlind);

	//burn a card
	free(pop(&deck));

	//the turn
	t->turn = pop(&deck);
	sprintf(buff,"The turn card is the %s\n",card_tostring(t->turn));
	broadcast_message(players,buff);

	round_of_betting(players,iter,bigBlind);

	//the river
	free(pop(&deck));
	t->river = pop(&deck);

	sprintf(buff,"The river is the %s\n",card_tostring(t->river));
	broadcast_message(players,buff);

	round_of_betting(players, iter, bigBlind);

	printf("The game has finished!\n");
}

/* returns true if there are more than one player left
 * and false if only player remains */

bool round_of_betting(linkedlist *players,linkedlist *position, int bigBlind)
{
	player *ply;
	char choice;
	char buff[255];
	int bid = bigBlind;
	int difference;

	while (position != NULL && !active_contributions_equal(players))
	{
		ply = position->data;

		if (ply->contribToPot < bid)
		{
			sprintf(buff,"%s must call %i to stay in.\n",ply->name,(bigBlind - ply->contribToPot));
			broadcast_message(players,buff);
			sprintf(buff, "press C to call, F to fold or R to raise: \n");
			player_message(ply,buff);
		}
		else
		{
			sprintf(buff,"%s players turn.\n",ply->name);
			broadcast_message(players,buff);
			sprintf(buff, "press C to check, F to fold or R to raise: \n");
			player_message(ply,buff);
		}

		choice = player_question(ply);

		if (choice == 'F')
		{
			ply->folded = true;
			sprintf(buff, "%s folded.\n",ply->name);
			broadcast_message(players,buff);
		}
		else if (choice == 'C')
		{
			if (ply->contribToPot < bid)
			{
				difference = bid - ply->contribToPot;
				if (ply->cash < bid)
				{
					sprintf(buff,"You don't have enough to check. You are now all in!.\n");
					player_message(ply,buff);

					sprintf(buff, "%s doesn't have neough money to check. There are now all in.\n");

				}
			}
		}
		else if (choice == 'R')
		{
			//do this too
		}

		position = move_to_next_active_player(players,position);
	}
}

void broadcast_message(linkedlist *players, char *message)
{
	linkedlist *iter = players;
	player *ply;

	while (iter != NULL)
	{

		ply = (player*) iter->data;
		player_message(ply, message);

		iter = iter->next;
	}
}

bool active_contributions_equal(linkedlist *players)
{
	linkedlist *iter = players;
	player *p;
	int standard = -1;

	while (iter != NULL)
	{
		p = iter->data;

		if (!p->folded && !p->allIn)
		{
			if (standard == -1)
				standard = p->contribToPot;
			else if (standard != p->contribToPot)
				return false;
		}

		iter = iter->next;
	}

	return true;
}

/*
 * returns the next value for the iterator.
 * The reason for the two values is because when the list reaches the tail
 * it automatically jumps back to the head and continues looking. The stopping
 * criteria is when the iterator hits it's original value again.
*/

linkedlist *move_to_next_active_player(linkedlist *players, linkedlist *iter)
{
	if (players == NULL)
		return NULL;

	linkedlist *i = iter;
	player *p = i->data;

	//first bump
	if (i == NULL)
	{
		i = players;
		p = i->data;
	}
	else
	{
		i = i->next;
		p = i->data;
	}

	//search loop
	while (i != iter)
	{
		if (i == NULL)
			i = players;

		p = i->data;

		if (!p->folded && !p->allIn)
			return i;

		i = i->next;
	}

	/* If the code got here, we've
	 * gone round in a full loop of
	 * the list and were back where
	 * we started. This player is the
	 * last one who hasn't folded.
	*/

	return NULL;
}


