////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"

// add your own #includes here
#include "Trap.h"
#include "Queue.h"

#define PREMATURE_VAMPIRE 0
#define REGULAR_TRAP	  1


// TODO: ADD YOUR OWN STRUCTS HERE
typedef struct hunter {
	int id;
	int health;
	Place place;
	PlaceId *moveHistory;
} Hunter;

typedef struct dracula {
	int id;
	int health;
	Place place;
	PlaceId *moveHistory;
} Dracula;



struct hunterView {
	// TODO: ADD FIELDS HERE
	GameView gv;
	Map map;
	int score;
	Round round;
	int *numReturnedMoves;
	int *shortest_arry[NUM_PLAYERS - 1];

};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	HunterView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}

	new->gv = GvNew(pastPlays, messages);
	new->map = MapNew();
	new->score = GvGetScore(new->gv);
	new->round = GvGetRound(new->gv);
	new->numReturnedMoves = 0;

	//Initialize the shortest_path array
    for (int a = 0; a < NUM_PLAYERS - 1; a++) {
		new->hShortestP[a] = MapGetShortestPath(GvGetPlayerLocation(new->gv, a),
		    GvGetRound(new->gv),a);
	} 


	return new;
}

void HvFree(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		free(hv->shortest_arry[i]);
	} //  free the shortestpath array
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv)
{
	return GvGetRound(hv->gv);
}

Player HvGetPlayer(HunterView hv)
{
	// TODO implment GvGetPlayer in gv
	return GvGetPlayer(hv->gv);
}

int HvGetScore(HunterView hv)
{
	return GvGetScore(hv->gv);
}

int HvGetHealth(HunterView hv, Player player)
{
	return GvGetHealth(hv->gv, player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	return GvGetPlayerLocation(hv->gv, player);
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	return GvGetVampireLocation(hv->gv);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	bool freec = false;
	int Returned = 0 ;
	PlaceId *gethistory = GvGetMoveHistory(hv->gv,PLAYER_DRACULA,&Returned,&freec);
	PlaceId where = NOWHERE;
	for(int i = 0;i < Returned;i++){
			if(gethistory[i] != CITY_UNKNOWN && gethistory[i] != SEA_UNKNOWN && 
			    gethistory[i] != CASTLE_DRACULA){
				where = gethistory[i];
				*round = i;
				break;
			}
	}
	if(freec == true){
		free(moveget);
	}


	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return where;
}

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	return 0
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	
	*numReturnedLocs = 0;
	return GvGetReachable(hv->gv, GvGetPlayer(hv->gv), GvGetRound(hv->gv),
            	GvGetPlayerLocation(hv->gv, GvGetPlayer(hv->gv)), numReturnedLocs);
}


PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{

	*numReturnedLocs = 0;
	return GvGetReachableByType(hv->gv, GvGetPlayer(hv->gv), GvGetRound(hv->gv),
            	GvGetPlayerLocation(hv->gv, GvGetPlayer(hv->gv)), 
				road, rail, boat, numReturnedLocs);
}


PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{

	// TODO If Dracula's current location is not revealed, the
 	// * function should set *numReturnedLocs to 0 and return NULL.
	*numReturnedLocs = 0;
	return GvGetReachable(hv->gv, player, GvGetRound(hv->gv),
            	GvGetPlayerLocation(hv->gv, player), numReturnedLocs);
}


PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO If Dracula's current location is not revealed, the
 	// * function should set *numReturnedLocs to 0 and return NULL.
	*numReturnedLocs = 0;
	return GvGetReachableByType(hv->gv, player, GvGetRound(hv->gv),
            	GvGetPlayerLocation(hv->gv, player), 
				road, rail, boat, numReturnedLocs);
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
