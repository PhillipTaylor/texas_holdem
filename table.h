
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

#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <stddef.h>

#include "card.h"
#include "player.h"
#include "linkedlist.h"

typedef struct
{
    int smallBlind;

    linkedlist *player_head;
    linkedlist *player_tail;

    card *flop1;
    card *flop2;
    card *flop3;
    card *turn;
    card *river;

} table;

//Table's are affiliated with forks and run independendly from the main branch.

table *table_new();
void table_free(table *t);
void table_clear_cards(table *t);

#endif
