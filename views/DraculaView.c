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
#include <string.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
#include "Queue.h"
#include "Trap.h"
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
	char *pastPlays;
	Message messages;
	Map map;
	int score;
	Round round;
	PlaceId vampLoc;
	Hunter Lord_Godalming;
	Hunter Dr_Seward;
	Hunter Van_Helsing;
	Hunter Mina_Harker;
	Dracula Dracula;
	PlaceId *trapLocations;
	int *numReturnedMoves;
	int *numTraps;
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
	new->pastPlays = pastPlays;
	strcpy(new->messages, messages);
	new->round = GvGetRound(gv);
	new->score = GvGetScore(gv);
	new->map = MapNew();

	new->numReturnedMoves = 0;

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

	new->numTraps = 0;


	new->vampLoc = GvGetVampireLocation(gv);
	new->trapLocations = GvGetTrapLocations(gv, new->numTraps);

	return new;
}

void DvFree(DraculaView dv)
{
	free(dv->trapLocations);
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
	return dv->score;
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
	} else if (dv->Dracula.id == player) {
		health = dv->Dracula.health;
	} 

	return health;
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	PlaceId location;
	if (dv->round == 1) {
		if (player == dv->Lord_Godalming.id /*&& location is HOSPITAL*/) return dv->Lord_Godalming.place;			
		if (player == dv->Dr_Seward.id /*&& location is HOSPITAL*/) return dv->Dr_Seward.place;		
		if (player == dv->Van_Helsing.id /*&& location is HOSPITAL*/) return dv->Van_Helsing.place;		
		if (player == dv->Mina_Harker.id /*&& location is HOSPITAL*/) return dv->Mina_Harker.place;	
	    if (player == dv->Dracula.id /*&& location is CASTLE/*/) return dv->Dracula.place;	
	}	
	return NOWHERE;

}


PlaceId DvGetVampireLocation(DraculaView dv)
{
	return dv->vampLoc;
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	*numTraps = dv->numTraps;
	return dv->trapLocations;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	if (dv->round == 0){
		*numReturnedMoves = 0;
		return NULL;
	}

	GameView gv = GvNew(dv->pastPlays, dv->messages);
	PlaceId *reachable = GvGetReachable(gv, PLAYER_DRACULA, dv->round, dv->Dracula.place, numReturnedMoves);
	PlaceId *last_moves = GvGetLastMoves(gv, PLAYER_DRACULA, TRAIL_SIZE, numReturnedMoves, false);
	int hide = 0;
	int db = 0;
	int newSize = 0;
	PlaceId *new;
	for (int i = 0; last_moves[i]; i++){
		if (last_moves[i] == HIDE){
			hide = 1;
		}
		if (last_moves[i] >= DOUBLE_BACK_1 && last_moves[i] <= DOUBLE_BACK_5){
			db = 1;
		}
	}
	if (hide = 0) {
		new = realloc(new, (newSize + 1) * sizeof(*new));
		new[newSize] = HIDE;
		newSize++;
	} 


	for (int i = 0; reachable[i]; i++){
		int dupe = 0;
		for (int j = 0; last_moves[j]; j++){
			if (reachable[i] == last_moves[j]){
				dupe = 1;
				if (db == 0){
					new = realloc(new, (newSize + 1) * sizeof(*new));
					new[newSize] = DOUBLE_BACK_1 + j;
					newSize++;
				}
			}
		}
		if (dupe == 0) {
			new = realloc(new, (newSize + 1) * sizeof(*new));
			new[newSize] = reachable[i];
			newSize++;
		}
	}

	if (newSize == 0){
		*numReturnedMoves = 0;
		return NULL;
	}

	return new;

}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	if (dv->round == 0){
		*numReturnedLocs = 0;
		return NULL;
	}
	GameView gv = GvNew(dv->pastPlays, dv->messages);
	PlaceId *reachable = GvGetReachable(gv, PLAYER_DRACULA, dv->round, dv->Dracula.place, numReturnedLocs);
	PlaceId *last_moves = GvGetLastMoves(gv, PLAYER_DRACULA, TRAIL_SIZE, numReturnedLocs, false);
	int hide = 0;
	int db = 0;
	int newSize = 0;
	PlaceId *new;
	for (int i = 0; last_moves[i]; i++){
		if (last_moves[i] == dv->Dracula.place){
			hide = 1;
		}
		if (last_moves[i] >= DOUBLE_BACK_1 && last_moves[i] <= DOUBLE_BACK_5){
			db = 1;
		}
	}
	if (hide = 0) {
		new = realloc(new, (newSize + 1) * sizeof(*new));
		new[newSize] = HIDE;
		newSize++;
	} 


	for (int i = 0; reachable[i]; i++){
		int dupe = 0;
		for (int j = 0; last_moves[j]; j++){
			if (reachable[i] == last_moves[j]){
				dupe = 1;
				if (db == 0){
					new = realloc(new, (newSize + 1) * sizeof(*new));
					new[newSize] = reachable[i];
					newSize++;
				}
			}
		}
		if (dupe == 0) {
			new = realloc(new, (newSize + 1) * sizeof(*new));
			new[newSize] = reachable[i];
			newSize++;
		}
	}

	if (newSize == 0){
		*numReturnedLocs = 0;
		return NULL;
	}

	return new;

}



PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	
	if (dv->round == 0){
		*numReturnedLocs = 0;
		return NULL;
	}
	GameView gv = GvNew(dv->pastPlays, dv->messages);
	PlaceId *reachable = GvGetReachableByType(gv, PLAYER_DRACULA, dv->round, dv->Dracula.place, road, false, boat, numReturnedLocs);
	PlaceId *last_moves = GvGetLastMoves(gv, PLAYER_DRACULA, TRAIL_SIZE, numReturnedLocs, false);
	int hide = 0;
	int db = 0;
	int newSize = 0;
	PlaceId *new;
	for (int i = 0; last_moves[i]; i++){
		if (last_moves[i] == dv->Dracula.place){
			hide = 1;
		}
		if (last_moves[i] >= DOUBLE_BACK_1 && last_moves[i] <= DOUBLE_BACK_5){
			db = 1;
		}
	}
	if (hide = 0) {
		new = realloc(new, (newSize + 1) * sizeof(*new));
		new[newSize] = HIDE;
		newSize++;
	} 


	for (int i = 0; reachable[i]; i++){
		int dupe = 0;
		for (int j = 0; last_moves[j]; j++){
			if (reachable[i] == last_moves[j]){
				dupe = 1;
				if (db == 0){
					new = realloc(new, (newSize + 1) * sizeof(*new));
					new[newSize] = reachable[i];
					newSize++;
				}
			}
		}
		if (dupe == 0) {
			new = realloc(new, (newSize + 1) * sizeof(*new));
			new[newSize] = reachable[i];
			newSize++;
		}
	}

	if (newSize == 0){
		*numReturnedLocs = 0;
		return NULL;
	}

	return new;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{

	// Implement something that detects if the player has had their turn yet

	GameView gv = GvNew(dv->pastPlays, dv->messages);
	int newSize = 0;
	PlaceId *new;
	for (Player p = 0; p < 4; p++) {
		PlaceId location = DvGetPlayerLocation(dv, p);
		PlaceId *reachable = GvGetReachable(gv, p, dv->round, location, numReturnedLocs);

		for (int i = 0; reachable[i]; i++){
			int dupe = 0;
			for (int j = 0; new[j]; j++){
				if (reachable[i] == new[j]){
					dupe = 1;
				}
			}
			if (dupe == 0) {
				new = realloc(new, (newSize + 1) * sizeof(*new));
				new[newSize] = reachable[i];
				newSize++;
			}
		}
	}

	if (newSize == 0){
		*numReturnedLocs = 0;
		return NULL;
	}

	return new;
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{

	// Implement something that detects if the player has had their turn yet

	GameView gv = GvNew(dv->pastPlays, dv->messages);
	int newSize = 0;
	PlaceId *new;
	for (Player p = 0; p < 4; p++) {
		PlaceId location = DvGetPlayerLocation(dv, p);
		PlaceId *reachable = GvGetReachableByType(gv, p, dv->round, location, road, rail, boat, numReturnedLocs);

		for (int i = 0; reachable[i]; i++){
			int dupe = 0;
			for (int j = 0; new[j]; j++){
				if (reachable[i] == new[j]){
					dupe = 1;
				}
			}
			if (dupe == 0) {
				new = realloc(new, (newSize + 1) * sizeof(*new));
				new[newSize] = reachable[i];
				newSize++;
			}
		}
	}

	if (newSize == 0){
		*numReturnedLocs = 0;
		return NULL;
	}

	return new;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions
// TODO
