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

void decideDraculaMove(DraculaView dv)
{
	
	int numReturnedMoves = 0;
	PlaceId *playable = DvGetValidMoves(dv, &numReturnedMoves);

	if (numReturnedMoves == 0) {
		registerBestPlay("TP", "lol");
		return;
	}

	
	int numReturnedMovesLord = 0;
	PlaceId *lord = DvWhereCanTheyGo(dv, PLAYER_LORD_GODALMING, &numReturnedMovesLord);
	int numReturnedMovesDr = 0;
	PlaceId *dr = DvWhereCanTheyGo(dv, PLAYER_DR_SEWARD, &numReturnedMovesDr);
	int numReturnedMovesVan = 0;
	PlaceId *van = DvWhereCanTheyGo(dv, PLAYER_VAN_HELSING, &numReturnedMovesVan);
	int numReturnedMovesMina = 0;
	PlaceId *mina = DvWhereCanTheyGo(dv, PLAYER_MINA_HARKER, &numReturnedMovesMina);

	PlaceId *goodPlays = malloc(sizeof(int));
	int size = 0;
	int dupe = 0;
	for (int i = 0; playable[i]; i++){
		for (int j = 0; lord[j]; j++){
			if (lord[j] == playable[i]) {
				dupe = 1;
			}
		}
		for (int j = 0; dr[j]; j++){
			if (dr[j] == playable[i]) {
				dupe = 1;
			}
		}
		for (int j = 0; van[j]; j++){
			if (van[j] == playable[i]) {
				dupe = 1;
			}
		}
		for (int j = 0; mina[j]; j++){
			if (mina[j] == playable[i]) {
				dupe = 1;
			}
		}
		if (dupe == 0) {
			size++;
			goodPlays = realloc(goodPlays, size);
			goodPlays[size] = playable[i];
		}
	}


	if (size == 0) {
		registerBestPlay(placeIdToAbbrev(playable[0]), "lol");
	} else {
		registerBestPlay(placeIdToAbbrev(goodPlays[0]), "lol");
	}
	
}






