/****************************************************************
** FileName: testDiscardCard.c
** Author: Glen Gougeon
** Class: CS362 Software Engineering II
** Date: 11-6-2019
** Last Mod: 11-6-2019
*
** Description:	Assignment 3: Unit Testing :
*		testing refactored discardCard() function for coverage
*
****************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"

void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game);

/* HAND RELATED */
void resetHand(struct gameState* dState);
void setHandCount(struct gameState* state, int newHandSize);
void setHandPos(struct gameState* state, int card, int handPos);

void printStats(int player, struct gameState* state);
/* int discardCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag) */

int main()
{
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Set up Game State 
	int seed = 1;
	struct gameState G;
	initTestGame(2, kingdomCards, seed, &G);
	
	int currentPlayer = whoseTurn(&G);

	printf("TESTING STATS AFTER INITIALIZATION\n");
	printStats(currentPlayer , &G);	

	printf("TESTING STATS AFTER DISCARD CARD POS 0\n");
	int check = discardCard(0, currentPlayer, &G);	
	printf("RETURNED: %d\n", check);
	printStats(currentPlayer, &G);


	return 0;
}

/* This function adds memset initialization, and prints error if fail  */
void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game)
{
	memset(game, '\0', sizeof(struct gameState));   // clear mem of 

	int checkInit = initializeGame(2, kDeck, mySeed, game); // initialize 2 player game 

	if (checkInit == -1)
	{
		printf("Bad game initialization.\n");
	}

}

/* Sets current player's handCount to newHandSize, then
	overwrites everything in hand with -1 */
void resetHand(struct gameState* dState)
{
	int currentPlayer = whoseTurn(dState);

	int i;
	for (i = 0; i < dState->handCount[currentPlayer]; i++)
	{
		dState->hand[currentPlayer][i] = -1;
		dState->handCount[currentPlayer] -= 1;
	}
}

void setHandCount(struct gameState* state, int newHandSize)
{
	int currentPlayer = whoseTurn(state);
	state->handCount[currentPlayer] = newHandSize;
}

/* adds indicated card in current player's hand at handPos */
void setHandPos(struct gameState* state, int card, int handPos)
{
	int currentPlayer = whoseTurn(state);
	state->hand[currentPlayer][handPos] = card;
}

void printStats(int player, struct gameState* state)
{
	printf("SUPPLY CONTENTS:\n");
	printSupply(state);

	printf("Current player: %d\n", player);
	printDeck(player, state);
	printDiscard(player, state);

	printHand(player, state);
}
