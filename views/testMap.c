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

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable. Include rail, maxDist is 2\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, VIENNA, 100,
			3, 3, &numLocs, true, true, true);

		//printf("numloc is %d\n", numLocs);
		assert(numLocs == 8);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BERLIN);
		assert(locs[1] == BUDAPEST);
		assert(locs[2] == MUNICH);
		assert(locs[3] == PRAGUE);
		assert(locs[4] == SZEGED);
		assert(locs[5] == VENICE);
		assert(locs[6] == VIENNA);
		assert(locs[7] == ZAGREB);

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable. Include rail, maxDist is 3\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, VIENNA, 100,
			3, 4, &numLocs, true, true, true);

		//printf("numloc is %d\n", numLocs);
		assert(numLocs == 12);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BERLIN);
		assert(locs[2] == BUCHAREST);
		assert(locs[3] == BUDAPEST);
		assert(locs[4] == HAMBURG);
		assert(locs[5] == LEIPZIG);
		assert(locs[6] == MUNICH);
		assert(locs[7] == PRAGUE);
		assert(locs[8] == SZEGED);
		assert(locs[9] == VENICE);
		assert(locs[10] == VIENNA);
		assert(locs[11] == ZAGREB);

		free(locs);
		printf("Test passed!\n");
	}


	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable. Port city, rail maxDist is 2\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, BORDEAUX, 100,
			0, 2, &numLocs, true, true, true);

		/*
		printf("numloc is %d\n", numLocs);
		for(int i = 0; i <12; i++){
			printf("loc[%d]:%d\n",i, locs[i]);
		}
		*/
		assert(numLocs == 12);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BARCELONA);
		assert(locs[1] == BAY_OF_BISCAY);
		assert(locs[2] == BORDEAUX);
		assert(locs[3] == BRUSSELS);
		assert(locs[4] == CLERMONT_FERRAND);
		assert(locs[5] == LE_HAVRE);
		assert(locs[6] == MADRID);
		assert(locs[7] == MARSEILLES);
		assert(locs[8] == NANTES);
		assert(locs[9] == PARIS);
		assert(locs[10] == SARAGOSSA);
		assert(locs[11] == TOULOUSE);

		free(locs);
		printf("Test passed!\n");
	}


	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable. Port city, rail maxDist is 3\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, BORDEAUX, 100,
			0, 3, &numLocs, true, true, true);

		assert(numLocs == 16);
		sortPlaces(locs, numLocs);
		assert(locs[0] == ALICANTE);
		assert(locs[1] == BARCELONA);
		assert(locs[2] == BAY_OF_BISCAY);
		assert(locs[3] == BORDEAUX);
		assert(locs[4] == BRUSSELS);
		assert(locs[5] == CLERMONT_FERRAND);
		assert(locs[6] == COLOGNE);
		assert(locs[7] == LE_HAVRE);
		assert(locs[8] == LISBON);
		assert(locs[9] == MADRID);
		assert(locs[10] == MARSEILLES);
		assert(locs[11] == NANTES);
		assert(locs[12] == PARIS);
		assert(locs[13] == SANTANDER);
		assert(locs[14] == SARAGOSSA);
		assert(locs[15] == TOULOUSE);

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable. In sea\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, ENGLISH_CHANNEL, 100,
			0, 3, &numLocs, true, true, true);

		assert(numLocs == 6);
		sortPlaces(locs, numLocs);
		assert(locs[0] == ATLANTIC_OCEAN);
		assert(locs[1] == ENGLISH_CHANNEL);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == LONDON);
		assert(locs[4] == NORTH_SEA);
		assert(locs[5] == PLYMOUTH);

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetHunterReachable: ByType. Can go nowhere from Naples\n");

		int numLocs = 0;
		PlaceId *locs = MapGetHunterReachable(m, NAPLES, 100,
			0, 4, &numLocs, false, true, false);

		assert(numLocs == 1);
		sortPlaces(locs, numLocs);
		assert(locs[0] == NAPLES);

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetDraculaReachable.\n");

		int numLocs = 0;
		PlaceId *locs = MapGetDraculaReachable(m, BORDEAUX, 200,
			&numLocs, true, false, true);

		assert(numLocs == 6);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BAY_OF_BISCAY);
		assert(locs[1] == BORDEAUX);
		assert(locs[2] == CLERMONT_FERRAND);
		assert(locs[3] == NANTES);
		assert(locs[4] == SARAGOSSA);
		assert(locs[5] == TOULOUSE);

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetDraculaReachable ByType Sea only.\n");

		int numLocs = 0;
		PlaceId *locs = MapGetDraculaReachable(m, BORDEAUX, 200,
			&numLocs, false, false, true);

		assert(numLocs == 2);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BAY_OF_BISCAY);
		assert(locs[1] == BORDEAUX);

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetDraculaReachable ByType Road only.\n");

		int numLocs = 0;
		PlaceId *locs = MapGetDraculaReachable(m, BORDEAUX, 200,
			&numLocs, true, false, false);

		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BORDEAUX);
		assert(locs[1] == CLERMONT_FERRAND);
		assert(locs[2] == NANTES);
		assert(locs[3] == SARAGOSSA);
		assert(locs[4] == TOULOUSE);

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetDraculaReachable Can't go to hospital.\n");

		int numLocs = 0;
		PlaceId *locs = MapGetDraculaReachable(m, SZEGED, 200,
			&numLocs, true, false, true);

		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUDAPEST);
		assert(locs[2] == KLAUSENBURG);
		assert(locs[3] == SZEGED);
		assert(locs[4] == ZAGREB);

		free(locs);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetDraculaReachable ByType nowhere to go.\n");

		int numLocs = 0;
		PlaceId *locs = MapGetDraculaReachable(m, SZEGED, 200,
			&numLocs, false, false, true);

		assert(numLocs == 1);
		sortPlaces(locs, numLocs);
		assert(locs[0] == SZEGED);

		free(locs);
		printf("Test passed!\n");
	}







	{///////////////////////////////////////////////////////////////////
	
		printf("Testing MapGetShortestPath.\n");

		int pathLength = 0;
		PlaceId *locs = MapGetShortestPath(m, CADIZ, BARCELONA, 0, 1, &pathLength);

		assert(pathLength == 2);
		sortPlaces(locs, pathLength);
		//assert(locs[0] == SZEGED);

		free(locs);
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
