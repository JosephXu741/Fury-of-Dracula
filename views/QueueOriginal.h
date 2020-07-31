// Queue.h ... interface to Queue ADT
// Written by John Shepherd, March 2013

//#ifndef QUEUE_ORIGINAL_H
#define QUEUE_ORIGINAL_H


typedef struct QueueRep *Queue;
typedef int Item;

#define ItemCopy(i) (i)
#define ItemEQ(i1, i2) ((i1) == (i2))
#define ItemShow(i) printf ("%d", (i))


Queue newQueue (void);			// create new empty queue
void dropQueue (Queue);			// free memory used by queue
void showQueue (Queue);			// display as 3 > 5 > 4 > ...
void QueueJoin (Queue, Item);	// add item on queue
Item QueueLeave (Queue);		// remove item from queue
int QueueIsEmpty (Queue);		// check for no items

//#endif
