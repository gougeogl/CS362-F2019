/**************************************************
* FileName: testInitGame.c
* Author:	Glen Gougeon
* Date:		11-5-2019
* Last Mod: 11-5-2019
*
* Description:	Program to test basic assumptions 
*				about how initializeGame().
*
**************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed,
	struct gameState *state) */

/* NOTES: *************************************
	-numPlayers might change 
	-kingdomCards will NOT change
	-randomSeed will probably not change ?? 
**********************************************/

void printAll(struct gameState* state);

int main()
{
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine, gardens, remodel, smithy };

	// Set up Game State 
	struct gameState G;

	int seed = 1;
	int check_init_game_return = -1;

	/* CHECK TO ENSURE BAD INIT WITH WRONG # OF PLAYERS */
	int i; 
	for (i = -1; i < 2; i++)
	{
		memset(&G, '\0', sizeof(struct gameState));   // clear mem of 

		check_init_game_return = initializeGame(-1, kingdomCards, seed, &G);
		if (check_init_game_return == -1)
		{
			printf("check_init_game_return was: %d\n", check_init_game_return);
			printf("Number of players: %d\n", i);
			fflush(stdout);
			fprintf(stderr, "%s\n", "Bad initializeGame()");
		}
	}

	memset(&G, '\0', sizeof(struct gameState));   // clear mem of 
	check_init_game_return = initializeGame(5, kingdomCards, seed, &G);
	if (check_init_game_return == -1)
	{
		printf("check_init_game_return was: %d\n", check_init_game_return);
		printf("Number of players: 5\n");
		fflush(stdout);
		fprintf(stderr, "%s\n", "Bad initializeGame()");
	}
	printAll(&G);

	printf("TESTING VALID PLAYER\n");
	int j;
	for(j = 2; j <= MAX_PLAYERS; j++)
	{
		printf("TEST FOR %d PLAYERS\n\n", j );
		fflush(stdout);
		memset(&G, '\0', sizeof(struct gameState));   // clear mem of 
		check_init_game_return = initializeGame(j, kingdomCards, seed, &G);
		printf("NUMBER OF PLAYERS: %d\n", G.numPlayers );

		printAll(&G);

		printf("NEXT SET mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm\n");
		fflush(stdout);
	}	

	return 0;
}

void printAll(struct gameState* state)
{
	int i;
	for( i = 0; i < state->numPlayers; i++)
	{
		printf("NEW PLAYER STATS\n\n");
		printf("SUPPLY NOW:\n");
		printSupply(state);

		printf("\nplayer's %d hand count : %d\n", i, state->handCount[i] );
		printf("CURRENT PLAYER'S HAND:\n");
		printHand(i, state);

		printf("CURRENT PLAYER'S DECK:\n");
		printDeck(i, state);

		printf("CURRENT PLAYER'S DISCARD:\n");
		printDiscard(i, state);

		printf("CURRENT PLAYER'S CARDS PLAYED:\n");
		printPlayed(i, state);
		printf("played card count : %d\n", state->playedCardCount );
		printf("outpost played ?? : %d\n\n", state->outpostPlayed );

		printf("GAME STATE NOW:\n");
		printState(state);

		printf("CURRENT SCORES:\n");
		printScores(state);

	}
}
