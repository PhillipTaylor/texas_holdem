
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


#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct stack_node_
{
	void *item;
	struct stack_node_ *next;
} stack_node;

typedef struct stack_
{
	stack_node *node;
} stack;

stack *stack_new();

void stack_push(stack *s, void *item);
void *stack_pop(stack *s);

int stack_count(stack *s);
bool stack_empty(stack *s);


#endif
