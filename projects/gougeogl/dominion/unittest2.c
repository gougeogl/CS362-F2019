/***************************************************************
** FileName: unittest2.c
** Author: Glen Gougeon
** Class : CS362 Software Engineering II
** Date : 10 - 25 - 2019
** Last Mod : 11 - 8 - 2019
*
** Description : Assignment 3 : Unit Testing :
*		Refactored code for Minion, has 2 bugs I introduced.
*		The following test code should catch the errors while
*		executing 80 % branch and function coverage according
*		to gcov coverage tool.
*
** To Compile:	Use included 'Makefile'
*		compile command : make unittest2
*		generate output file: make unittestresults
*		output: unittestresults.out
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

// TO PRINT RULES FOR TESTS
#define RULES 0 /* <========= SET TO 1 TO PRINT RULES FOR TESTS & RELATION TO BUGS I INTRODUCED !! */

// TEST PROTO-TYPES
void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game);
void prepMinion(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card);

// HELPER PROTOS-TYPES
void wipeDeck(struct gameState* state);
void wipeDiscard(struct gameState* state);

void resetHand(struct gameState* dState);
void setHandCount(struct gameState* state, int newHandSize);
void setHandPos(struct gameState* state, int card, int handPos);

/* This function adds memset initialization, and prints error if fail  */
void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game)
{
	memset(game, '\0', sizeof(struct gameState));   // clear mem of 

	int checkInit = initializeGame(players, kDeck, mySeed, game); // initialize 2 player game 

	if (checkInit == -1)
	{
		printf("Bad game initialization.\n");
	}

}

void prepMinion(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card)
{
	initTestGame(players, kingdom, seed, state);
	//removeEstatesDeck(state);
	wipeDeck(state);
	wipeDiscard(state);
	updateCoins(player, state, 0); /* <== added this */
	resetHand(state);
	setHandCount(state, handSize);
	setHandPos(state, card, handPos);
	state->coins = 0;
}

// set player to remove all estates from current player's deck  
void wipeDeck(struct gameState* state)
{
	int currentPlayer = whoseTurn(state);

	int i = 0;
	while (i < state->deckCount[currentPlayer])
	{
		state->deck[currentPlayer][i] = -1;
		i++;
	}
	state->deckCount[currentPlayer] = 0;
}

// eliminate all estates from discard.   
void wipeDiscard(struct gameState* state)
{
	int currentPlayer = whoseTurn(state);

	int i = 0;
	while (i < state->discardCount[currentPlayer])
	{
		state->discard[currentPlayer][i] = -1;
		i++;
	}
	state->discardCount[currentPlayer] = 0;
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
	}
	dState->handCount[currentPlayer] = 0;
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