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
#include "Trap.h"
#include "Queue.h"
#include "Places.h"
// add your own #includes here

#define ALL_TRAPS 0
#define HUNTER 		100
#define DRACULA 	200


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


struct gameView {
	char *pastPlays;
	Map map;
	int score;
	int turn;
	Trail trail;		// a queue of all dracula's trails and traps
	Hunter Lord_Godalming;
	Hunter Dr_Seward;
	Hunter Van_Helsing;
	Hunter Mina_Harker;
	Dracula Dracula;
	int numTraps;
};


char *playerToLetter(GameView gv, Player player);
////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[])
{
	GameView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}
	new->turn = 0;
	new->trail = newTrail();
	new->map = MapNew();
	new->score = GAME_START_SCORE;

	new->Lord_Godalming.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Dr_Seward.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Van_Helsing.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Mina_Harker.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Dracula.health = GAME_START_BLOOD_POINTS;


	new->Lord_Godalming.id = PLAYER_LORD_GODALMING;
	new->Dr_Seward.id = PLAYER_DR_SEWARD;
	new->Van_Helsing.id = PLAYER_VAN_HELSING;
	new->Mina_Harker.id = PLAYER_MINA_HARKER;
	new->Dracula.id = PLAYER_DRACULA;
	

	new->Lord_Godalming.place = NOWHERE;
	new->Dr_Seward.place = NOWHERE;
	new->Van_Helsing.place = NOWHERE;
	new->Mina_Harker.place = NOWHERE;
	new->Dracula.place = NOWHERE;

	char s[10000];
    strcpy(s, pastPlays);
    char *token = strtok(s, " ");

    while (token != NULL){

        if (token[0] == 'D') {									// Dracula past moves
            char abbv[3];		
            abbv[0] = token[1];
            abbv[1] = token[2];
            abbv[2] = '\0';

			char dEncounter[3];
			dEncounter[0] = token[3];
			dEncounter[1] = token[4];
			dEncounter[2] = '\0';

			char dAction = token[5];


			if (strcmp(abbv,"TP") == 0) {						// TP move
				new->Dracula.place = CASTLE_DRACULA;
				new->Dracula.health += LIFE_GAIN_CASTLE_DRACULA;
			} else if (abbv[0] == 'D') {						// Double Back move
				int trailposition = abbv[1] - '0';				// convert char to int (atoi doesn't work)
				PlaceId location = getDBTrailPosition(new->trail, trailposition);
				new->Dracula.place = location;
			} else {											// is a place
				PlaceId placeid = placeAbbrevToId(abbv); 
				new->Dracula.place = placeid;
			}


			if (dEncounter[0] == 'T') {
				TrailJoin(new->trail, NORMAL_TRAP, new->Dracula.place);
			}
			else if (dEncounter[1] == 'V') {
				TrailJoin(new->trail, IMMATURE_VAMPIRE, new->Dracula.place);
			}

			if (dAction != '.') { // trap expired / vampire matured i.e. trail too long
				TrapId type = TrailLeave(new->trail);
				if (type == IMMATURE_VAMPIRE) new->score -= SCORE_LOSS_VAMPIRE_MATURES;
			}
	            
            
        } else {										// Hunter past moves
			Hunter player;
			if (token[0] == 'G') player =  new->Lord_Godalming; 
			if (token[0] == 'S') player =  new->Dr_Seward; 
			if (token[0] == 'H') player =  new->Van_Helsing; 
			if (token[0] == 'M') player =  new->Mina_Harker; 

            char abbv[3];
            abbv[0] = token[1];
            abbv[1] = token[2];
            abbv[2] = '\0';

            PlaceId placeid = placeAbbrevToId(abbv); 
			player.place = placeid;	

			char hTrap = token[3];
			char hVamp = token[4];
			char hDrac = token[5];

			if (hTrap == 'T') {		
				TrapRemove(new->trail, player.place);
				player.health -= LIFE_LOSS_TRAP_ENCOUNTER;
				if (player.health <= 0) {
					new->score -= SCORE_LOSS_HUNTER_HOSPITAL;
					player.health = GAME_START_HUNTER_LIFE_POINTS;
					player.place = HOSPITAL_PLACE;
				}
			}
			if (hVamp == 'V') {
				TrapRemove(new->trail, player.place);
				
			}
			if (hDrac == 'D') { // hunter encounters dracula
				player.health -= LIFE_LOSS_DRACULA_ENCOUNTER;
				new->Dracula.health -= LIFE_LOSS_DRACULA_ENCOUNTER;
				if (player.health <= 0) {
					new->score -= SCORE_LOSS_HUNTER_HOSPITAL;
					player.health = GAME_START_HUNTER_LIFE_POINTS;
					player.place = HOSPITAL_PLACE;
				}
			}
            
        }
		new->turn++;			// whose turn is it currently
        token = strtok(NULL, " ");
    }
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
	return gv->turn / 5;
}

