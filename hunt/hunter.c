////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "hunter.h"
#include "HunterView.h"
#include "Places.h"

PlaceId *getGoodPlays(PlaceId *currPr, PlaceId *pr1, PlaceId *pr2, PlaceId *pr3);




void decideHunterMove(HunterView hv)
{
	if (HvGetRound(hv) == 0) {
    		if (HvGetPlayer(hv) == PLAYER_LORD_GODALMING) registerBestPlay("MA", "start game");
    		else if (HvGetPlayer(hv) == PLAYER_DR_SEWARD) registerBestPlay("PA", "start game");
    		else if (HvGetPlayer(hv) == PLAYER_VAN_HELSING) registerBestPlay("BR", "start game");
    		else if (HvGetPlayer(hv) == PLAYER_MINA_HARKER) registerBestPlay("SO", "start game");
    		return;
	}

	// rest if hp <= 4
	if (HvGetHealth(hv,HvGetPlayer(hv)) <= 4) {
		int movelocation= HvGetPlayerLocation(hv,HvGetPlayer(hv));
		registerBestPlay((char *)placeIdToAbbrev(movelocation), "rest");
		return;
	}


	int pathLength = 0;
	char* abbv;
	Round round = 0;
	
	if (HvGetLastKnownDraculaLocation(hv, &round) != NOWHERE) {
		PlaceId *path = HvGetShortestPathTo(hv,HvGetPlayer(hv), 
			HvGetLastKnownDraculaLocation(hv, &round), &pathLength);
		abbv = placeIdToAbbrev(path[0]);	
	
	} else {
		// if dk where dracula is, hunters go around search
		// and avoid goes to the same position to each other
		int numReturnedMovesLord = 0;
		PlaceId *lord = HvWhereCanTheyGo(hv, PLAYER_LORD_GODALMING, &numReturnedMovesLord);
		int numReturnedMovesDr = 0;
		PlaceId *dr = HvWhereCanTheyGo(hv, PLAYER_DR_SEWARD, &numReturnedMovesDr);
		int numReturnedMovesVan = 0;
		PlaceId *van = HvWhereCanTheyGo(hv, PLAYER_VAN_HELSING, &numReturnedMovesVan);
		int numReturnedMovesMina = 0;
		PlaceId *mina = HvWhereCanTheyGo(hv, PLAYER_MINA_HARKER, &numReturnedMovesMina);

		PlaceId *goodPlays = NULL;
		if (HvGetPlayer(hv) == PLAYER_LORD_GODALMING) {goodPlays = getGoodPlays(lord, dr, van, mina);}
		else if (HvGetPlayer(hv) == PLAYER_DR_SEWARD) {goodPlays = getGoodPlays(dr, lord, van, mina);}
		else if (HvGetPlayer(hv) == PLAYER_VAN_HELSING) {goodPlays = getGoodPlays(van, dr, lord, mina);}
		else if (HvGetPlayer(hv) == PLAYER_MINA_HARKER) {goodPlays = getGoodPlays(mina, dr, van, lord);}


		// If goodPlays is empty, rest
		if (goodPlays == NULL) {
			int movelocation= HvGetPlayerLocation(hv,HvGetPlayer(hv));
			registerBestPlay((char *)placeIdToAbbrev(movelocation), "rest");
		} else {
			PlaceId *path = HvGetShortestPathTo(hv,HvGetPlayer(hv), 
				goodPlays[0], &pathLength);
			abbv = placeIdToAbbrev(path[0]);	
		}
	}

	registerBestPlay(abbv, "lol");
}




// return an array that the current hunter can reach 
// without going into the position that other hunters can reach
PlaceId *getGoodPlays (PlaceId *currPr, PlaceId *pr1, PlaceId *pr2, PlaceId *pr3) {
	PlaceId *goodPlays = malloc(sizeof(int));
	int size = 0;
	int dupe = 0;
	for (int i = 0; currPr[i]; i++){
		for (int j = 0; pr1[j]; j++){
			if (pr1[j] == currPr[i]) {
				dupe = 1;
			}
		}
		for (int j = 0; pr2[j]; j++){
			if (pr3[j] == currPr[i]) {
				dupe = 1;
			}
		}
		for (int j = 0; pr3[j]; j++){
			if (pr3[j] == currPr[i]) {
				dupe = 1;
			}
		}

		if (dupe == 0) {
			size++;
			goodPlays = realloc(goodPlays, size);
			goodPlays[size] = currPr[i];
		}
	}
	return goodPlays;
}
