
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
#include <poll.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "config.h"

#include "util.h"
#include "logging.h"
#include "card.h"
#include "linkedlist.h"
#include "player.h"
#include "table.h"

#define VERSION "0.2"

//Max unaccepted connections to hold and port to recieve on
#define MAX_CONN 10
#define PORT 8888


//This is how long poll() waits before returning. Players have 30 seconds
//possibly upto 40 depending on other players before their go is forfitted.
#define SECOND 1000
#define TIMEOUT (10 * SECOND)

//A table waits for this many players to join before it starts.
#define PLAYERS_PER_TABLE 3


//This creates the socket that accepts new sockets.
void create_listen_socket();
int accepting_socket_fd;

//When players join the game they are put on this linkedlist whilst asked their
//username, password and the table they want to play on. Until they have selected
//their table they sit here. When they have chosen their table they are removed from
//this list and placed on the table list.
linkedlist *limbo_players;

//List of all active tables.
linkedlist *tables;

int main(int argc, char **argv); //Inits the program

//This is the main game loop. It plays until there are no more connections available.
//It basically loops, builds a table of sockets, calls poll and decides what to do with
//the one that answered.
int gameLoop();

void new_connection_request(int fd);
void supplied_username(player *p);
void supplied_password(player *p);
void supplied_table(player *p);

int main(int argc, char **argv)
{
	printf("------------ PRT POKER -----------\n");
	printf("A texas holdem poker server\n");
	printf("~~~~Version: %s~~~~~~~\n", VERSION);

	//start configuration and logging services
	config_load("poker.conf");
	logging_init();

	//prepare linkedlists.
	limbo_players = linkedlist_new();
	tables = linkedlist_new();
	
	create_listen_socket();

	while (game_loop());

	logging_shutdown();
	printf("Application ended\n");
	return 0;
}

void create_listen_socket()
{
    struct sockaddr_in a;
    int s;
    int yes;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	logging_critical("Can't create socket in create_listen_socket");
	exit(0);
    }
    yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
            (char *) &yes, sizeof(yes)) < 0) {
	logging_critical("Error in setsockopt in create_listen_socket");
        close(s);
        exit(0);
    }
    memset(&a, 0, sizeof(a));
    a.sin_port = htons(PORT);
    a.sin_family = AF_INET;
    if (bind(s, (struct sockaddr *) &a, sizeof(a)) < 0) {
        logging_critical("Error in bind in create_listen_socket. Did you kill the last instance?");
        close(s);
        exit(0);
    }
    logging_info("Accepting connections on port %d\n", PORT);
    listen(s, 10);
    accepting_socket_fd = s;
}


