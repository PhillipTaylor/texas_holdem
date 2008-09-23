
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
#include <stdbool.h>

#include "simplecardalgorithms.h"
#include "handdetection.h"

bool is_royal_flush(hand* h)
{
    char flushSuit = get_flush_suit(h);
    if (flushSuit == 'x') //Not a Flush
        return false;
    
    int straightValue = get_value_of_straight(h);
    if (straightValue != 14) //Not an Ace high straight.
        return false;
        
    //make sure all the straight cards are also of the flush suit.
    if (contains_value_of_suit(h,14,flushSuit) == 0) return 0;
    if (contains_value_of_suit(h,13,flushSuit) == 0) return 0;
    if (contains_value_of_suit(h,12,flushSuit) == 0) return 0;
    if (contains_value_of_suit(h,11,flushSuit) == 0) return 0;
    if (contains_value_of_suit(h,10,flushSuit) == 0) return 0;
    
    return true; //an ace high straight of all the same suit (aka: A Royal Flush!)
}   

bool is_straight_flush(hand* h)
{
    char flushSuit = get_flush_suit(h);
    if (flushSuit == 'x') //Not a Flush
        return false;
    
    int straightValue = get_value_of_straight(h);
    if (straightValue == -1) //Not a straight.
        return false;
        
    //make sure all the straight cards are also of the flush suit.
    if (contains_value_of_suit(h,straightValue,flushSuit) == 0) return false;
    if (contains_value_of_suit(h,(straightValue - 1),flushSuit) == 0) return false;
    if (contains_value_of_suit(h,(straightValue - 2),flushSuit) == 0) return false;
    if (contains_value_of_suit(h,(straightValue - 3),flushSuit) == 0) return false;
    if (straightValue > 5)
    {
        if (contains_value_of_suit(h,(straightValue - 4),flushSuit) == 0)
        {
            return false;
        }
    }
    else        //ace is low or high in straights.
    {
        if (contains_value_of_suit(h,14,flushSuit) == 0)
        {
            return false;
        }    
    }         
        
    return true;   
}

bool is_four_of_a_kind(hand* h)
{
    
    if (get_value_of_four_of_a_kind(h) == -1)
        return 0;
    else
        return 1;
}

bool is_full_house(hand* h)
{
    int value[15];
    int i;
    int threeFound = 0, twoFound = 0;
    
    if (!h) return false;

    //initialise the value
    for (i = 0; i < 15; i++)
        value[i] = 0;
    
    //Count the number of each value
    for (i = 0; i < 7; i++)
    {
        value[h->c[i]->value]++;
    }
    
    //find highest three and mark them as zero, but counted!
    for (i = 14; i > 1 && threeFound == 0; i--)
    {
        if (value[i] > 2)
        {
            //highest three of a kind found
            value[i] = 0; //reset.
            threeFound = 1;
        }
    }
    
    if (threeFound == 0)
        return false; //not a full house.
    
    for (i = 14; i > 1 && twoFound == 0; i--)
    {
        if (value[i] > 1) //pair of second three of a kind (but a lower value!)
        {
            value[i] = 0;
            twoFound = 1;
        }
    }
    
    if (twoFound == 0)
        return false;
    
    return true; //you have a full house!        
}
  
bool is_flush(hand* h)
{
	if (!h) return false;

	if (get_flush_suit(h) == 'x')
	        return false;
	else
		return true;
}

bool is_straight(hand* h)
{
	if (!h) return false;
	
	if (get_value_of_straight(h) == -1)
        	return false;
	else
		return true;
}

bool is_three_of_a_kind(hand* h)
{
	if (!h) return false;

	if (get_value_of_three_of_a_kind(h) == -1)
		return false;
	else
		return true;
} 

bool is_two_pair(hand* h)
{
	if (!h) return false;

	if ((get_value_of_highest_pair(h) != -1) && (get_value_of_second_highest_pair(h) != -1))
		return true;
	else
		return false;
}    

bool is_pair(hand* h)
{
	if (!h) return false;

	if (get_value_of_highest_pair(h) == -1)
        	return false;
	else
		return true;
}
                             
