
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

#include "table.h"

table *table_new()
{
	linkedlist *iterator = NULL;
	table *tbl = (table*) malloc (sizeof(table));

	tbl->player_head = players;

	if (players == NULL)
		return NULL;

	//get tail now. we allow the turns to be flipped better later.
	iterator = tbl->player_head;

	while (iterator->next != NULL)
		iterator = iterator->next;
	
	tbl->player_tail = iterator;

	config_get_int("small_blind", &tbl->smallBlind);
	tbl->flop1 = NULL;
	tbl->flop2 = NULL;
	tbl->flop3 = NULL;
	tbl->turn = NULL;
	tbl->river = NULL;

	return tbl;
}

void table_free(table *t)
{
	free(t);
}

void table_clear_cards(table *t)
{
    t->flop1 = NULL;
    t->flop2 = NULL;
    t->flop3 = NULL;
    t->turn = NULL;
    t->river = NULL;
}; 
