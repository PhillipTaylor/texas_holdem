
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
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>

#define VERSION "0.1"

#define SEMAPHORE_UNIQ_ID "main.c"
#define SEMAPHORE_UNIQ_CH 'A'
#define NUMBER_OF_SEMAPHORES 1
#define SEM_SERV_WT 0
#define SEMAPHORE_PERM 0666

#define ERR -1

/*
 * Main function runs, loads 5 named table objects. For each table
 * created a fork is created for that specific table object that
 * allows each table to run independently.
 *
 * Usernames details are also created and prepared. These are hardcoded
 * until I add database logic later.
 *
 * After the tables are prepared, the main function starts a server
 * listening thread that waits for UNIX sockets to connect to it. When
 * they arrive it spawns a handling thread that does a quick login check
 * and then asks the user which of the named tables they want to play on
 *
 * The socket information is then connected to the table, so the table can
 * then communicate with user. When the table is full, the table algorithm
 * is responcible for basically looping contineously, creating new "games"
 * until only one player is left. He is the winner. When he wins, the table
 * exits.
 *
 * the game loop does all the real work for the texas holdem game, hosting
 * rounds of bets, dealing cards etc.
 */

void table_process(char *table);
void main_game_loop(void);

//APPLICATION WIDE GLOBALS
key_t semaphore_key;

int semaphore_id;

union semun {
    int val;              /* used for SETVAL only */
    struct semid_ds *buf; /* for IPC_STAT and IPC_SET */
    ushort *array;        /* used for GETALL and SETALL */
};

union semun sem_un;

char *table[3];

int main(int argc, char **argv)
{

	stack *my_deck;
	card *current_card;
	int i;

	printf("------------ PRT POKER -----------\nA fully functional texas holdem poker server\n~~~~Version: %s~~~~~~~\n", VERSION);

	config_load("poker.conf");
	logging_init();

	if (semaphore_key = ftok(SEMAPHORE_UNIQ_ID, SEMAPHORE_UNIQ_CH) == ERR)
	{
		logging_critical("Unable to get a key\n");
		exit(2);		
	}

	if ((semaphore_id = semget(semaphore_key, NUMBER_OF_SEMAPHORES, SEMAPHORE_PERM | IPC_CREAT)) == ERR)
	{
		logging_critical("Unable to get a semaphore\n");
		exit(2);
	}
	
	//Semaphore initialisation block
	sem_un.val = 1;
	if (semctl(semaphore_id, SEM_SERV_WT, SETVAL, sem_un) == ERR)
	{
		logging_critical("Semctl function failed\n");
		exit(2);
	}

	table[0] = "table one";
	table[1] = "table two";
	table[2] = "table three";

	//for each table
	for (i = 0; i < 3; i++)
	{
		logging_debug_low("forking table");
		if (fork())
			table_process(table[i]);
	}

	//ok let's let the players join the server
	if (fork())
		wait_for_players();

	logging_debug_low("Generating a deck\n");

	my_deck = generate_new_deck();

	logging_debug_low("deck generated\n");

	for (i = 0; i < 52; i++)
	{
		current_card = (card*) stack_pop(my_deck);

		printf("card: %s\n", card_tostring(current_card));

		free(current_card);

	}

	printf("application ended\n");

	return 0;
}

void table_process(char *table_name)
{
	logging_debug_low("table process\n");
}

void main_game_loop(void)
{
	logging_debug_low("main_game_loop\n");
}
