#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Trap.h"
#include "Queue.h"

typedef struct QueueNode {
	PlaceId location;
	bool vampire;
	int trapNum;
	struct QueueNode *next;
} QueueNode;

typedef struct QueueRep {
	QueueNode *head;  // ptr to first node
	QueueNode *tail;  // ptr to last node
} QueueRep;

// create new empty Queue
Trail newTrail()
{
	Queue q;
	q = malloc(sizeof(QueueRep));
	assert(q != NULL);
	q->head = NULL;
	q->tail = NULL;
	return q;
}

// free memory used by Queue
void dropTrail(Queue Q)
{
	QueueNode *curr, *next;
	assert(Q != NULL);
	// free list nodes
	curr = Q->head;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	// free queue rep
	free(Q);
}

// returns dynamic array of the locations containing traps/trail
PlaceId *getTrailLocation(Queue Q)
{
	QueueNode *curr;
	assert(Q != NULL);
	
	PlaceId *arr = calloc(TRAIL_SIZE, sizeof(int));
	int i = 0;
	curr = Q->head;
	while (curr != NULL) {
		arr[i] = (curr->location);
		curr = curr->next;
		i++;
	}
	return realloc(arr, i*sizeof(PlaceId));
}

// add item at end of Queue 
void TrailJoin(Queue Q, PlaceId it)
{
	assert(Q != NULL);
	QueueNode *new = malloc(sizeof(QueueNode));
	assert(new != NULL);
	new->location = ItemCopy(it);
	new->trapNum = 1;
	if (SearchTrail(Q, it)) new->trapNum++;
	new->vampire = false;
	new->next = NULL;
	if (Q->head == NULL)
		Q->head = new;
	if (Q->tail != NULL)
		Q->tail->next = new;
	Q->tail = new;
}

// remove item from front of Queue
PlaceId TrailLeave(Queue Q)
{
	assert(Q != NULL);
	assert(Q->head != NULL);
	PlaceId it = ItemCopy(Q->head->location);
	QueueNode *old = Q->head;
	Q->head = old->next;
	if (Q->head == NULL)
		Q->tail = NULL;
	free(old);
	return it;
}

// check for no items
int TrailIsEmpty(Queue Q)
{
	return (Q->head == NULL);
}
// searches a trail via a key (placeid) and returns it
PlaceId* SearchTrail(Queue Q, Key k) { 
	assert(Q != NULL);
	QueueNode *curr = Q->head;
	while (curr != NULL) {
		if (eq(k,key(curr->location)))
			return &(curr->location);
		else
			curr = curr->next;
	}
	return NULL; // key not found
}
