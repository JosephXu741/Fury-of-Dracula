////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "dracula.h"
#include "DraculaView.h"
#include "Game.h"
#include "Places.h"

void firstMove(DraculaView dv, PlaceId *hunterReachable, int num);
PlaceId *GetHunterReachable(DraculaView dv);
bool isPlaceInPlaceArray(PlaceId place, PlaceId *placeArray, int num);
PlaceId translatePlaceToMove(DraculaView dv, PlaceId place, PlaceId *moveArray, int num);


void decideDraculaMove(DraculaView dv)
{
	PlaceId *hunterReachable = GetHunterReachable(dv);
	int numReturnedLocs = 0;
	PlaceId *reachable = DvWhereCanIGo(dv, &numReturnedLocs);
	int numReturned = 0;
	bool fal = false;
	// GvGetLastLocations is giving me DOUBLE_BACK and HIDE moves even though it shouldnt
	// This is the View code provided by cse...
	PlaceId *pastLocations = DvGetPlayerHistory(dv, PLAYER_DRACULA, &numReturned, &fal);
	if (numReturned > 5) {
		// Bootleg GvGetLastLocations
		for (int i = 0; i < 5; i++) {
			pastLocations[i] = pastLocations[numReturned - 5 + i];
		}
		numReturned = 5;
	}

	for (int i = 0; i < numReturnedLocs; i++) {
		printf("%d\n", reachable[i]);
	}

	for (int i = 0; i < numReturned; i++) {
		printf("past locations: %d\n", pastLocations[i]);
	}

	if (DvGetRound(dv) == 0) {
		firstMove(dv, hunterReachable, numReturnedLocs);
		return;
	}

	if (numReturnedLocs == 0){
		registerBestPlay("TP", "lol");
		return;
	}

	int numReturnedMoves = 0;
	PlaceId *validMoves = DvGetValidMoves(dv, &numReturnedMoves);
	for (int i = 0; i < numReturnedMoves; i++) {
		printf("valid moves: %d\n", validMoves[i]);
	}
	for (int i = 0; i < numReturnedMoves; i++){
		if (!isPlaceInPlaceArray(reachable[i], hunterReachable, numReturnedLocs) && !isPlaceInPlaceArray(reachable[i], pastLocations, numReturned)) {
			int move = translatePlaceToMove(dv, reachable[i], validMoves, numReturnedMoves);
			registerBestPlay(placeIdToAbbrev(move), "lol");
			return;
		}
	}
	registerBestPlay(placeIdToAbbrev(validMoves[0]), "lol");

}




// How to determine where Dracula Starts
void firstMove(DraculaView dv, PlaceId *hunterReachable, int num) {

	if (isPlaceInPlaceArray(CASTLE_DRACULA, hunterReachable, num)) {
		registerBestPlay("CD", "lol");
		return;
	}

	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		if (i == ST_JOSEPH_AND_ST_MARY || placeIsSea(i) || 
			isPlaceInPlaceArray(i, hunterReachable, num)) {
			continue;
		}
		else {
			registerBestPlay(placeIdToAbbrev(i), "lol");
			return;
			}
		}
}

// return an array of all reachable places from all hunters
PlaceId *GetHunterReachable(DraculaView dv) {

	PlaceId *hunterReachable = malloc(sizeof(int));

	PlaceId *places;
	int numReturnedLocs;
	int size = 0;
	// loop through all hunters and add their reachables into hunterReachable
	for (int i = 0; i < 4; i++) {
		numReturnedLocs = 0;
		places = DvWhereCanTheyGo(dv, i, &numReturnedLocs);
		// If hunterReachable is not 0
		if (numReturnedLocs != 0) {
			// For every place in a particular hunter's reachable
			for (int j = 0; j < numReturnedLocs; j++) {
				// If the place is not already in hunterReachable
				if (size == 0 || !isPlaceInPlaceArray(places[j], hunterReachable, numReturnedLocs)) {
					// increase hunterReachable array by one and add place.
					hunterReachable = realloc(hunterReachable, (size + 1) * sizeof(int));
					hunterReachable[size] = places[j];
					size++;
				}
			}
		}
	}
	return hunterReachable;

}

// Given a place and an array, return True if the place is in the array
bool isPlaceInPlaceArray(PlaceId place, PlaceId *placeArray, int num) {

	for (int i = 0; i < num; i++) {
		if (place == placeArray[i]) {
			return true;
		}
	}
	return false; 

}

// Given a place, and move array, register an appropriate move
PlaceId translatePlaceToMove(DraculaView dv, PlaceId place, PlaceId *moveArray, int num) {

	for (int i = 0; i < num; i++) {
		if (place == moveArray[i]) {
			return place;
		}
	}

	if (DvGetPlayerLocation(dv, PLAYER_DRACULA) == place) {
		if (isPlaceInPlaceArray(HIDE, moveArray, num)) {
			return HIDE;
		} else if (isPlaceInPlaceArray(DOUBLE_BACK_1, moveArray, num)) {
			return DOUBLE_BACK_1;
		}
	}
	int numReturnedLocs = 0;
	bool f = false;
	PlaceId *locationHistory = DvGetPlayerHistory(dv, PLAYER_DRACULA, &numReturnedLocs, &f);
	
	int i = 0;
	while (i < numReturnedLocs) {
		if (locationHistory[i] == place) {
			break;
		}
		i++;
	}
	int db = numReturnedLocs - i;
	switch (db) {
		case 5 : return DOUBLE_BACK_5; break;
		case 4 : return DOUBLE_BACK_4; break;
		case 3 : return DOUBLE_BACK_3; break;
		case 2 : return DOUBLE_BACK_2; break;
		default : return DOUBLE_BACK_1; break;
	}


}