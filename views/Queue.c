#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Trap.h"
#include "Queue.h"

typedef struct QueueNode {
	PlaceId location;
	int trapNums;
	bool vampire;
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
	q->trapNum = 0;
	return q;
}

PlaceId GetVampireLocation (Queue Q)
{
	QueueNode *curr = Q->head;
	while (curr != NULL) {
		if (curr->vampire == true){
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
	
	PlaceId *arr = calloc(Q->trapNum, sizeof(PlaceId));
	curr = Q->head;
	int i = 0;
	
	while (curr != NULL) {
		if (curr->vampire == false && curr->trapNums > 0){
			arr[i] = curr->location;
			i++;
		}
		curr = curr->next;
	}
	return arr;
}

// add item at end of Queue 
void TrailJoin(Queue Q, PlaceId location)
{
	assert(Q != NULL);
	QueueNode *new = malloc(sizeof(QueueNode));
	assert(new != NULL);
	new->location = location;
	new->trapNums = 0;
	new->vampire = false;

	//if (SearchTrail(Q, it)) new->trapNum++;
	new->next = NULL;
	if (Q->head == NULL)
		Q->head = new;
	if (Q->tail != NULL)
		Q->tail->next = new;		
	Q->tail = new;
	
}
// adds traps to trail
void addTrapToTrail(Queue Q, PlaceId location, TrapId traptype) {
	QueueNode *curr = Q->head;
	while (curr != NULL) {
		if (curr->location == location) {
			if (traptype == NORMAL_TRAP) {
				curr->trapNums++;
				UpdateTrapSameLocation(Q, location);
			}
			if (traptype == IMMATURE_VAMPIRE) curr->vampire = true;
			break;
		}
		curr = curr->next;
	}
	if (curr->trapNums > 0) Q->trapNum ++;
}

// remove item from front of Queue
TrapId TrailLeave(Queue Q)
{
	assert(Q != NULL);
	assert(Q->head != NULL);
	TrapId it = 0;
	if (Q->head->trapNums > 0) it = NORMAL_TRAP;
	if (Q->head->vampire == true) it = IMMATURE_VAMPIRE;
	
	QueueNode *old = Q->head;
	Q->head = old->next;
	if (it == NORMAL_TRAP){
		Q->trapNum--;
	}
	if (Q->head == NULL)
		Q->tail = NULL;
	free(old);
	if (Q->head->vampire == true) it = IMMATURE_VAMPIRE;
	return it;
}

// check for no items
int TrailIsEmpty(Queue Q)
{
	return (Q->head == NULL);
}
// searches a trail via a key (placeid) and returns the trap it
TrapId TrapRemove(Queue Q, PlaceId location, TrapId traptype) 
{ 
	assert(Q != NULL);
	QueueNode *curr = Q->head;
	while (curr != NULL) {
		if (curr->location == location){
			if (traptype == NORMAL_TRAP && curr->trapNums > 0) {
				
				Q->trapNum -= curr->trapNums;
				curr->trapNums--;
				UpdateTrapSameLocation(Q, location);
				return NORMAL_TRAP;
			}
			if (traptype == IMMATURE_VAMPIRE && curr->vampire == true) {
				curr->vampire = false;
				return IMMATURE_VAMPIRE;
			}
			
		} else {
			curr = curr->next;
		}	
	}
	return NO_TRAP; // location not found
}

PlaceId getDBTrailPosition(Queue Q, int backtrackNUM) { 	// gets the location that dracula wants to DB to in trail
	QueueNode *curr = Q->head;
	int headtotail = TrailLength(Q) + 1 - backtrackNUM;
	
	int i = 1;
	
	while (i < headtotail && curr->next != NULL) {
		curr = curr->next;
		i++;
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
// checks for the max num of traps in location if there are multiple same locations in trail
void UpdateTrapSameLocation(Queue Q, PlaceId location) { 
	int firstoccurencetraps = 0;
	bool first = false;
	QueueNode *curr = Q->head;
	while (curr != NULL) {
		if (curr->location == location) {
			if (first == false) {
				first = true;
				firstoccurencetraps = curr->trapNums;
			}
			curr->trapNums = firstoccurencetraps;
		}

		curr = curr->next;
	}

}