int game_loop()
{
	struct pollfd *file_descriptors;
	int num_fds, nxt_fd;
	int i, j, poll_result, player_found;
	linkedlist_node *iter;
	table *curr_table;
	player *curr_player;

	//lets go and count all the file descriptors
	//so we know how much memory to allocate for
	//the structure going into poll.
	
	//lets start at one, because we know we're
	//going to include the thread that waits for new
	//connections.
	num_fds = 1;

	//we're going to listen to every socket on the
	//limbo linkedlist, since each of them represents
	//a client at a different stage in the handshake.
	num_fds += linkedlist_count(limbo_players);

	logging_debug("Limbo player count: %i", (num_fds - 1));
	//We allow people to chat during the game so rather
	//than just holding the fd for the person's who go it
	//is in the game, were going to have one for every player
	//in every table!
	iter = tables->head;
	while (iter != NULL)
	{
		curr_table = (table*) iter->data;

		logging_debug("Table %s has %i players", curr_table->name, curr_table->num_players);
		num_fds += curr_table->num_players;
		iter = iter->next;
	}

	logging_debug("Allocate enough space for %i file descriptors!", num_fds);

	file_descriptors = (struct pollfd*) malloc(sizeof(struct pollfd) * num_fds);
	
	//now actually populate all the pollfds with the actual socket info.
	file_descriptors[0].fd = accepting_socket_fd;
	file_descriptors[0].events = POLLIN;
	file_descriptors[0].revents = 0;

	//nxt_fd tracks point of inserting into file_descriptors array
	nxt_fd = 1;

	//get the limbo player's socket on there.
	iter = limbo_players->head;
	while (iter != NULL)
	{
		curr_player = (player*) iter->data;

		file_descriptors[nxt_fd].fd = curr_player->socket;
		file_descriptors[nxt_fd].events = POLLIN | POLLPRI;
		file_descriptors[nxt_fd].revents = 0;

		nxt_fd++;
		iter = iter->next;
	}

	//get the the tables' player's socket.
	iter = tables->head;
	while (iter != NULL)
	{
		curr_table = (table*) iter->data;

		for (i = 0; i < curr_table->num_players; i++)
		{
			file_descriptors[nxt_fd].fd = curr_table->players[i]->socket;
			file_descriptors[nxt_fd].events = POLLIN | POLLPRI;
			file_descriptors[nxt_fd].revents = 0;
			nxt_fd++;
		}

		iter = iter->next;
	}

	//finally! we can pass our structure into poll to wait for events.
	logging_debug("Poll called with %i sockets", num_fds);

	do
	{
		poll_result = poll(file_descriptors, num_fds, TIMEOUT);

		if (poll_result == 0)
			logging_debug("Poll Timeout");

	} while (poll_result == 0); //zero retval == timeout, so just repeat without full loop.

	if (poll_result == -1)
	{
		logging_critical("Poll returned -1 :-(");
		free(file_descriptors);
		return 0;
	}

	//data has been recieved!!!!
	//now we have the pleasure of having to match them back
	//to the original event sources. A painful job!
	
	if (file_descriptors[0].revents != 0)
		new_connection_request(file_descriptors[0].fd);

	//TODO: SKIP LOOP if poll_result == 1 and was accepting socket.

	for (i = 1; i < num_fds && poll_result > 0; i++)
	{
		if (file_descriptors[i].revents != 0)
		{
			//This file descriptor responded. Go find it!
			
			//search the limbo players.
			iter = limbo_players->head;
			while (iter != NULL)
			{
				curr_player = (player*) iter->data;
				if (curr_player->socket == file_descriptors[i].fd)
				{
					if (curr_player->state == USERNAME)
						supplied_username(curr_player);
					else if (curr_player->state == PASSWORD)
						supplied_password(curr_player);
					else if (curr_player->state == TABLE)
						supplied_table(curr_player);
					
					poll_result--;
				}

				iter = iter->next;
			}

			//TODO: Skip this loop if it has already been found above.
			//get the the tables' player's socket.
			iter = tables->head;
			while (iter != NULL)
			{
				curr_table = (table*) iter->data;

				for (j = 0; j < curr_table->num_players; j++)
				{
					if (file_descriptors[i].fd == curr_table->players[j]->socket)
					{
						table_state_changed(curr_table, curr_table->players[j]);
						poll_result--;
					}
				}

				iter = iter->next;
			}
		}
	}

	free(file_descriptors);
	return 1;
}

void new_connection_request(int fd)
{
	struct sockaddr_in their_addr;  // holds their IP address
	socklen_t sin_size;
	player *new_player;


	new_player = player_new();
	sin_size = sizeof their_addr;
	new_player->socket = accept(fd, (struct sockaddr *)&their_addr, &sin_size);
	new_player->state = USERNAME;

	logging_info("New connection request from %s", inet_ntoa(their_addr.sin_addr));

	send_str(new_player->socket, "Username: ");

	linkedlist_add_last(limbo_players, new_player);
}

void supplied_username(player *p)
{
	p->name = recv_str(p->socket);
	logging_info("username: %s", p->name);

	p->state = PASSWORD;
	send_str(p->socket, "Password: ");
}

void supplied_password(player *p)
{
	char *password;
	linkedlist_node *iter;
	table *t;

	password = recv_str(p->socket);
	logging_info("password attempt: %s", password);

	//now give them a list of tables to choose from.
	p->state = TABLE;
	send_str(p->socket, "Password Accepted\nEnter a tablename:\n");
	
	iter = tables->head;
	while (iter != NULL)
	{

		t = (table*) iter->data;

		send_str(p->socket, t->name);

		iter = iter->next;
	}

	send_str(p->socket, "Your choice: ");

}

void supplied_table(player *p)
{
	char *table_choosen;
	linkedlist_node *iter;
	table *t, *found;

	//going to find table with a matching name.
	//or create it if it doesn't exist.
	table_choosen = recv_str(p->socket);

	found = NULL;	
	iter = tables->head;
	while (iter != NULL)
	{
		t = (table*) iter->data;

		if (strncmp(t->name, table_choosen, 254) == 0)
		{
			found = t;
			break;
		}

		iter = iter->next;
	}

	logging_debug("before remove item: %i", linkedlist_count(limbo_players));
	linkedlist_remove_item(limbo_players, p);
	logging_debug("after remove item: %i", linkedlist_count(limbo_players));
	//add to matching table
	if (found != NULL)
	{
		logging_info("player added to table %s", found->name);
		table_add_player(found, p);
	}
	else
	{
		logging_info("new table created: %s", table_choosen);
		t = table_new(table_choosen);
		linkedlist_add_last(tables, t);
		table_add_player(t, p);
	}

	p->state = ON_TABLE;
}
