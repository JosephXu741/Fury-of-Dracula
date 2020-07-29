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

#include "QueueOriginal.h"
#include "Item.h"

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
int furtherRailMove (Map m, PlaceId p, int maxDist, PlaceId *arr, int curr_i, int *numReturnedLocs);
int searchInReachable (PlaceId *arr, PlaceId newPlace, int length);


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




/*
int *MapGetShortestPath(PlaceId where, int round, int player)
{

	Map newmap = MapNew(); // create a new map
	assert (g != NULL);
	
	int total_length[NUM_REAL_PLACES];
	int visited[NUM_REAL_PLACES];
	int *st_path = malloc(sizeof(int) * NUM_REAL_PLACES);// the array of shortest path

	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		total_length[i] = MAX_STEPS;
		visited[i] = 0;
		st_path[i] = -1;
	}
	//Initialize all 3 arrays
	int totalround;
	total_length[where] = 0;
	Queue newQueue = newQueue();
	QueueJoin(newQueue, where);


	while (QueueIsEmpty(newQueue) == 0) {
		PlaceId lL = QueueLeave(nQueue);
		totalround = round + total_length[lL]; 
		// determine the current round
		visited[lL] = 1;
		// mark visited array
	
		int num_Steps = (player + totalround) % 4;
		// Calculate the number of steps hunters can move when using rail
			
			ConnList link_Vertex = newmap->connections[lL];

			while (link_Vertex != NULL ) {
				
				// If vertex has been vistied
				if (visited[link_Vertex->p] == 1) {
					link_Vertex = link_Vertex->next;
					continue;
				}
				
				// if the connection type is rail
				if (link_Vertex->type == RAIL) {
					if (num_Steps == 0) { //  if the use of trail in this round is zero
						nextVertex = nextVertex->next;
						continue;
					}
					Find_railway(num_Steps, lL, link_Vertex, newmap, 
					    total_length, st_path, visited, newQueue);
					link_Vertex = link_Vertex->next;
					continue;
					
				}

				if (st_path[link_Vertex->p] == -1) {
					QueueJoin(newQueue, link_Vertex->p);
						st_path[link_Vertex->p] = lL;
						total_length[link_Vertex->p] = total_length[lL] + 1;
				}

				if (st_path[link_Vertex->p] != -1) {
					if (total_length[lL] + 1 < total_length[link_Vertex->p]) {
						total_length[link_Vertex->p] = total_length[lL] + 1;
						st_path[nextVertex->p] = lL;
						link_Vertex = link_Vertex->next;
						continue;
					}
				}
				link_Vertex = link_Vertex->next;
			}
		
	}
	
	return st_path;
}


static void Find_railway(int num_Steps, int lL, ConnList link_Vertex, 
    Map g, int *total_length, int *st_path, int *visited, Queue newQueue)
{
	
	// the number of steps is 1 
	if (num_Steps == 1) {
		if (total_length[link_Vertex->p] > total_length[lL] + 1) {
			// we do not visit this location
			if (st_path[link_Vertex->p] == -1) {
				QueueJoin(newQueue, link_Vertex->p);
			}
			total_length[link_Vertex->p] = total_length[lL] + 1;
			st_path[link_Vertex->p] = lL;
		}

		return;
	}

	ConnList current = g->connections[link_Vertex->p];
	//point to the first  node
	while (current != NULL) {
		// determine visited
		if (visited[current->p] == 1) {
			current = current->next;
			continue;
		}
		// Check the rail type
		if (current->type == RAIL)
			Find_railway(num_Steps-1, lL, link_Vertex, g, 
			    total_length, st_path, visited, newQueue);
		current = current->next;
	}
	return;
}
*/





PlaceId *MapGetHunterReachable (Map m, PlaceId p, int playerType, 
	Player player, Round round, int *numReturnedLocs,
	bool road, bool rail, bool boat)
{
	assert(playerType == HUNTER);

	PlaceId *arr = calloc(MapNumPlaces(m), sizeof(PlaceId));
	// stay at the same location: rest
	arr[0] = p;
	*numReturnedLocs += 1;
	printf("%d\n", arr[0]);
	
	// go through connections list, add into arr.
	ConnList curr = MapGetConnections(m, p);
	int i = 1;
	while(curr != NULL) {
		// Firstly: make sure place is unique
		if (searchInReachable(arr, curr->p, i)) {
			curr = curr->next;
			continue;
		}

		if(curr->type == ROAD && road == true) {
			arr[i] = curr->p;
			*numReturnedLocs += 1;
			printf("%d\n", arr[i]);
			i++;

		} else if (curr->type == BOAT && boat == true) {
			arr[i] = curr->p;
			*numReturnedLocs += 1;
			printf("%d\n", arr[i]);
			i++;
		
		} else if(curr->type == RAIL && rail == true) {
			int sum = round + player;
			if(sum % 4 == 0) {
				curr = curr->next;
				continue;

			} else if(sum % 4 == 1) {
				arr[i] = curr->p;
				*numReturnedLocs += 1;
				printf("%d\n", arr[i]);
				i++;

			} else if(sum % 4 == 2 || sum % 4 == 3) {
				arr[i] = curr->p;
				*numReturnedLocs += 1;
				printf("%d\n", arr[i]);
				i++;
				i = furtherRailMove (m, curr->p, sum % 4, arr, i, numReturnedLocs);
			}
		}
		curr = curr->next;
	}
	
	return arr;
}


// find the reachable rail move when max allowed distance
// is more than 1
// return curr_i (use to increment index for array)
// if maxDist == 2, search RAIL in current place (which is 2 dist away)
// if maxDist == 3, go one dist further (which is 3 dist away)
int furtherRailMove (Map m, PlaceId p, int maxDist, PlaceId *arr, int curr_i, int *numReturnedLocs)
{
	ConnList curr = MapGetConnections(m, p);
	while(curr != NULL) {
		if(curr->type == RAIL) {

			// avoid adding existing places
			if (searchInReachable(arr, curr->p, curr_i) == 0) {
				arr[curr_i] = curr->p;
				*numReturnedLocs += 1;
				curr_i++;

				// if max distance is 3, call a recursion with maxDist = 2
				if (maxDist == 3) {
					curr_i = furtherRailMove(m, curr->p, maxDist - 1, 
						arr, curr_i, numReturnedLocs);
				}
			}
		}
	}
	return curr_i;
}

// search the array, does the place already exist in array?
// resturn 1 if it does
int searchInReachable (PlaceId *arr, PlaceId newPlace, int length)
{
	int r = 0;
	for (int i = 0; i < length; i++) {
		if (arr[i] == newPlace) {
			r = 1;
			return r;
		}
	}
	return r;
}


PlaceId *MapGetDraculaReachable (Map m, PlaceId p, int playerType, int *numReturnedLocs,
	bool road, bool rail, bool boat)
{
	assert(playerType == DRACULA);

	PlaceId *arr = calloc(MapNumPlaces(m), sizeof(PlaceId));
	// stay at the same location
	arr[0] = p;
	*numReturnedLocs += 1;
	
	// go through connections list, add into arr.
	ConnList curr = MapGetConnections(m, p);
	int i = 1;
	while(curr != NULL) {

		// cannot go hospital
		if (curr->p == ST_JOSEPH_AND_ST_MARY) {
			continue;

		} else if(curr->type == ROAD && road == true) {
			arr[i] = curr->p;
			*numReturnedLocs += 1;
			i++;

		} else if (curr->type == BOAT && boat == true) {
			arr[i] = curr->p;
			*numReturnedLocs += 1;
			i++;
		}

		curr = curr->next;
	}
	
	return arr;
}

