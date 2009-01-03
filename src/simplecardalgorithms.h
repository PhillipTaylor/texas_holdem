
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

#ifndef SIMPLE_CARD_ALGORITHMS_H
#define SIMPLE_CARD_ALGORITHMS_H

#include "hand.h"

/* 
 * These functions probably do the most work to identify player
 * hands and to score likewise hands against each other.
*/

enum enumSuit
{
	DIAMONDS = 'd',
	HEARTS = 'h',
	CLUBS = 'c',
	SPADES = 's',
	UNSET = 'x'
};

enum enumValue 
{
	NO_MATCH = -1,
	TWO = 2,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	ACE
};

int get_value_of_straight(hand* h);
int contains_value_of_suit(hand* h, int value, char suit);
char get_flush_suit(hand* h);
int get_value_of_four_of_a_kind(hand* h);
int get_value_of_three_of_a_kind(hand* h);
int get_value_of_highest_pair(hand* h);
int get_value_of_second_highest_pair(hand* h);
int get_value_of_highest_card(hand* h);

#endif
