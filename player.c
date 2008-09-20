
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
#include "player.h"

player *player_new(char *name, int initialCash)
{
	player *p = (player*) malloc (sizeof (player));

	//copy the name
	p->name = (char*) malloc ((sizeof(char) * strlen(name)) + 1);
	strcpy(p->name,name);

	p->cash = initialCash;
	p->contribToPot = 0;
	p->folded = false;
	p->allIn = false;
	p->cardOne = NULL;
	p->cardTwo = NULL;

	return p;
}

void player_free(player *p)
{
	free(p->name);
	free(p);
}

void player_message(player *p,char *message)
{

    printf("%s:\t%s",p->name,message);
}

int player_question(player *p)
{
	int answer = fgetc(stdin);
	fgetc(stdin);

	return answer;
}
