
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

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct linkedlist_
{
	void *data;
	struct linkedlist_ *next;
} linkedlist;

/*
 *  There is no function to instantiate a linkedlist. To instantiate one
 *  Merely set the pointer to NULL.
 *
 *  i.e      linkedlist *myList = NULL;
 *
 *  If you don't set the value to NULL explicitly you may get problems.
*/

/*
 * Add an item to the end of a linkedlist. You may provide any node within the list you wish to add to.
*/
void linkedlist_add(linkedlist **l, void *item);

/*
 * This removes an element from the list.
 * The behaviour is to explicitly NOT call free on the matching item.
 * If a list contains multiple duplicate items, all instances are removed.
*/
void linkedlist_remove(linkedlist **l, void *item);

int linkedlist_count(linkedlist *l);

/*
 * This function performs an increment to the next item and when your at the end of the list
 * it will wrap around again to the front. Remember that your loop cannot be "while iter != NULL"
 * because this claus will never occur. The iterator is edited in place so you should probably use
 * a loop like this:
 *
 * linkedlist *head
 * linkedlist *startedAt;
 * linkedlist *iterator;
 *
 * while (iterator != startedAt)
 * {
 * 	...
 *	linkedlist_circular_move_next(head,iterator);
 * }
 *
 * for one complete loop.
*/
void linkedlist_circular_move_next(linkedlist *head,linkedlist **iterator);

#endif