Player GvGetPlayer(GameView gv)
{
	int player = gv->turn;		
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
	return gv->score;
}

int GvGetHealth(GameView gv, Player player)
{
	int health;

	if (gv->Lord_Godalming.id == player) {
		health = gv->Lord_Godalming.health;
	} else if (gv->Dr_Seward.id == player) {
		health = gv->Dr_Seward.health;
	} else if (gv->Van_Helsing.id == player) {
		health = gv->Van_Helsing.health;
	} else if (gv->Mina_Harker.id == player) {
		health = gv->Mina_Harker.health;
	} else if (gv->Dracula.id == player) {
		health = gv->Dracula.health;
	} 

	return health;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{

	if (player == gv->Lord_Godalming.id) return gv->Lord_Godalming.place;			
	if (player == gv->Dr_Seward.id) return gv->Dr_Seward.place;		
	if (player == gv->Van_Helsing.id) return gv->Van_Helsing.place;	
	if (player == gv->Mina_Harker.id) return gv->Mina_Harker.place;	

	return gv->Dracula.place;	

}

PlaceId GvGetVampireLocation(GameView gv)
{
	return GetVampireLocation(gv->trail);
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	*numTraps = TotalTrapsTrail(gv->trail);
	return getTrapsLocations(gv->trail);
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	PlaceId *history;
	int numMoves = 0;
	char s[10000];
    strcpy(s, gv->pastPlays);
    char *token = strtok(s, " ");
	char *letter = playerToLetter(gv, player);
    while (token != NULL){
			if (strncmp(token, letter, 1) == 0) {
			char abbv[3];
			abbv[0] = token[1];
			abbv[1] = token[2];
			abbv[2] = '\0';
			PlaceId move = placeAbbrevToId(abbv); 
			history = realloc(history, (numMoves + 1) * sizeof(*history));
			history[numMoves] = move;
			numMoves++;
		}
        token = strtok(NULL, " ");
    }

	*numReturnedMoves = numMoves;
	*canFree = false;
	return history;
}

char *playerToLetter(GameView gv, Player player)
{
	if (player == gv->Lord_Godalming.id) return "G";			
	if (player == gv->Dr_Seward.id) return "S";		
	if (player == gv->Van_Helsing.id) return "H";		
	if (player == gv->Mina_Harker.id) return "M";	

	return "D";
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	PlaceId *history;
	int num = 0;
	char s[10000];
    strcpy(s, gv->pastPlays);
    char *token = strtok(s, " ");
	char *letter = playerToLetter(gv, player);
    while (token != NULL && num < numMoves){
			if (strncmp(token, letter, 1) == 0) {
			char abbv[3];
			abbv[0] = token[1];
			abbv[1] = token[2];
			abbv[2] = '\0';
			PlaceId move = placeAbbrevToId(abbv); 
			history = realloc(history, (numMoves + 1) * sizeof(*history));
			history[numMoves] = move;
			num++;
		}
        token = strtok(NULL, " ");
    }

	*numReturnedMoves = num;
	*canFree = false;
	return history;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	if (player != PLAYER_DRACULA) {
		return GvGetMoveHistory(gv, player, numReturnedLocs, canFree);
	}
	PlaceId *history;
	int numLocs = 0;
	char s[10000];
    strcpy(s, gv->pastPlays);
    char *token = strtok(s, " ");
    while (token != NULL){
		if (strncmp(token, "D", 1) == 0) {
			char abbv[3];
			abbv[0] = token[1];
			abbv[1] = token[2];
			abbv[2] = '\0';
			PlaceId move = placeAbbrevToId(abbv); 

			if(move == HIDE){
				history = realloc(history, (numLocs + 1) * sizeof(*history));
				history[numLocs] = history[numLocs - 1];
				numLocs++;
			} else if (move > HIDE && move < TELEPORT) {
				int back = move - HIDE;
				history = realloc(history, (numLocs + 1) * sizeof(*history));
				history[numLocs] = history[numLocs - back];
				numLocs++;
			} else if (move == TELEPORT) {
				history = realloc(history, (numLocs + 1) * sizeof(*history));
				history[numLocs] = CASTLE_DRACULA;
				numLocs++;
			} else {
				history = realloc(history, (numLocs + 1) * sizeof(*history));
				history[numLocs] = move;
				numLocs++;
			}
		}
		
		token = strtok(NULL, " ");
	}		

	*numReturnedLocs = numLocs;
	*canFree = false;
	return history;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	if (player != PLAYER_DRACULA) {
		return GvGetLastMoves(gv, player, numLocs, numReturnedLocs, canFree);
	}
	PlaceId *history;
	int num = 0;
	char s[10000];
    strcpy(s, gv->pastPlays);
    char *token = strtok(s, " ");
    while (token != NULL && num < numLocs){
		if (strncmp(token, "D", 1) == 0) {
			char abbv[3];
			abbv[0] = token[1];
			abbv[1] = token[2];
			abbv[2] = '\0';
			PlaceId move = placeAbbrevToId(abbv); 

			if(move == HIDE){
				history = realloc(history, (numLocs + 1) * sizeof(*history));
				history[numLocs] = history[numLocs - 1];
				numLocs++;
			} else if (move > HIDE && move < TELEPORT) {
				int back = move - HIDE;
				history = realloc(history, (numLocs + 1) * sizeof(*history));
				history[numLocs] = history[numLocs - back];
				numLocs++;
			} else if (move == TELEPORT) {
				history = realloc(history, (numLocs + 1) * sizeof(*history));
				history[numLocs] = CASTLE_DRACULA;
				numLocs++;
			} else {
				history = realloc(history, (numLocs + 1) * sizeof(*history));
				history[numLocs] = move;
				numLocs++;
			}
		}
		
		token = strtok(NULL, " ");
	}		

	*numReturnedLocs = numLocs;
	*canFree = false;
	return history;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	*numReturnedLocs = 0;
	
	if (player == PLAYER_LORD_GODALMING ||
		player == PLAYER_DR_SEWARD ||
		player == PLAYER_MINA_HARKER ||
		player == PLAYER_VAN_HELSING) {
		return MapGetHunterReachable (gv->map, from, HUNTER, 
			player, round, numReturnedLocs,
			true, true, true);
	}

	if (player == PLAYER_DRACULA) {
		return MapGetDraculaReachable (gv->map, from, DRACULA, 
			numReturnedLocs, true, false, true);
	}

	return NULL;
}



PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	*numReturnedLocs = 0;

	if (player == PLAYER_LORD_GODALMING ||
		player == PLAYER_DR_SEWARD ||
		player == PLAYER_MINA_HARKER ||
		player == PLAYER_VAN_HELSING) {
		return MapGetHunterReachable (gv->map, from, HUNTER, 
			player, round, numReturnedLocs,
			road, rail, boat);
	}

	if (player == PLAYER_DRACULA) {
		return MapGetDraculaReachable (gv->map, from, DRACULA, 
			numReturnedLocs, road, false, boat);
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions
