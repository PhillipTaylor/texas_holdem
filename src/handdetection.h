
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

#ifndef HAND_DETECTION_H
#define HAND_DETECTION_H

#include <stdbool.h>

//lowest level functions to identify a hand

bool is_royal_flush(hand* h);
bool is_straight_flush(hand* h);
bool is_four_of_a_kind(hand* h);
bool is_full_house(hand* h);
bool is_flush(hand* h);
bool is_straight(hand* h);
bool is_three_of_a_kind(hand* h);
bool is_two_pair(hand* h);
bool is_pair(hand* h);

#endif
