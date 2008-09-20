
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

#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include "card.h"
#include "player.h"

typedef struct
{
    char *name;
    int cash;
    int contribToPot;
    bool folded;
    bool allIn;
    card *cardOne;
    card *cardTwo;
} player;

player *player_new(char *name, int initialCash);
void player_free(player *p);

/*this message should be transmitted in a way which is easily parsable
 * rather than a straight question to the user */
void player_message(player *p, char* message);

/*
 * responce can either be a char such as  'f' for fold, 'c' for check or 'r' for raise,
 * 'y' for yes, or 'n' for no depending on the question or it can be an number such
 * as how much they want to raise or bet.
 */
int player_question(player *p);

#endif
