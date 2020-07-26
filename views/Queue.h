#ifndef QUEUE_H
#define QUEUE_H

#include "Trap.h"

typedef struct QueueRep *Queue;
typedef Queue Trail;



Queue newTrail(); // create new empty queue
PlaceId GetVampireLocation (Queue Q);
void dropTrail(Queue Q); // free memory used by queue
PlaceId *getTrapsLocations(Queue Q); // returns array of locations where a trap exists
void TrailJoin(Queue Q, TrapId traptype, PlaceId location); // add trap on trail
TrapId TrailLeave(Queue Q); // remove trap from trail
TrapId TrapRemove(Queue Q, PlaceId location); // looks if there is a trap in a certain location
int TrailIsEmpty(Queue); // check for no items
int TrailLength(Queue Q);
int TotalTrapsTrail(Queue Q);

#endif