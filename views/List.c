#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"
#include "Trap.h"

typedef struct ListNode {
	PlaceId location;
	trapType trap;

	struct ListNode *next;
} ListNode;

typedef struct ListRep {
	ListNode *first;  // ptr to first node
	ListNode *last;   // ptr to last node
} ListRep;

#define eq(a,b) ItemEQ(a,b)
#define gt(a,b) ItemGT(a,b)
#define key(a)  ItemKey(a)

// create new empty list
List newTrapList()
{
	List L;
	L = malloc(sizeof(ListRep));
	assert(L != NULL);
	L->first = NULL;
	L->last = NULL;
	return L;
}

// free memory used by list
void dropTrapList(List L)
{
	assert(L != NULL);
	ListNode *next;
	while (L->first != NULL) {
		next = L->first->next;
		ItemDrop(L->first->location);
		free(L->first);
		L->first = next;
	}
	free(L);
}

// display as [1,2,3,4...]
void showTrapList(List L)
{
	assert(L != NULL);
	ListNode *curr = L->first;
	printf("[");
	while (curr != NULL) {
		ItemShow(curr->location);
		if (curr->next != NULL)
			printf(",");
		curr = curr->next;
	}
	printf("]");
}

// add item into list
// no check for duplicates
void TrapListInsert(List L, Item it)
{
	assert(L != NULL);
	ListNode *prev, *curr;
	prev = NULL; curr = L->first;
	while (curr != NULL) {
		if (eq(key(it),key(curr->location)))
			return; // already in list
		if (gt(key(curr->location),key(it)))
            break;
		prev = curr;
		curr = curr->next;
	}
	ListNode *new = malloc(sizeof(ListNode));
	assert(new != NULL);
	new->location = ItemCopy(it);
	new->next = NULL;
	if (L->last == NULL)
		L->first = L->last = new;
	else {
        if (prev == NULL) {      // insert at front
            new->next = L->first;
            L->first = new;
        }
        else if (curr == NULL) { // insert at end
			L->last->next = new;
			L->last = new;
		}
		else {                  // insert in middle
			new->next = prev->next;
			prev->next = new;
		}
	}
}

// remove item(s)
// assumes no duplicates
void TrapListDelete(List L, Key k)
{
	assert(L != NULL);
	ListNode *prev, *curr;
	prev = NULL; curr = L->first;
	while (curr != NULL) {
		if (eq(k,key(curr->location)))
			break;
		prev = curr;
		curr = curr->next;
	}
	if (curr != NULL) {
		if (prev == NULL)
			L->first = curr->next;
		else
			prev->next = curr->next;
		
		if (curr == L->last)
			L->last = prev;
		
		free(curr);
		if (L->first == NULL)
			L->last = NULL;
	}
}

// return item with key
PlaceId *TrapListSearch(List L, Key k)
{
	
	assert(L != NULL);
	ListNode *curr = L->first;
	while (curr != NULL) {
		if (eq(k,key(curr->location)))
			return &(curr->location);
		else
			curr = curr->next;
	}
	return NULL; // key not found
}

// returns dynamically allocated array containing locations with traps
PlaceId *TrapLocations(List L, Key k)
{
	assert(L != NULL);
	PlaceId *arr = calloc(TrapListLength(L), sizeof(int));
	int i = 0;
	ListNode *curr = L->first;
	while (curr != NULL) {
		if (k == 0) {
			arr[i] = (curr->location);
		}
		else if (eq(k,key(curr->location))) {
			arr[i] = (curr->location);
		}
		curr = curr->next;
		i++;
	}
	return realloc(arr, i*sizeof(int));
}

// # items in list
int TrapListLength(List L)
{
	int n = 0;
	ListNode *curr = L->first; 
	while (curr != NULL) {
		n++;
		curr = curr->next;
	}
	return n;
}
