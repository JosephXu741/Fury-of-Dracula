
#define QUEUE_H

#include "Trap.h"

typedef struct QueueRep *Queue;
typedef Queue Trail;



Queue newTrail(); // create new empty queue
PlaceId GetVampireLocation (Queue Q);
void dropTrail(Queue Q); // free memory used by queue
PlaceId *getTrapsLocations(Queue Q); // returns array of locations where a trap exists
void TrailJoin(Queue Q, PlaceId location); // add trail
void addTrapToTrail(Queue Q, PlaceId location, TrapId traptype); // add trap to trail
void UpdateTrapSameLocation(Queue Q, PlaceId location); // updates the trapnums if the same location exists > 1
TrapId TrailLeave(Queue Q); // remove oldest trail and returns trapId
TrapId TrapRemove(Queue Q, PlaceId location, TrapId traptype); // looks if there is a trap in a certain location
PlaceId getDBTrailPosition(Queue Q, int backtrackNUM); // gets the location that dracula wants to DB to from trail
int TrailIsEmpty(Queue); // check for no items
int TrailLength(Queue Q);
int TotalTrapsTrail(Queue Q);

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//Original queue ADTs
/////////////////////////////////////////////////////////////////////
// Queue.c ... implementation of Queue ADT
// Written by John Shepherd, March 2013

#define QUEUE_ORIGINAL_H


typedef struct QueueOrgRep *QueueOrg;
typedef int Item;

#define ItemCopy(i) (i)
#define ItemEQ(i1, i2) ((i1) == (i2))
#define ItemShow(i) printf ("%d", (i))


QueueOrg newQueueOrg (void);			// create new empty QueueOrg
void dropQueueOrg (QueueOrg);			// free memory used by QueueOrg
void showQueueOrg (QueueOrg);			// display as 3 > 5 > 4 > ...
void QueueOrgJoin (QueueOrg, Item);	// add item on QueueOrg
Item QueueOrgLeave (QueueOrg);		// remove item from QueueOrg
int QueueOrgIsEmpty (QueueOrg);		// check for no items








