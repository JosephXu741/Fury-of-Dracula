#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Trap.h"
#include "Queue.h"

typedef struct QueueNode {
	PlaceId location;
	TrapId traptype;
	struct QueueNode *next;
} QueueNode;

typedef struct QueueRep {
	int trapNum;
	QueueNode *head;  // ptr to first node
	QueueNode *tail;  // ptr to last node
} QueueRep;

// create new empty Queue
Queue newTrail()
{
	Queue q;
	q = malloc(sizeof(QueueRep));
	assert(q != NULL);
	q->head = NULL;
	q->tail = NULL;
	return q;
}

PlaceId GetVampireLocation (Queue Q)
{
	QueueNode *curr = Q->head;
	while (curr != NULL) {
		if (curr->traptype == IMMATURE_VAMPIRE){
			return curr->location;
		}
		curr = curr->next;
	}
	return NOWHERE;
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
PlaceId *getTrapsLocations(Queue Q)
{
	QueueNode *curr;
	assert(Q != NULL);
	
	PlaceId *arr = calloc(Q->trapNum, sizeof(int));
	curr = Q->head;
	int i = 0;
	while (curr != NULL) {
		if (curr->traptype == NORMAL_TRAP){
			arr[i] = curr->location;
			i++;
		}
		curr = curr->next;
	}
	return arr;
}

// add item at end of Queue 
void TrailJoin(Queue Q, TrapId traptype, PlaceId location)
{
	assert(Q != NULL);
	QueueNode *new = malloc(sizeof(QueueNode));
	assert(new != NULL);
	new->location = location;
	new->traptype = traptype;
	//if (SearchTrail(Q, it)) new->trapNum++;
	new->next = NULL;
	if (Q->head == NULL)
		Q->head = new;
	if (Q->tail != NULL)
		Q->tail->next = new;
	Q->tail = new;

	if (traptype == NORMAL_TRAP) {
		Q->trapNum++;
	}
}

// remove item from front of Queue
TrapId TrailLeave(Queue Q)
{
	assert(Q != NULL);
	assert(Q->head != NULL);
	TrapId it = Q->head->traptype;
	QueueNode *old = Q->head;
	Q->head = old->next;
	if (it == NORMAL_TRAP){
		Q->trapNum--;
	}
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
// searches a trail via a key (placeid) and returns the trap it
TrapId TrapRemove(Queue Q, PlaceId location) 
{ 
	assert(Q != NULL);
	QueueNode *curr = Q->head;
	while (curr != NULL) {
		if (curr->location == location){
			if (curr->traptype == NORMAL_TRAP) {
				Q->trapNum--;
			}
			return curr->traptype;
		} else {
			curr = curr->next;
		}	
	}
	return NO_TRAP; // location not found
}

PlaceId getDBTrailPosition(Queue Q, int backtrackNUM) { 	// gets the location that dracula wants to DB to in trail
	QueueNode *curr = Q->head;
	int headtotail = 6 - backtrackNUM;
	for (int i = 1; i < headtotail; i++) {
		curr = curr->next;
	}
	return curr->location;
}

int TrailLength(Queue Q){
	int length = 0;
	for (QueueNode* curr = Q->head; curr != NULL; length++) 
	curr = curr->next;
	return length;
}
int TotalTrapsTrail(Queue Q){
	return Q->trapNum;
}
