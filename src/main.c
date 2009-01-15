
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

#include "logging.h"
#include "card.h"
#include "stack.h"
#include "player.h"

#define VERSION "0.1"
#define PORT 8888
#define MAX_CONN 10
#define SECOND 1000
#define TIMEOUT (30 * SECOND)

static int listen_socket();

int main(int argc, char **argv)
{
	struct pollfd my_fds[MAX_CONN];          //array of pollfd structures for poll()
	int num_fds;                             //count of how many are being used
	int i, j;                                //for loops
	char buff[255], buff2[255];              //for sending and recieving text
	struct sockaddr_in my_addr, their_addr;  // my address information
	socklen_t sin_size;
	int buff_sz;                             //size of data recieved

	printf("------------ PRT POKER -----------\n");
	printf("A texas holdem poker server\n");
	printf("~~~~Version: %s~~~~~~~\n", VERSION);

	config_load("poker.conf");
	logging_init();

	//I call listen_socket() which creates a socket to listen to
	//this is anchored into my_fds array at element 0.
	my_fds[0].fd = listen_socket();
	my_fds[0].events = POLLIN;
	my_fds[0].revents = 0;

	logging_debug("Listening socket fd locked always at position zero in array: %d\n", my_fds[0].fd);

	//num_fds, the count of items in the array is set to 1
	//because the listen socket is already present
	num_fds = 1;
	
	//This is the main loop.
	//While (true)
	//  set all struct pollfd items revents to 0
	//  call poll
	//  loop through, see if there is data to read
	//      read the data
	//          loop through all sockets (except the listen_socket()) and send the data.
	while (1)
	{

		//reset all event flag
		for (i = 1; i < num_fds; i++)
		{
			my_fds[i].events = POLLIN | POLLPRI;
			logging_info("%i: fd %i\n", i, my_fds[i].fd);
			my_fds[i].revents = 0;
			send(my_fds[i].fd, "Enter some text:\n", 18, 0);
		}

		//put all this into poll and wait for something magical to happen
		printf("calling poll (%d sockets)\n", num_fds);
		if (poll(my_fds, num_fds, TIMEOUT) == -1)
		{
			perror("poll");
			exit(0);
		}

		printf("poll returned!\n");

		//First item is the accepting socket....check it independently of the rest!
		if (my_fds[0].revents != 0)
		{
			printf("We have a new connection.\nAccept goes here...\n");

			//Accept the connection
			sin_size = sizeof their_addr;
			my_fds[num_fds].fd = accept(my_fds[0].fd, (struct sockaddr *)&their_addr, &sin_size);
			my_fds[num_fds].events = POLLIN;
			my_fds[num_fds].revents = 0;

			printf("Connection from %s\n", inet_ntoa(their_addr.sin_addr));
			sprintf(buff, "Your %i\n", num_fds);
			send(my_fds[num_fds].fd, buff, 7, 0);

			num_fds++;

		}
		else
		{
			//skip first one, we know that's the accepting socket (handled above).
			for (i = 1; i < num_fds; i++)
			{
				if (my_fds[i].revents != 0)
				{
					buff_sz = recv(my_fds[i].fd, &buff, 254, 0);
					buff[buff_sz] = '\0';
					logging_debug("Recieved: %s", buff);

					//send the message to everyone else
					for (j = 1; j < num_fds; j++)
					{
						printf("i = %i, j = %i\n", i, j);
						if (j != i)
						{
							sprintf(buff2, "%i sent you %i: %s", i, j, buff);
							send(my_fds[j].fd, buff2, strlen(buff2) + 1, 0);
						}
					}
				}
			}
		}
	}

	logging_shutdown();
	printf("Application ended\n");
	return 0;
}

static int listen_socket()
{
    struct sockaddr_in a;
    int s;
    int yes;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
            (char *) &yes, sizeof(yes)) < 0) {
        perror("setsockopt");
        close(s);
        return -1;
    }
    memset(&a, 0, sizeof(a));
    a.sin_port = htons(PORT);
    a.sin_family = AF_INET;
    if (bind(s, (struct sockaddr *) &a, sizeof(a)) < 0) {
        perror("bind");
        close(s);
        return -1;
    }
    printf("Accepting connections on port %d\n", PORT);
    listen(s, 10);
    return s;
}
