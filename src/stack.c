
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

#include <stdio.h>

#include "stack.h"

stack *stack_new()
{
	stack *s = (stack*) malloc (sizeof(stack));

	s->node = NULL;

	return s;
}

void stack_push(stack *s, void *item)
{
	stack_node *node = (stack_node*) malloc (sizeof(stack_node));

	node->item = item;
	node->next = s->node;

	s->node = node;
}

void *stack_pop(stack *s)
{
	if (s->node == NULL)
		return NULL;

	stack_node *node = s->node;
	void *item = node->item;
	s->node = node->next;

	free(node);

	return item;
}

int stack_count(stack *s)
{
	int i = 0;
	stack_node *node = s->node;

	while (node != NULL)
	{
		node = node->next;
		i++;
	}

	return i;
}

bool stack_empty(stack *s)
{
	if (s->node == NULL)
		return true;
	else
		return false;
}

