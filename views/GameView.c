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
#include "trap.h"
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
	Message message;
	Map map;
	int score;
	int turn;
	Round round;
	Trail trail;		// a queue of all dracula's trails and traps
	Hunter Lord_Godalming;
	Hunter Dr_Seward;
	Hunter Van_Helsing;
	Hunter Mina_Harker;
	Dracula Dracula;
	int *numTraps;
} ;


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
	int total_turns = 0;
	new->trail = newTrail();
	new->map = MapNew();
	new->score = GAME_START_SCORE;

	new->Lord_Godalming.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Dr_Seward.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Van_Helsing.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Mina_Harker.health = GAME_START_HUNTER_LIFE_POINTS;
	new->Dracula.health = GAME_START_BLOOD_POINTS;

	char s[10000];
    strcpy(s, pastPlays);
    char *token = strtok(s, " ");
    while (token != NULL){
        int cmp = strncmp(token, "D", 1);
        if (cmp == 0){									// Dracula past moves
            char abbv[3];		
            abbv[0] = token[1];
            abbv[1] = token[2];
            abbv[2] = '\0';

			char dEncounter[3];
			dEncounter[0] = token[3];
			dEncounter[1] = token[4];
			dEncounter[2] = '\0';

			char dAction = token[5];

            PlaceId placeid = placeAbbrevToId(abbv); 
            TrailJoin(new->trail, placeid);
			if (TrailLength(new->trail) >= TRAIL_SIZE) {
				TrailLeave(new->trail);
			}
			if (abbv == "TP") {
				new->Dracula.place = CASTLE_DRACULA;
				new->Dracula.health += LIFE_GAIN_CASTLE_DRACULA;
			}
			if (dEncounter[0] == 'T') {
				TrailJoin(new->trail, NORMAL_TRAP, new->Dracula.place);
			}
			else if (dEncounter[1] == 'V') {
				TrailJoin(new->trail, IMMATURE_VAMPIRE, new->Dracula.place);
			}

			if (dAction != '.') { // trap expired / vampire matured
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

			char hTrap = token[3];
			char hVamp = token[4];
			char hDrac = token[5];

			if (hTrap == 'T') {		
				TrapRemove(new->trail, player.place);
				player.health -= LIFE_LOSS_TRAP_ENCOUNTER;
			}
			if (hVamp = 'V') {
				int trap = TrapRemove(new->trail, player.place);
				
			}
			if (hDrac = 'D') { // hunter encounters dracula
				player.health -= LIFE_LOSS_DRACULA_ENCOUNTER;
				new->Dracula.health -= LIFE_LOSS_DRACULA_ENCOUNTER;
			}
            
        }
		total_turns++;
        token = strtok(NULL, " ");
    }
	new->round = total_turns/NUM_PLAYERS;
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
	return gv->round;
}

Player GvGetPlayer(GameView gv)
{
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
	PlaceId location;
	if (gv->round == 1) {
		if (player == gv->Lord_Godalming.id /*&& location is HOSPITAL*/) return gv->Lord_Godalming.place;			
		if (player == gv->Dr_Seward.id /*&& location is HOSPITAL*/) return gv->Dr_Seward.place;		
		if (player == gv->Van_Helsing.id /*&& location is HOSPITAL*/) return gv->Van_Helsing.place;		
		if (player == gv->Mina_Harker.id /*&& location is HOSPITAL*/) return gv->Mina_Harker.place;	
	    if (player == gv->Dracula.id /*&& location is CASTLE/*/) return gv->Dracula.place;	
	}	
	return NOWHERE;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	return GetVampireLocation(gv->trail);
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	*numTraps = TrapListLength(gv->traps);
	return getTrapsLocation(gv->trail);
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
	if (player == gv->Dracula.id) return "D";
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
