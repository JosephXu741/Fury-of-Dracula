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
#include "Places.h"

// add your own #includes here



struct draculaView {
	char *pastPlays;
	GameView gv;
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
	new->gv = GvNew(pastPlays, messages);
	new->pastPlays = pastPlays;

	return new;
}

void DvFree(DraculaView dv)
{
	GvFree(dv->gv);
	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv)
{
	return GvGetRound(dv->gv);
}

int DvGetScore(DraculaView dv)
{
	return GvGetScore(dv->gv);
}

int DvGetHealth(DraculaView dv, Player player)
{
	return GvGetHealth(dv->gv, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	return GvGetPlayerLocation(dv->gv, player);
}


PlaceId DvGetVampireLocation(DraculaView dv)
{
	return GvGetVampireLocation(dv->gv);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	return GvGetTrapLocations(dv->gv, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// If Dracula has not made any moves yet, return NOWHERE
	if (GvGetPlayerLocation(dv->gv, PLAYER_DRACULA) == NOWHERE){
		*numReturnedMoves = 0;
		return NULL;
	}

	// reachable array includes all places that are reachable while last_moves gets the past 5 moves
	PlaceId *reachable = GvGetReachable(dv->gv, PLAYER_DRACULA, GvGetRound(dv->gv), GvGetPlayerLocation(dv->gv, PLAYER_DRACULA), numReturnedMoves);
	PlaceId *last_moves = GvGetLastMoves(dv->gv, PLAYER_DRACULA, TRAIL_SIZE, numReturnedMoves, false);
	int hide = 0;
	int db = 0;
	int newSize = 0;
	PlaceId *new = malloc(sizeof(int));

	// Check if the moves HIDE or DOUBLE_BACK have been made in the past 5 moves
	for (int i = 0; last_moves[i]; i++){
		if (last_moves[i] == HIDE){
			hide = 1;
		}
		if (last_moves[i] >= DOUBLE_BACK_1 && last_moves[i] <= DOUBLE_BACK_5){
			db = 1;
		}
	}
	// If no HIDE moves, HIDE is always going to be a valid move
	if (hide == 0) {
		new = realloc(new, (newSize + 1) * sizeof(*new));
		new[newSize] = HIDE;
		newSize++;
	} 


	// For every value in reachable, add to new if its not in last_moves
	for (int i = 0; reachable[i]; i++){
		int dupe = 0;
		for (int j = 0; last_moves[j]; j++){
			if (reachable[i] == last_moves[j]){
				dupe = 1;
				// If it exists in last_moves, check to see if it can be a DOUBLE_BACK move
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

	*numReturnedMoves = newSize;
	return new;

}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	if (GvGetPlayerLocation(dv->gv, PLAYER_DRACULA) == NOWHERE){
		*numReturnedLocs = 0;
		return NULL;
	}

	PlaceId *reachable = GvGetReachable(dv->gv, PLAYER_DRACULA, GvGetRound(dv->gv), GvGetPlayerLocation(dv->gv, PLAYER_DRACULA), numReturnedLocs);
	PlaceId *last_moves = GvGetLastMoves(dv->gv, PLAYER_DRACULA, TRAIL_SIZE, numReturnedLocs, false);
	int hide = 0;
	int db = 0;
	int newSize = 0;
	PlaceId *new = malloc(sizeof(int));

	for (int i = 0; last_moves[i]; i++){
		if (last_moves[i] == GvGetPlayerLocation(dv->gv, PLAYER_DRACULA)){
			hide = 1;
		}
		if (last_moves[i] >= DOUBLE_BACK_1 && last_moves[i] <= DOUBLE_BACK_5){
			db = 1;
		}
	}
	if (hide == 0) {
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

	*numReturnedLocs = newSize;
	return new;

}



PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	
	if (GvGetPlayerLocation(dv->gv, PLAYER_DRACULA) == NOWHERE){
		*numReturnedLocs = 0;
		return NULL;
	}
	PlaceId *reachable = GvGetReachableByType(dv->gv, PLAYER_DRACULA, GvGetRound(dv->gv), GvGetPlayerLocation(dv->gv, PLAYER_DRACULA), road, false, boat, numReturnedLocs);
	PlaceId *last_moves = GvGetLastMoves(dv->gv, PLAYER_DRACULA, TRAIL_SIZE, numReturnedLocs, false);
	int hide = 0;
	int db = 0;
	int newSize = 0;
	PlaceId *new = malloc(sizeof(int));
	for (int i = 0; last_moves[i]; i++){
		if (last_moves[i] == GvGetPlayerLocation(dv->gv, PLAYER_DRACULA)){
			hide = 1;
		}
		if (last_moves[i] >= DOUBLE_BACK_1 && last_moves[i] <= DOUBLE_BACK_5){
			db = 1;
		}
	}
	if (hide == 0) {
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
	
	*numReturnedLocs = newSize;
	return new;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{

	if (GvGetPlayerLocation(dv->gv, player) == NOWHERE){
		*numReturnedLocs = 0;
		return NULL;
	}

	int newSize = 0;
	PlaceId *new = malloc(sizeof(int));
	PlaceId location = DvGetPlayerLocation(dv, player);
	PlaceId *reachable = GvGetReachable(dv->gv, player, GvGetRound(dv->gv), location, numReturnedLocs);

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


	if (newSize == 0){
		*numReturnedLocs = 0;
		return NULL;
	}
	*numReturnedLocs = newSize;
	return new;
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{

	if (GvGetPlayerLocation(dv->gv, player) == NOWHERE){
		*numReturnedLocs = 0;
		return NULL;
	}
	int newSize = 0;
	PlaceId *new = malloc(sizeof(int));
	PlaceId location = DvGetPlayerLocation(dv, player);
	PlaceId *reachable = GvGetReachableByType(dv->gv, player, GvGetRound(dv->gv), location, road, rail, boat, numReturnedLocs);

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

	if (newSize == 0){
		*numReturnedLocs = 0;
		return NULL;
	}

	*numReturnedLocs = newSize;
	return new;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions
// TODO
