////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
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
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
#include "List.h"
#include "Queue.h"
// add your own #includes here

#define ALL_TRAPS 0
#define PREMATURE_VAMPIRE 1
#define REGULAR_TRAP	  2

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


struct gameView {
	Message message;
	Map map;
	int score;
	int turn;
	Round round;
	Trail trail;		// a queue of all dracula's trails
	TrapList traps;		// a list of all traps
	Hunter Lord_Godalming;
	Hunter Dr_Seward;
	Hunter Van_Helsing;
	Hunter Mina_Harker;
	Dracula Dracula;
	int *numTraps;
} ;

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[])
{
	GameView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}
	int total_turns = 0;
	new->traps = newTrapList();
	new->trail = newTrail();
	new->map = MapNew();
	new->score = GAME_START_SCORE;
	

	char s[10000];
    strcpy(s, pastPlays);
    char *token = strtok(s, " ");
    while (token != NULL){
        int cmp = strncmp(token, "D", 1);
        if (cmp == 0){
            char abbv[3];
            abbv[0] = token[1];
            abbv[1] = token[2];
            abbv[2] = '\0';
            PlaceId placeid = placeAbbrevToId(abbv); 
            TrailJoin(new->trail, placeid);
			if (TrailLength(new->trail) >= TRAIL_SIZE) {
				TrailLeave(new->trail);
			}
            
        } else {
            char abbv[3];
            abbv[0] = token[1];
            abbv[1] = token[2];
            abbv[2] = '\0';
            PlaceId placeid = placeAbbrevToId(abbv); 

        }
		total_turns++;
        token = strtok(NULL, " ");
    }
	new->round = total_turns/NUM_PLAYERS + 1;
	new->numTraps = TotalTrapsTrail(new->trail);
	
	return new;
}

void GvFree(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	// TODO
	return gv->round;
}

Player GvGetPlayer(GameView gv)
{
	// TODO
	int player = gv->turn % 6;			//mod 6????????
	switch(player) {
		case 0 :
			player = PLAYER_LORD_GODALMING;
			break;
		case 1 :
			player = PLAYER_DR_SEWARD;
			break;
		case 2 :
			player = PLAYER_VAN_HELSING;
			break;
		case 3 :
			player = PLAYER_MINA_HARKER;
			break;
		case 4 :
			player = PLAYER_DRACULA;
			break;
		default :
			player = PLAYER_LORD_GODALMING;
   }
	return player;
}


int GvGetScore(GameView gv)
{
	// TODO
	return gv->score;
}

int GvGetHealth(GameView gv, Player player)
{
	// TODO
	return player->health;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return NOWHERE;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO
	// Gets the locations of all active traps.
	//This  function should return the locations in a dynamically allocated array

	return getTrailLocation(gv->trail);
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions
