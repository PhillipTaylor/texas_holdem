
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

#include "linkedlist.h"

int linkedlist_count(linkedlist *l)
{
	int count = 0;
	linkedlist *iter = l;

	while (iter != NULL)
	{
		count++;
		iter = iter->next;
	}

	return count;
}

void linkedlist_add(linkedlist **l, void *item)
{
	linkedlist *iter = *l;

	if (l != NULL)
	{
		while (iter->next != NULL)
			iter = iter->next;

		iter->next = (linkedlist*) malloc (sizeof(linkedlist));
		iter->next->data = item;
		iter->next->next = NULL;
	}
	else
	{
		*l = (linkedlist*) malloc (sizeof (linkedlist));
		(*l)->data = item;
		(*l)->next = NULL;
	}
}

void linkedlist_remove(linkedlist **l, void *item)
{
	linkedlist *leading = NULL;
	linkedlist *iter = *l;
	linkedlist *found = NULL;

	while (iter != NULL)
	{
		if (iter->data == item)
		{
			found = iter;

			if (leading == NULL)
				*l = iter->next; //very first record. so move the calling functions pointer forward.
			else
				leading->next = iter->next; //skip

			//clean up the linkedlist record. we don't clean up the item pointed to itself. That's not our policy!
			free(found);
		}

		leading = iter;
		iter = iter->next;
	}
}

void linkedlist_circular_move_next(linkedlist *head,linkedlist **iterator)
{

	if (iterator == NULL)
		*iterator = head;

	if (head == NULL)
		return;

	if (((linkedlist*)iterator)->next == NULL)
		*iterator = head;
	else
		*iterator = ((linkedlist*)iterator)->next;
}
