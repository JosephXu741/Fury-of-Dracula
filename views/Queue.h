#ifndef QUEUE_H
#define QUEUE_H

#include "Trap.h"

typedef struct QueueRep *Queue;
typedef Queue Trail;

Queue newTrail(); // create new empty queue
void dropTrail(Queue); // free memory used by queue
void showTrail(Queue); // display as 3 > 5 > 4 > ...
void TrailJoin(Queue,Item); // add item on queue
Item TrailLeave(Queue); // remove item from queue
int TrailIsEmpty(Queue); // check for no items

#endif