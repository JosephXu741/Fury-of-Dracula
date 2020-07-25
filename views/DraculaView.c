////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
#include "Queue.h"
#include "List.h"
// add your own #includes here
#define PREMATURE_VAMPIRE 0
#define REGULAR_TRAP	  1

// TODO: ADD YOUR OWN STRUCTS HERE
typedef struct hunter {
	int id;
	int health;
	PlaceId place;
} Hunter;

typedef struct dracula {
	int id;
	int health;
	PlaceId place;
} Dracula;


struct draculaView {
	Map map;
	int score;
	Round round;
	TrapList traps;
	Trail trail;
	Hunter Lord_Godalming;
	Hunter Dr_Seward;
	Hunter Van_Helsing;
	Hunter Mina_Harker;
	Dracula Dracula;
} ;

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[])
{
	DraculaView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaView\n");
		exit(EXIT_FAILURE);
	}
	GameView gv = GvNew(pastPlays, messages);
	new->round = GvGetRound(gv);
	new->score = GvGetScore(gv);
	new->map = MapNew();

	new->Lord_Godalming.id = PLAYER_LORD_GODALMING;
	new->Lord_Godalming.health = GvGetHealth(gv, PLAYER_LORD_GODALMING);
	new->Lord_Godalming.place = GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING);

	new->Dr_Seward.id = PLAYER_DR_SEWARD;
    new->Dr_Seward.health = GvGetHealth(gv, PLAYER_DR_SEWARD);
    new->Dr_Seward.place = GvGetPlayerLocation(gv, PLAYER_DR_SEWARD);

    new->Van_Helsing.id = PLAYER_VAN_HELSING;
    new->Van_Helsing.health = GvGetHealth(gv, PLAYER_LORD_GODALMING);
    new->Van_Helsing.place = GvGetPlayerLocation(gv, PLAYER_VAN_HELSING);

    new->Mina_Harker.id = PLAYER_MINA_HARKER;
    new->Mina_Harker.health = GvGetHealth(gv, PLAYER_LORD_GODALMING);
    new->Mina_Harker.place = GvGetPlayerLocation(gv, PLAYER_MINA_HARKER);

    new->Dracula.id = PLAYER_DRACULA;
    new->Dracula.health =  GvGetHealth(gv, PLAYER_LORD_GODALMING);
    new->Dracula.place =  GvGetPlayerLocation(gv, PLAYER_DRACULA);

	new->traps = newList();
	
	new->trail = newQueue();
	

	return new;
}

void DvFree(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv)
{
	return dv->round;
}

int DvGetScore(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

int DvGetHealth(DraculaView dv, Player player)
{
	int health = 0;

	if (dv->Lord_Godalming.id == player) {
		health = dv->Lord_Godalming.health;
	} else if (dv->Dr_Seward.id == player) {
		health = dv->Dr_Seward.health;
	} else if (dv->Van_Helsing.id == player) {
		health = dv->Van_Helsing.health;
	} else if (dv->Mina_Harker.id == player) {
		health = dv->Mina_Harker.health;
	} 

	return health;
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	
	return NOWHERE;
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	return NULL;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
