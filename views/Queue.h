#ifndef QUEUE_H
#define QUEUE_H

#include "Trap.h"

typedef struct QueueRep *Queue;
typedef Queue Trail;

Queue newQueue(); // create new empty queue
void dropQueue(Queue); // free memory used by queue
void showQueue(Queue); // display as 3 > 5 > 4 > ...
void QueueJoin(Queue,Item); // add item on queue
Item QueueLeave(Queue); // remove item from queue
int QueueIsEmpty(Queue); // check for no items

#endif