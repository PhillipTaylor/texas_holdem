
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
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "linkedlist.h"
#include "globals.h"
#include "card.h"
#include "logging.h"
#include "stack.h"
#include "servers.h"
#include "player.h"

#define VERSION "0.1"

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

void table_process(int tables_id);
void main_game_loop(void);

int main(int argc, char **argv)
{
	int main_process_id;
	int connection_process_id;

	char table_config_name[8];
	char *table_name;
	stack *my_deck;
	card *current_card;
	int i;
	int s;

	printf("------------ PRT POKER -----------\nA texas holdem poker server\n~~~~Version: %s~~~~~~~\n", VERSION);

	config_load("poker.conf");
	logging_init();

	//read int the list of tables from the configuration file

	config_get_int("players_per_table", &player_count);
	logging_info("players per table: %i", *player_count);

	config_get_int("table_count", &table_count);
	logging_info("number of tables: %i", *table_count);

	table_names = malloc(sizeof(char*) * (*table_count));

	logging_info("Table count: %d", *table_count);

	for (i = 0; i < *table_count; i++)
	{
		sprintf(table_config_name, "table_%d", (i + 1));	
		config_get_string(table_config_name, &table_name);

		table_names[i] = table_name;

		logging_info("Created table %d: %s", i, table_name);

		if (fork() == 0)
		{
			table_process(i);
			_exit(0);
		}
	}

	main_process_id = getpid();

	connection_process_id = fork();
	logging_info("connecton process: %d", connection_process_id);

	if (connection_process_id == 0)
	{
		wait_for_players();
		_exit(0);
	}
	else
		waitpid(connection_process_id, &s, 0);


	logging_info("Application ended");
	return 0;
}

void table_process(int table_id)
{
	linkedlist *players;
	char fifo_name[255];
	int player_add_fd;
	player *p;
	int players_added;

	logging_info("table %s (%i) running in process %d", table_names[table_id], table_id, getpid());
	
	players = linkedlist_new();
	
	//create a fifo_pipe for the table name so we can recieve players
	sprintf(fifo_name, "fifo_table_%s\n", table_names[table_id]);
	mknod(fifo_name, S_IFIFO | 0666, 0);
	player_add_fd = open(fifo_name, O_RDONLY);

	players_added = *player_count;
	while (players_added > 0)
	{
		logging_info("still waiting for %i players to join %s", players_added, table_names[table_id]);
		read(player_add_fd, p, 1);

		players_added--;
	}

	logging_info("table %s ready to start!!!!!", table_names[table_id]);
}

void main_game_loop(void)
{
	logging_debug("main_game_loop");
}
