////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// Map.c: an implementation of a Map type
// You can change this as much as you want!
// If you modify this, you should submit the new version.
//
// 2017-11-30	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "Map.h"
#include "Places.h"
#include "Game.h"

#define HUNTER 100
#define DRACULA 200




struct map {
	int nV; // number of vertices
	int nE; // number of edges
	ConnList connections[NUM_REAL_PLACES];
};

static void addConnections(Map m);
static void addConnection(Map m, PlaceId v, PlaceId w, TransportType type);
static inline bool isSentinelEdge(Connection c);

static ConnList connListInsert(ConnList l, PlaceId v, TransportType type);
static bool connListContains(ConnList l, PlaceId v, TransportType type);


// add
int furtherRailMove (Map m, PlaceId p, int maxDist, PlaceId arr, int curr_i, int *numReturnedLocs);
int searchInReachable (PlaceId arr, PlaceId newPlace);


////////////////////////////////////////////////////////////////////////

/** Creates a new map. */
// #vertices always same as NUM_REAL_PLACES
Map MapNew(void)
{
	Map m = malloc(sizeof(*m));
	if (m == NULL) {
		fprintf(stderr, "Couldn't allocate Map!\n");
		exit(EXIT_FAILURE);
	}

	m->nV = NUM_REAL_PLACES;
	m->nE = 0;
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		m->connections[i] = NULL;
	}

	addConnections(m);
	return m;
}

/** Frees all memory allocated for the given map. */
void MapFree(Map m)
{
	assert (m != NULL);

	for (int i = 0; i < m->nV; i++) {
		ConnList curr = m->connections[i];
		while (curr != NULL) {
			ConnList next = curr->next;
			free(curr);
			curr = next;
		}
	}
	free(m);
}

////////////////////////////////////////////////////////////////////////

/** Prints a map to `stdout`. */
void MapShow(Map m)
{
	assert(m != NULL);

	printf("V = %d, E = %d\n", m->nV, m->nE);
	for (int i = 0; i < m->nV; i++) {
		for (ConnList curr = m->connections[i]; curr != NULL; curr = curr->next) {
			printf("%s connects to %s by %s\n",
			       placeIdToName((PlaceId) i),
			       placeIdToName(curr->p),
			       transportTypeToString(curr->type)
			);
		}
	}
}

////////////////////////////////////////////////////////////////////////

/** Gets the number of places in the map. */
int MapNumPlaces(Map m)
{
	assert(m != NULL);
	return m->nV;
}

/** Gets the number of connections of a particular type. */
int MapNumConnections(Map m, TransportType type)
{
	assert(m != NULL);
	assert(transportTypeIsValid(type) || type == ANY);

	int nE = 0;
	for (int i = 0; i < m->nV; i++) {
		for (ConnList curr = m->connections[i]; curr != NULL; curr = curr->next) {
			if (curr->type == type || type == ANY) {
				nE++;
			}
		}
	}

	return nE;
}

////////////////////////////////////////////////////////////////////////

/// Add edges to Graph representing map of Europe
static void addConnections(Map m)
{
	assert(m != NULL);

	for (int i = 0; !isSentinelEdge(CONNECTIONS[i]); i++) {
		addConnection(m, CONNECTIONS[i].v, CONNECTIONS[i].w, CONNECTIONS[i].t);
	}
}

/// Add a new edge to the Map/Graph
static void addConnection(Map m, PlaceId start, PlaceId end, TransportType type)
{
	assert(m != NULL);
	assert(start != end);
	assert(placeIsReal(start));
	assert(placeIsReal(end));
	assert(transportTypeIsValid(type));

	// don't add edges twice
	if (connListContains(m->connections[start], end, type)) return;

	m->connections[start] = connListInsert(m->connections[start], end, type);
	m->connections[end]   = connListInsert(m->connections[end], start, type);
	m->nE++;
}

/// Is this the magic 'sentinel' edge?
static inline bool isSentinelEdge(Connection c)
{
	return c.v == -1 && c.w == -1 && c.t == ANY;
}

/// Insert a node into an adjacency list.
static ConnList connListInsert(ConnList l, PlaceId p, TransportType type)
{
	assert(placeIsReal(p));
	assert(transportTypeIsValid(type));

	ConnList new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate ConnNode");
		exit(EXIT_FAILURE);
	}
	
	new->p = p;
	new->type = type;
	new->next = l;
	return new;
}

/// Does this adjacency list contain a particular value?
static bool connListContains(ConnList l, PlaceId p, TransportType type)
{
	assert(placeIsReal(p));
	assert(transportTypeIsValid(type));

	for (ConnList curr = l; curr != NULL; curr = curr->next) {
		if (curr->p == p && curr->type == type) {
			return true;
		}
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////

ConnList MapGetConnections(Map m, PlaceId p)
{
	assert(placeIsReal(p));
	return m->connections[p];
}

////////////////////////////////////////////////////////////////////////

// find the reachable places from the current place
// given the type of players
PlaceId *MapGetHunterReachable (Map m, PlaceId p, int playerType, 
	Player player, Round round, int *numReturnedLocs,
	bool road, bool rail, bool boat)
{
	assert(playerType == HUNTER);

	PlaceId *arr = calloc(MapNumPlaces(m), sizeof(PlaceId));
	// stay at the same location: rest
	arr[0] = p;
	*numReturnedLocs += 1;
	
	// go through connections list (adjacency list), add into arr.
	ConnList *curr = MapGetConnections(m, p);
	int i = 1;
	while(curr != NULL) {
		if(curr->type == ROAD && road == true) {
			arr[i] = curr->p;
			*numReturnedLocs += 1;
			i++;

		} else if (curr->type == BOAT && boat == true) {
			arr[i] = curr->p;
			*numReturnedLocs += 1;
			i++;
		
		} else if(curr->type == RAIL && rail == true) {
			int sum = round + player;
			if(sum % 4 == 0) {
				continue;

			} else if(sum % 4 == 1) {
				arr[i] = curr->p;
				*numReturnedLocs += 1;
				i++;

			} else if(sum % 4 == 2 || sum % 4 == 3) {
				arr[i] = curr->p;
				*numReturnedLocs += 1;
				i++;
				i = furtherRailMove (m, curr->p, 2, arr, i, numReturnedLocs);
			}
		}
		curr = curr->next;
	}
	
	return arr;
}


// find the reachable rail move when max allowed distance
// is more than 1
// return curr_i (use to increment index for array)
int furtherRailMove (Map m, PlaceId p, int maxDist, PlaceId arr, int curr_i, int *numReturnedLocs)
{
	ConnList *curr = MapGetConnections(m, p);
	while(curr != NULL) {
		if(curr->type == RAIL) {

			// TODO duplication
			// avoid adding existing places
			if (searchInReachable(arr, curr->p) == 0) {
				arr[curr_i] = curr->p;
				*numReturnedLocs += 1;
				curr_i++;

				// if max distance is 3, call a recursion
				if (maxDist == 3) {
					curr_i = furtherRailMove(m, curr->p, 2, arr, curr_i);
				}
			}
		}
	}
	return curr_i;
}

// search the array, does the place already exist in array?
// resturn 1 if it does
int searchInReachable (PlaceId arr, PlaceId newPlace)
{
	int r = 0;
	for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
		if (arr[i] == newPlace) {
			r = 1;
			return r;
		}
	}
	return r;
}




