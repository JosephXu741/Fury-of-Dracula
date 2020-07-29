////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testMap.c: a simple program that checks the Map ADT
// You can change this as much as you want!
// You do not need to submit this file.
//
// 2020-07-10	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"


#include <assert.h>
#include "testUtils.h"

#define MAX_LINE 1024

int main(void)
{
	Map m = MapNew();
	// MapShow(m);
	
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, TOULOUSE, 100,
			3, 1, &numLocs, true, true, true);

		assert(numLocs == 6);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BARCELONA);
		assert(locs[1] == BORDEAUX);
		assert(locs[2] == CLERMONT_FERRAND);
		assert(locs[3] == MARSEILLES);
		assert(locs[4] == SARAGOSSA);
		assert(locs[5] == TOULOUSE);

		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable. Include rail, maxDist is 0\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, VIENNA, 100,
			3, 1, &numLocs, true, true, true);

		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BUDAPEST);
		assert(locs[1] == MUNICH);
		assert(locs[2] == PRAGUE);
		assert(locs[3] == VIENNA);
		assert(locs[4] == ZAGREB);

		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable. Include rail, maxDist is 1\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, VIENNA, 100,
			3, 2, &numLocs, true, true, true);

		//printf("numloc is %d\n", numLocs);
		assert(numLocs == 6);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BUDAPEST);
		assert(locs[1] == MUNICH);
		assert(locs[2] == PRAGUE);
		assert(locs[3] == VENICE);
		assert(locs[4] == VIENNA);
		assert(locs[5] == ZAGREB);

		printf("Test passed!\n");
	}




	/*
	char buffer[MAX_LINE];

	printf("\nType a location name to see its connections: ");
	while (fgets(buffer, MAX_LINE, stdin) != NULL) {
		buffer[strlen(buffer) - 1] = '\0'; // remove newline character
		
		PlaceId place = placeNameToId(buffer);
		if (place == NOWHERE) {
			printf("Unknown location '%s'\n", buffer);
		} else {
			for (ConnList c = MapGetConnections(m, place); c != NULL; c = c->next) {
				const char *dest = placeIdToName(c->p);
				const char *transportType = transportTypeToString(c->type);
				printf("%s connects to %s by %s\n", buffer, dest, transportType);
			}
		}
		
		printf("\nType a location name to see its connections: ");
	}
	*/
}
