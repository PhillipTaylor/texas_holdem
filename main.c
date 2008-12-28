
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
#include <sys/msg.h>
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

void table_process(long tables_id);
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

	if (key = ftok("/mnt/mydocuments/git/texas_holdem/poker", 'a') == -1)
	{
		logging_critical("Call to ftok failed");
		exit(1);
	}

	if ((msg_queue = msgget(key, 0644 | IPC_CREAT)) == -1)
	{
		logging_critical("call to msgget failed");
		exit(1);
	}
		logging_info("System V Message Queue Id: %i", msg_queue);

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
			table_process((long)i);
			_exit(0);
		}
	}

	main_process_id = getpid();

	connection_process_id = fork();

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

void table_process(long table_id)
{
	player **all_players;
	player *p;
	int players_added;

	all_players = malloc(sizeof(player*) * (*player_count));

	players_added = *player_count;
	while (players_added > 0)
	{
		logging_info("table %i (%s) pid: %i reciving on mtype %i, waiting for players: %i",
			table_id, table_names[table_id],
			getpid(), (long)(table_id + MSG_QUEUE_OFFSET),
			players_added);
		
		p = malloc(sizeof(player));

		if (msgrcv(msg_queue, p, sizeof(player), (long)(table_id + MSG_QUEUE_OFFSET), 0) == -1)
		{
			logging_critical("recieving from message queue failed");
			_exit(1);
		}
		
		logging_info("Message recieved by %s", p->connection, table_names[table_id]);

		all_players[players_added] = p;
		players_added--;
	}

	logging_info("table %s ready to start!", table_names[table_id]);

	player_broadcast(all_players, *player_count, "%s is ready to start!!!", table_names[table_id]);

}

void main_game_loop(void)
{
	logging_debug("main_game_loop");
}
