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

void decideHunterMove(HunterView hv)
{
	int pathLength = 0;
	char* abbv;
	Round round = 0;
	
	if (HvGetLastKnownDraculaLocation(hv, &round) != NOWHERE) {
		PlaceId *path = HvGetShortestPathTo(hv,HvGetPlayer(hv), 
		HvGetLastKnownDraculaLocation(hv, &round), &pathLength);
		abbv = placeIdToAbbrev(path[0]);	
	} 
	else {
		PlaceId *path = HvGetShortestPathTo(hv,HvGetPlayer(hv), 
		CASTLE_DRACULA, &pathLength);
		abbv = placeIdToAbbrev(path[0]);	
	}

	registerBestPlay(abbv, "lol");
}
