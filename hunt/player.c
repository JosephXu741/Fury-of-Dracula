////////////////////////////////////////////////////////////////////////
// Runs a player's game turn ...
//
// Can  produce  either a hunter player or a Dracula player depending on
// the setting of the I_AM_DRACULA #define
//
// This  is  a  dummy  version of the real player.c used when you submit
// your AIs. It is provided so that you can test whether  your  code  is
// likely to compile ...
//
// Note that this is used to drive both hunter and Dracula AIs. It first
// creates an appropriate view, and then invokes the relevant decideMove
// function,  which  should  use the registerBestPlay() function to send
// the move back.
//
// The real player.c applies a timeout, and will halt your  AI  after  a
// fixed  amount of time if it doesn 't finish first. The last move that
// your AI registers (using the registerBestPlay() function) will be the
// one used by the game engine. This version of player.c won't stop your
// decideMove function if it goes into an infinite loop. Sort  that  out
// before you submit.
//
// Based on the program by David Collien, written in 2012
//
// 2017-12-04	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.3	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#define I_AM_DRACULA 1
#ifdef I_AM_DRACULA
# include "dracula.h"
# include "DraculaView.h"
#else
# include "hunter.h"
# include "HunterView.h"
#endif

// Moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

// A pseudo-generic interface, which defines
// - a type `View',
// - functions `ViewNew', `decideMove', `ViewFree',
// - a trail `xtrail', and a message buffer `xmsgs'.
#ifdef I_AM_DRACULA

typedef DraculaView View;

# define ViewNew DvNew
# define decideMove decideDraculaMove
# define ViewFree DvFree

# define xPastPlays "GGA.... SRO.... HBA.... MBU.... DAL.V.. GBC.... SBI.... HALVD.. MPA.... DGRT... GBE.... SNP.... HMA.... MBO.... DCAT... GSA.... STS.... HAL.... MBA.... DAO.... GIO.... SMS.... HGRT... MAL.... DBB.... GTS.... SAL.... HGR.... MGR.... DBOT... GCG.... SAL.... HGR.... MMA.... DCFT... GCG.... SAL.... HGR.... MAL.... DGET... GCG.... SAL.... HAL.... MAL.... DMRT.M. GCG.... SAL.... HAL.... MAL.... DGOT... GCG.... SAL.... HAL.... MAL.... DFLT... GCG.... SAL.... HAL.... MAL.... DROT.M. GCG.... SAL.... HAL.... MAL.... DBIT.M. GCG.... SAL.... HAL.... MAL.... DD1.VM. GCG.... SAL.... HAL.... MAL...."
# define xMsgs { "", "", "", "" }

#else

typedef HunterView View;

# define ViewNew HvNew
# define decideMove decideHunterMove
# define ViewFree HvFree

# define xPastPlays "GMA.... SPA.... HBR.... MSO.... DIO.... GMA.... SGE.... HBR.... MSO.... DAS.... GMA.... SGE.... HBR.... MSO.... DBIT... GMA.... SGE.... HBR.... MSO.... DD2.... GMA.... SGE.... HBR.... MSO.... DVET... GMA.... SGE.... HBR.... MSO.... DMIT... GMA.... SGE.... HBR.... MSO.... DGOT... GMA.... SGE.... HBR.... MSO.... DHIT... GMA.... SGE.... HBR.... MSO.... DMRT.M. GMA.... SGE.... HBR.... MSO.... DCFT... GMA.... SGE.... HBR...."
# define xMsgs { "", "", "" }

#endif

int main(void)
{
	char *pastPlays = xPastPlays;
	Message msgs[] = xMsgs;
	View state = ViewNew(pastPlays, msgs);
	decideMove(state);
	ViewFree(state);

	printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
	return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay(char *play, Message message)
{
	strncpy(latestPlay, play, MOVE_SIZE - 1);
	latestPlay[MOVE_SIZE - 1] = '\0';

	strncpy(latestMessage, message, MESSAGE_SIZE - 1);
	latestMessage[MESSAGE_SIZE - 1] = '\0';
}
