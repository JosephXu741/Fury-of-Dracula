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


struct draculaView {
	char *pastPlays;
	Message messages;
	GameView gv;
	Map map;
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
	strcpy(new->messages, messages);
	new->map = MapNew();

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
