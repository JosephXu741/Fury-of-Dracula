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
#include "List.h"
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
	Hunter Lord_Godalming;
	Hunter Dr_Seward;
	Hunter Van_Helsing;
	Hunter Mina_Harker;
	Dracula Dracula;
	int *numReturnedMoves;


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
	new->score = GvGetScore(gv);
	new->round = GvGetRound(gv);

	new->numReturnedMoves = 0;


	new->Lord_Godalming.id = PLAYER_LORD_GODALMING;
	new->Lord_Godalming.health = GvGetHealth(gv, PLAYER_LORD_GODALMING);
	new->Lord_Godalming.place = GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING);
	new->Lord_Godalming.placeID = GvGetMoveHistory(gv, PLAYER_LORD_GODALMING, new->numReturnedMoves, false);

	new->Dr_Seward.id = PLAYER_DR_SEWARD;
    new->Dr_Seward.health = GvGetHealth(gv, PLAYER_DR_SEWARD);
    new->Dr_Seward.place = GvGetPlayerLocation(gv, PLAYER_DR_SEWARD);
	new->Dr_Seward.placeID = GvGetMoveHistory(gv, PLAYER_DR_SEWARD, new->numReturnedMoves, false);

    new->Van_Helsing.id = PLAYER_VAN_HELSING;
    new->Van_Helsing.health = GvGetHealth(gv, PLAYER_VAN_HELSING);
    new->Van_Helsing.place = GvGetPlayerLocation(gv, PLAYER_VAN_HELSING);
	new->Van_Helsing.placeID = GvGetMoveHistory(gv, PLAYER_VAN_HELSING, new->numReturnedMoves, false);

    new->Mina_Harker.id = PLAYER_MINA_HARKER;
    new->Mina_Harker.health = GvGetHealth(gv, PLAYER_MINA_HARKER);
    new->Mina_Harker.place = GvGetPlayerLocation(gv, PLAYER_MINA_HARKER);
	new->Mina_Harker.placeID = GvGetMoveHistory(gv, PLAYER_MINA_HARKER, new->numReturnedMoves, false);

    new->Dracula.id = PLAYER_DRACULA;
    new->Dracula.health =  GvGetHealth(gv, PLAYER_DRACULA);
    new->Dracula.place =  GvGetPlayerLocation(gv, PLAYER_DRACULA);

	return new;
}

void HvFree(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
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
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*round = 0;
	return NOWHERE;
}

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*pathLength = 0;
	return NULL;
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

	*numReturnedLocs = 0;
	return GvGetReachable(hv->gv, player, GvGetRound(hv->gv),
            	GvGetPlayerLocation(hv->gv, player), numReturnedLocs);
}


PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	*numReturnedLocs = 0;
	return GvGetReachableByType(hv->gv, player, GvGetRound(hv->gv),
            	GvGetPlayerLocation(hv->gv, player), 
				road, rail, boat, numReturnedLocs);
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
