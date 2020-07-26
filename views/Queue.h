#ifndef QUEUE_H
#define QUEUE_H

#include "Trap.h"

typedef struct QueueRep *Queue;
typedef Queue Trail;

Queue newTrail(); // create new empty queue
PlaceId* SearchTrail(Queue Q, Key k);
void dropTrail(Queue); // free memory used by queue
PlaceId *getTrailLocation(Queue Q); // display as 3 > 5 > 4 > ...
void TrailJoin(Queue,Item); // add item on queue
PlaceId TrailLeave(Queue); // remove item from queue
int TrailIsEmpty(Queue); // check for no items
int TrailLength(Queue Q);
int TotalTrapsTrail(Queue Q);

#endif