
#include "linkedlist.h"

#include <stddef.h>
#include <stdlib.h>

linkedlist *linkedlist_new()
{
	linkedlist *new_item = (linkedlist*) malloc (sizeof(linkedlist));

	new_item->head = NULL;

	return new_item;
}

void *linkedlist_add_first(linkedlist *list, void *data)
{
	linkedlist_node *first = (linkedlist_node*) malloc (sizeof(linkedlist_node));

	first->data = data;
	first->next = list->head;

	list->head = first;

}

void *linkedlist_add_last(linkedlist *list, void *data)
{

	linkedlist_node *item = (linkedlist_node*) malloc (sizeof(linkedlist_node));

	item->data = data;
	item->next = NULL;

	linkedlist_node *iter = list->head;

	if (iter == NULL)
		list->head = item;
	else
	{
		while (iter->next != NULL)
			iter = iter->next;

		iter->next = item;
	}

}

void *linkedlist_get_first(linkedlist *list)
{
	if (list->head == NULL)
		return NULL;
	else
		return list->head->data;
}

void *linkedlist_get_last(linkedlist *list)
{

	linkedlist_node *node = list->head;

	if (node == NULL)
		return NULL;
	if (node->next == NULL)
		return node;

	while (node->next != NULL)
		node = node->next;

	return node->data;

}

void *linkedlist_remove_first(linkedlist *list)
{

	void *item = NULL;
	linkedlist_node *node = list->head;

	if (node == NULL)
		return;
	else
	{
		item = node->data;

		list->head = node->next;
		free(node);
		return item;
	}

}

void *linkedlist_remove_last(linkedlist *list)
{

	//iterator shoots to the last record
	linkedlist_node *iter = list->head;

	//keep the seond to last record so we can NULL it.
	linkedlist_node *second_to_last = NULL;

	//to be returned to user.
	void *item = NULL;

	if (iter == NULL)
		return NULL;

	//only the head item
	if (iter->next == NULL)
	{
		item = list->head->data;

		free(list->head);
		list->head = NULL;

		return item;
	}
	else
		second_to_last = list->head;

	while (iter->next != NULL)
	{
		second_to_last = iter;
		iter = iter->next;
	}

	item = iter->data;

	free(iter);
	second_to_last->next = NULL;
	return item;
}

int linkedlist_count(linkedlist *list)
{
	int i = 0;
	linkedlist_node *iter = list->head;

	if (iter == NULL)
		return 0;
	else
		i = 1;

	while (iter->next != NULL)
	{
		iter = iter->next;
		i++;
	}

	return i;
}

void linkedlist_clear(linkedlist *list)
{

	linkedlist_node *n = list->head;
	linkedlist_node *r = n;

	if (n != NULL)
	{
		while (n != NULL)
		{
			r = n;
			n = n->next;
			free(r);
		}
	}

	list->head = NULL;
}

void linkedlist_foreach(linkedlist *list, void (*f)(void*))
{
	linkedlist_node *node = list->head;

	while (node != NULL)
	{
		f(node->data);
		node = node->next;
	}
}

