
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

#ifndef HAND_H
#define HAND_H

#include "card.h"
#include "player.h"
#include "table.h"

/*
 * It's worth explicitly stating that a hand is a combination
 * of both the player's own 2 cards and the table cards. This
 * is used to derive the final winning hand, rather than to be
 * an interactive structure the player uses or holds.
 */

#define CARDS_IN_HAND 7

typedef struct
{
    card *c[CARDS_IN_HAND];
} hand;

hand* hand_new();
void hand_free(hand *h);

hand* convert_to_hand(player *p, table *t);

#endif
