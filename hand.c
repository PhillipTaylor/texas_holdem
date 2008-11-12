
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

#include "player.h"
#include "table.h"
#include "hand.h"

hand* hand_new()
{
	int i;
	
	hand *h = (hand*) malloc (sizeof(hand));

	for (i = 0; i < CARDS_IN_HAND; i++)
		h->c[i] = NULL;
	
	return h;
}

void hand_free(hand *h)
{
	free(h);
}

hand* convert_to_hand(player *p, table *t)
{
    //construct new hand.
    hand *h = (hand*)malloc(sizeof(hand));
    
    h->c[0] = p->cardOne;
    h->c[1] = p->cardTwo;
    h->c[2] = t->flop1;
    h->c[3] = t->flop2;
    h->c[4] = t->flop3;
    h->c[5] = t->turn;
    h->c[6] = t->river;
    
    return h;
}    
