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

/* int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed,
	struct gameState *state) */

/* NOTES: *************************************
	-numPlayers might change 
	-kingdomCards will NOT change
	-randomSeed will probably not change ?? 
**********************************************/

/* void cardNumToName(int card, char *name) */

void initTestGame(struct gameState* game, int* kDeck, int mySeed);
void get_stats_before_call(struct gameState* my_game, int my_arr[], int card);

int main()
{
	int overall_stat = -1;
	int check = -1;

	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Set up Game State 
	struct gameState G;

	int seed = 1;

	int stats[13];

	/* CHECK TO ENSURE BAD INIT WITH WRONG # OF PLAYERS */
	int i, check_init_game_return = -1;
	for (i = -1, i < 2; i++)
	{
		int check_init_game_return = initializeGame(-1, kingdomCards, seed, &G);
		if (check_init_game_return == -1)
		{
			printf("Number of players: %d\n", i);
			fprintf(stderr, "%s\n", "Bad initializeGame()");
		}
	}
	check_init_game_return = initializeGame(5, kingdomCards, seed, &G);
	if (check_init_game_return == -1)
	{
		printf("Number of players: 5\n");
		fprintf(stderr, "%s\n", "Bad initializeGame()");
	}


	// checks G is initialized properly.. calls initializeGame() from dominion.h
	//initTestGame(&G, kingdomCards, seed);

	// collect all info. of gameState before call
	get_stats_before_call(&G, stats, estate);

	return 0;
}
/* ** TEST SUITE FUNCTIONS *************************************************************/
void initTestGame(struct gameState* game, int* kDeck, int mySeed)
{
	memset(game, '\0', sizeof(struct gameState));   // clear mem of 

	int checkInit = initializeGame(2, kDeck, mySeed, game); // initialize 2 player game 

	if (checkInit == -1)
	{
		printf("Bad game initialization.\n");
	}

}

void get_stats_before_call(struct gameState* my_game, int my_arr[], int card)
{
	int currentPlayer = whoseTurn(my_game);

	my_arr[0] = my_game->supplyCount[card]; /* how many of card left in supply */
	my_arr[1] = my_game->playedCards[my_game->playedCardCount]; /* # cards played */
	my_arr[2] = my_game->playedCardCount;  /* how many cards played */

	my_arr[3] = my_game->numBuys; /* buys */
	my_arr[4] = my_game->numActions; /* actions */
	my_arr[5] = my_game->coins; /* coins */


	my_arr[6] = my_game->trashPile[my_game->trashCount]; /* top of trash */
	my_arr[7] = my_game->trashCount; /* trash size */

	my_arr[8] = my_game->discard[currentPlayer][my_game->discardCount[currentPlayer]]; /* top of discard */
	my_arr[9] = my_game->discardCount[currentPlayer]; /* discard count */

	my_arr[10] = my_game->deck[currentPlayer][my_game->discardCount[currentPlayer]]; /* top of deck */
	my_arr[11] = my_game->deckCount[currentPlayer]; /* deck count */

	my_arr[12] = my_game->handCount[currentPlayer]; /*  hand count */

}

void printStats(struct gameState* state)
{

}