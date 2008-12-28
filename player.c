
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
#include <stdarg.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "logging.h"
#include "card.h"
#include "player.h"

player *player_new()
{
	player *p = (player*) malloc (sizeof (player));

	p->name = NULL;
	p->password = NULL;
	p->connection = 0;

	return p;
}

void player_free(player *p)
{
	free(p);
}

void player_send(player *p, char *message, ...)
{
	char buff[255];

	va_list ap;
	va_start(ap, message);
	vsnprintf(buff, 255, message, ap);
	va_end(ap);

	send(p->connection, buff, strlen(buff) + 1, 0);
}

void player_broadcast(player **players, int player_count, char *message, ...)
{
	char buff[255];
	int i;
	player *p;
	va_list ap;

	va_start(ap, message);
	vsnprintf(buff, 255, message, ap);
	va_end(ap);

	logging_debug("player broadcast %i, player_count, %s", player_count, buff);

	for (i = 0; i < player_count; i++)
	{
		p = players[i];
		player_send(p, buff);
	}
}

int player_recv(player *p, char **message)
{
	*message = malloc(sizeof(char) * 255);
	recv(p->connection, *message, 254, 0);

	return 1;
}

