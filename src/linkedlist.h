
#ifndef linkedlist_h
#define linkedlist_h

typedef struct linkedlist_node_
{
	void *data;
	struct linkedlist_node_ *next;
} linkedlist_node;

typedef struct linkedlist_
{
	linkedlist_node *head;
} linkedlist;

linkedlist *linkedlist_new();

void linkedlist_add_first(linkedlist *list, void *data);
void linkedlist_add_last(linkedlist *list, void *data);

void *linkedlist_remove_first(linkedlist *list);
void *linkedlist_remove_last(linkedlist *list);
void *linkedlist_remove_item(linkedlist *list, void *data);

void *linkedlist_get_first(linkedlist *list);
void *linkedlist_get_last(linkedlist *list);

int linkedlist_count(linkedlist *list);
void linkedlist_clear(linkedlist *list);

void linkedlist_foreach(linkedlist *list, void (*f)(void*));

#endif
