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

#include "dracula.h"
#include "DraculaView.h"
#include "Game.h"
#include "Places.h"

void firstMove(DraculaView dv, PlaceId *hunterReachable);
PlaceId *GetHunterReachable(DraculaView dv);
bool isPlaceInPlaceArray(PlaceId place, PlaceId *placeArray);
PlaceId translatePlaceToMove(DraculaView dv, PlaceId place, PlaceId *moveArray);


void decideDraculaMove(DraculaView dv)
{
	PlaceId *hunterReachable = GetHunterReachable(dv);
	int numReturnedLocs = 0;
	PlaceId *reachable = DvWhereCanIGo(dv, &numReturnedLocs);
	//int health = DvGetHealth(dv, PLAYER_DRACULA);


	if (DvGetRound(dv) == 0) {
		firstMove(dv, hunterReachable);
		return;
	}

	if (numReturnedLocs == 0){
		registerBestPlay("TP", "lol");
		return;
	}

	int numReturnedMoves = 0;
	PlaceId *validMoves = DvGetValidMoves(dv, &numReturnedMoves);
	for (int i = 0; reachable[i]; i++){
		if (!isPlaceInPlaceArray(reachable[i], hunterReachable)) {
			int move = translatePlaceToMove(dv, reachable[i], validMoves);
			registerBestPlay(placeIdToAbbrev(move), "lol");
			return;
		}
	}
	registerBestPlay(placeIdToAbbrev(validMoves[0]), "lol");

}




// How to determine where Dracula Starts
void firstMove(DraculaView dv, PlaceId *hunterReachable) {

	if (isPlaceInPlaceArray(CASTLE_DRACULA, hunterReachable)) {
		registerBestPlay("CD", "lol");
		return;
	}

	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		if (i == ST_JOSEPH_AND_ST_MARY || placeIsSea(i) || 
			isPlaceInPlaceArray(i, hunterReachable)) {
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
	int numReturnedLocs = 0;
	int size = 0;
	// loop through all hunters and add their reachables into hunterReachable
	for (int i = 0; i < 4; i++) {
		places = DvWhereCanTheyGo(dv, i, &numReturnedLocs);
		// If hunterReachable is not 0
		if (numReturnedLocs != 0) {
			// For every place in a particular hunter's reachable
			for (int j = 0; places[j]; j++) {
				// If the place is not already in hunterReachable
				if (size == 0 || !isPlaceInPlaceArray(places[j], hunterReachable)) {
					// increase hunterReachable array by one and add place.
					hunterReachable = realloc(hunterReachable, size + 1);
					hunterReachable[size] = places[j];
					size++;
				}
			}
		}
	}
	return hunterReachable;

}

// Given a place and an array, return True if the place is in the array
bool isPlaceInPlaceArray(PlaceId place, PlaceId *placeArray) {

	for (int i = 0; placeArray[i]; i++) {
		if (place == placeArray[i]) {
			return true;
		}
	}
	return false; 

}

// Given a place, and move array, register an appropriate move
PlaceId translatePlaceToMove(DraculaView dv, PlaceId place, PlaceId *moveArray) {

	for (int i = 0; moveArray[i]; i++) {
		if (place == moveArray[i]) {
			return place;
		}
	}
	if (DvGetPlayerLocation(dv, PLAYER_DRACULA) == place) {
		if (isPlaceInPlaceArray(HIDE, moveArray)) {
			return HIDE;
		} else {
			return DOUBLE_BACK_1;
		}
	}
	int numReturnedLocs = 0;
	bool f = false;
	PlaceId *locationHistory = DvGetPlayerHistory(dv, PLAYER_DRACULA, 5, &numReturnedLocs, &f);
	
	int i = 0;
	while (locationHistory[i]) {
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