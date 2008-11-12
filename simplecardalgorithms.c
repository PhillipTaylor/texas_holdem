
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

#include "card.h"
#include "hand.h"
#include "simplecardalgorithms.h"

int get_value_of_straight(hand* h)
{
    int values[15];
    int i = 0;
    
    //set all values to zero.
    for (i = 0; i < 15; i++)
        values[i] = 0;
    
    //count all the cards. values[2] = 2, values[11] = jack etc. count how many!
    for (i = 0; i < 7; i++)
    {
        values[h->c[i]->value]++;
    }
    
    if (values[10] > 0 && values[11] > 0 && values[12] > 0 && values[13] > 0 && values[14] > 0) return 14; //Straight, Ace High.
    if (values[9] > 0 && values[10] > 0 && values[11] > 0 && values[12] > 0 && values[13] > 0) return 13;
    if (values[8] > 0 && values[9] > 0 && values[10] > 0 && values[11] > 0 && values[12] > 0) return 12;
    if (values[7] > 0 && values[8] > 0 && values[9] > 0 && values[10] > 0 && values[11] > 0) return 11;
    if (values[6] > 0 && values[7] > 0 && values[8] > 0 && values[9] > 0 && values[10] > 0) return 10;
    if (values[5] > 0 && values[6] > 0 && values[7] > 0 && values[8] > 0 && values[9] > 0) return 9;
    if (values[4] > 0 && values[5] > 0 && values[6] > 0 && values[7] > 0 && values[8] > 0) return 8;
    if (values[3] > 0 && values[4] > 0 && values[5] > 0 && values[6] > 0 && values[7] > 0) return 7;
    if (values[2] > 0 && values[3] > 0 && values[4] > 0 && values[5] > 0 && values[6] > 0) return 6;
    if (values[14] > 0 && values[2] > 0 && values[3] > 0 && values[4] > 0 && values[5] > 0) return 5; //Straight, 5 High 
    
    return -1; //No Straight!
    
}

int contains_value_of_suit(hand* h, int value, char suit) //1 equals true. 0 equals false.
{
    int i = 0;
    for (i = 0; i < 7; i++)
    {
        if (h->c[i]->value == value && h->c[i]->suit == suit) 
                return 1;
    }
    
    return 0;
} 

char get_flush_suit(hand* h)
{
    int diamonds = 0, spades = 0, clubs = 0, hearts = 0;
    int i;
    
    for (i = 0; i < 7; i++)
    {
        if (h->c[i]->suit == 'd') diamonds++;
        if (h->c[i]->suit == 's') spades++;
        if (h->c[i]->suit == 'c') clubs++;
        if (h->c[i]->suit == 'h') hearts++;
    }
    
    if (diamonds > 4)
        return 'd';
    
    if (spades > 4)
        return 's';
    
    if (clubs > 4)
        return 'c';
    
    if (hearts > 4)
        return 'h';
    
    //No flush.
    return 'x';
}   

int get_value_of_four_of_a_kind(hand* h)
{
    int values[15];
    int i = 0;
    
    //set all values to zero.
    for (i = 0; i < 15; i++)
        values[i] = 0;
    
    //count all the cards. values[2] = 2, values[11] = jack etc. count how many!
    for (i = 0; i < 7; i++)
    {
        values[h->c[i]->value]++;
    }
    
    if (values[14] == 4) return 14;   
    if (values[13] == 4) return 13;
    if (values[12] == 4) return 12;
    if (values[11] == 4) return 11;
    if (values[10] == 4) return 10;
    if (values[9] == 4) return 9;
    if (values[8] == 4) return 8;
    if (values[7] == 4) return 7;
    if (values[6] == 4) return 6;
    if (values[5] == 4) return 5;
    if (values[4] == 4) return 4;
    if (values[3] == 4) return 3;
    if (values[2] == 4) return 2;
    
    return -1; //No Four of a Kind
}

int get_value_of_three_of_a_kind(hand* h)
{
    int values[15];
    int i = 0;
    
    //set all values to zero.
    for (i = 0; i < 15; i++)
        values[i] = 0;
    
    //count all the cards. values[2] = 2, values[11] = jack etc. count how many!
    for (i = 0; i < 7; i++)
    {
        values[h->c[i]->value]++;
    }
    
    if (values[14] == 3) return 14;   
    if (values[13] == 3) return 13;
    if (values[12] == 3) return 12;
    if (values[11] == 3) return 11;
    if (values[10] == 3) return 10;
    if (values[9] == 3) return 9;
    if (values[8] == 3) return 8;
    if (values[7] == 3) return 7;
    if (values[6] == 3) return 6;
    if (values[5] == 3) return 5;
    if (values[4] == 3) return 4;
    if (values[3] == 3) return 3;
    if (values[2] == 3) return 2;
    
    return -1; //No Three of a Kind    
}      

int get_value_of_highest_pair(hand* h)
{
    int values[15];
    int i = 0;
    
    //set all values to zero.
    for (i = 0; i < 15; i++)
        values[i] = 0;
    
    //count all the cards. values[2] = 2, values[11] = jack etc. count how many!
    for (i = 0; i < 7; i++)
    {
        values[h->c[i]->value]++;
    }
    
    if (values[14] == 2) return 14;   
    if (values[13] == 2) return 13;
    if (values[12] == 2) return 12;
    if (values[11] == 2) return 11;
    if (values[10] == 2) return 10;
    if (values[9] == 2) return 9;
    if (values[8] == 2) return 8;
    if (values[7] == 2) return 7;
    if (values[6] == 2) return 6;
    if (values[5] == 2) return 5;
    if (values[4] == 2) return 4;
    if (values[3] == 2) return 3;
    if (values[2] == 2) return 2;
    
    return -1; //No Pairs at all.   
}   

int get_value_of_second_highest_pair(hand* h)
{
    int values[15];
    int i = 0;
    int skippedHighestYet = 0; //false.
    
    //set all values to zero.
    for (i = 0; i < 15; i++)
        values[i] = 0;
    
    //count all the cards. values[2] = 2, values[11] = jack etc. count how many!
    for (i = 0; i < 7; i++)
    {
        values[h->c[i]->value]++;
    }
    
    if (values[14] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 14;   
    }
        
    if (values[13] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 13;
    }
        
    if (values[12] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 12;
    }    
    
    if (values[11] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 11;
    }
        
    if (values[10] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 10;
    }
        
    if (values[9] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 9;
    }    
    
    if (values[8] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 8;
    }
    
    if (values[7] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 7;
    }   
     
    if (values[6] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 6;
    }
        
    if (values[5] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 5;
    }
        
    if (values[4] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 4;
    }   
     
    if (values[3] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 3;
    }   
     
    if (values[2] == 2)
    {
        if (skippedHighestYet == 0)
                skippedHighestYet = 1;
        else
                return 2;
    }    
    
    return -1; //No SECOND highest pair...user probably has a pair or nothing BUT NOT a two PAIR.
}    

int get_value_of_highest_card(hand* h)
{
    int currentHighest = -1;
    int i;
    
    for (i = 0; i < 7; i++)
    {
        if (h->c[i]->value > currentHighest)
                currentHighest = h->c[i]->value;
    }
    
    return currentHighest;
    
}

