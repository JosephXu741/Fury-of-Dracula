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
#include "List.h"
#include "Queue.h"
// add your own #includes here
#define PREMATURE_VAMPIRE 0
#define REGULAR_TRAP	  1

// TODO: ADD YOUR OWN STRUCTS HERE
typedef struct hunter {
	int id;
	int health;
	Place place;
} Hunter;

typedef struct dracula {
	int id;
	int health;
	Place place;
} Dracula;


struct draculaView {
	char *pastPlays;
	Message *message;
	Map map;
	int score;
	int turn;
	Round round;
	Trail trail;
	TrapList traps;
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
	Place hunterStart;
	hunterStart.id = HOSPITAL_PLACE;
	hunterStart.name = placeIdToName(hunterStart.id);
	hunterStart.abbrev = placeIdToAbbrev(hunterStart.id);

	Place draculaStart;
	draculaStart.id = CASTLE_DRACULA;
	draculaStart.name = placeIdToName(draculaStart.id);
	draculaStart.abbrev = placeIdToAbbrev(draculaStart.id);
	draculaStart.type = LAND;

	Message *mg = messages;

	new->pastPlays = pastPlays;
	new->message = mg;
	new->map = MapNew();
	new->score = GAME_START_SCORE;
	new->turn = 1;
	new->round = 1;
	new->trail = NewTrail();
	new->traps = NewTrapList();

	new->Lord_Godalming.id = PLAYER_LORD_GODALMING;
	new->Lord_Godalming.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Lord_Godalming.place = hunterStart;

	new->Dr_Seward.id = PLAYER_DR_SEWARD;
	new->Dr_Seward.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Dr_Seward.place = hunterStart;

	new->Van_Helsing.id = PLAYER_VAN_HELSING;
	new->Van_Helsing.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Van_Helsing.place = hunterStart;

	new->Mina_Harker.id = PLAYER_MINA_HARKER;
	new->Mina_Harker.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Mina_Harker.place = hunterStart;

	new->Dracula.id = PLAYER_DRACULA;
	new->Dracula.health = GAME_START_BLOOD_POINTS;
	new->Dracula.place = draculaStart;
	

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
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return 0;
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
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
