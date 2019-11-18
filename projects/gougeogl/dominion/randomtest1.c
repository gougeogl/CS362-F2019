/***************************************************************
** FileName: randomtest1.c
** Author: Glen Gougeon
** Class : CS362 Software Engineering II
** Date : 11 - 14 - 2019
** Last Mod : 11 - 17 - 2019
*
** Description : Assignment 4 : Random Testing :
*		Refactored code for Baron, has 2 bugs I introduced.
*		The following test code should generate random value
*		inputs to send to baronCard() acheiving at least
*		90% branch coverage.
*
** To Compile:	Use included 'Makefile'
*		compile command : make unittest1
*		generate output file: make randomtestresults
*		output: randomtestresults.out
*
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"

#define DEBUG_RANDOM 0

// TEST PROTO-TYPES
void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game);
void prep(int numPlayers, int seed, int* kingdom, struct gameState* state, int numEstates, int handSize, int handPos, int card);

// selects a random card from kingdomCards deck 
int _rand_of_kingdomCards();

// randomly gen an int in range [min..max]
int _genRandRange(int min, int max);

// HELPER PROTOS-TYPES
void wipeDeck(struct gameState* state);
void removeEstatesDiscard(struct gameState *st);
void setEstateSupply(struct gameState *state, int qtyEstates);
void resetHand(struct gameState* dState);
void setHandCount(struct gameState* state, int newHandSize);
void setHandPos(struct gameState* state, int card, int handPos);
int getTopDiscard(struct gameState* state);
int getNumEstateSupply(struct gameState* state);
int getNumEstateHand(int player, struct gameState* state);

// TEST PROTO-TYPE
void randomBaronTest();

int main()
{
	time_t t = 0;
	unsigned seedling = time(&t);
	srand(seedling);
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("ASSIGNMENT 4: RANDOM TESTING: BARON CARD.\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	randomBaronTest();
	//printf("\n\n");
	return 0;
}

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

void prep(int numPlayers, int seed, int* kingdom, struct gameState* state, int numEstates, int handSize, int handPos, int card)
{
	initTestGame(numPlayers, kingdom, seed, state);
	// removeEstatesDeck(state);
	wipeDeck(state);

	// update coins all players
	int i;
	for (i = 0; i < state->numPlayers; i++)
	{
		updateCoins(i, state, 0); /* <== added this */
	}
	

	removeEstatesDiscard(state);
	setEstateSupply(state, numEstates); /* <-- add 1 estate to supply here */
	resetHand(state);
	setHandCount(state, handSize);
	setHandPos(state, card, handPos);
	state->coins = 0;
}

/* *************************************************************************
* random of kingdom cards
* generates a random choice out of a hard-coded set of kingdom cards
*
****************************************************************************/
int _rand_of_kingdomCards()
{
	int result = -1;

	result = rand() % (10 - 1 + 1) + 1;
	switch (result)
	{
	case 1:	result = adventurer;
		break;

	case 2: result = ambassador;
		break;

	case 3: result = baron;
		break;

	case 4: result = estate;
		break;

	case 5: result = tribute;
		break;

	case 6: result = minion;
		break;

	case 7: result = mine;
		break;

	case 8: result = gardens;
		break;

	case 9: result = remodel;
		break;

	case 10: result = smithy;
		break;

	}

	return result;
}

// generate random number according to input range
int _genRandRange(int min, int max)
{
	int num = rand() % (max - min + 1) + min;
	if (DEBUG_RANDOM)
	{
		printf("rand num: %d\n", num);
		fflush(stdout);
	}
	return num;
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
void removeEstatesDiscard(struct gameState *st)
{
	int currentPlayer = whoseTurn(st);
	int tally = 0;

	int i = 0;
	while (i < st->discardCount[currentPlayer])
	{
		// search for an estate in discard of current player    

		if (st->discard[currentPlayer][i] == estate)
		{
			// if found, wipe out, set to -1     
			st->discard[currentPlayer][i] = -1;
			tally++;
		}
		i++;
	}
	st->discardCount[currentPlayer] -= tally;
}

/* set new default estate supplyCount for testing */
void setEstateSupply(struct gameState *state, int qtyEstates)
{
	state->supplyCount[estate] = qtyEstates;
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

/* getter for top of currentPlayer's discard */
int getTopDiscard(struct gameState* state)
{
	int result = -1;
	int currentPlayer = state->whoseTurn;
	int top = state->discardCount[currentPlayer];
	top--; // must decrement b/c discarCard() increments internally

	if ((state->discard[currentPlayer][top] >= curse) && 
		(state->discard[currentPlayer][top] <= treasure_map)) {

		result = state->discard[currentPlayer][top];

		/*
		char name[MAX_STRING_LENGTH];
		memset(name, '\0', sizeof(name));
		cardNumToName(result, name);
		printf("TRACE: returning named: %s\n", name);
		printf("TRACE: getTopDiscard returning: %d\n", result);
		*/

	}
	return result;
}

int getNumEstateSupply(struct gameState* state)
{
	return state->supplyCount[estate];
}

int getNumEstateHand(int player, struct gameState* state)
{
	int tally = 0;

	int i;
	for (i = 0; i < state->handCount[player]; i++)
	{
		if (state->hand[player][i] == estate)
		{
			tally++;
		}
	}
	return tally;
}
/******************************************************
 * Description:
 * 	tests baronCard() function with random inputs
 *
 * Check against:
 * 	no increase in num buys
 * 		- displays difference
 * 	when discard an estate but,
 * 	not found in discard afterward
 *
 * ***************************************************/
void randomBaronTest()
{
	int numErrors = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState backup;

	printf("randomtest1.c\n");
	printf("RANDOM BARON TEST :\n");
	printf("OUTPUT:\n\n");

	do
	{
		// number of players
		int newNumPlayers = _genRandRange(2, MAX_PLAYERS);

		// seed [1..1000]
		int newSeed = _genRandRange(1, 1000);

		// number of estates in supply
		int newNumEstates = 0;

		if (newNumPlayers == 2)
		{
			newNumEstates = _genRandRange(0, 8);
		}
		else 
		{
			newNumEstates = _genRandRange(0, 12);
		}

		// size of hand to use
		int newHandSize = _genRandRange(0, MAX_HAND);

		// position to place a single kingdom card into hand
		int newHandPos = _genRandRange(0, newHandSize);

		// random kingdom card for specific deck
		int whichKcard = _rand_of_kingdomCards();

		// CALL PREP w/ above randomly generated values
		prep(
			newNumPlayers, 
			newSeed, 
			kingdomCards, 
			&G, 
			newNumEstates, 
			newHandSize, 
			newHandPos, 
			whichKcard
		);

		/* SAVE NUMBER OF ESTATES IN HAND, AND IN SUPPLY */
		int prevNumEstateHand = getNumEstateHand(G.whoseTurn, &G);
		int prevEstateSupply = getNumEstateSupply(&G);

		// make a backup for comparison
		memset(&backup, '\0', sizeof(backup));
		backup = G;

		int zero_or_one = _genRandRange(0, 1);

		/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */
		/* ** CALLING BARON CARD !! **															*/
		baronCard(zero_or_one, &G);
		/* MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM */

		/* check num buys */
		if ((backup.numBuys + 1) != G.numBuys)
		{
			printf("Error: Baron card played, but numBuys didn't increase.\n");
			printf("     : previous numBuys: %d\n", backup.numBuys);
			printf("     : current numBuys: %d\n", G.numBuys);
			numErrors++;	
		}

		if (	(zero_or_one == 1) &&		/* Chose 1: discard estate & gain +4 coin */
				(prevNumEstateHand > 0) &&  /* assert an estate was in hand */
				(prevEstateSupply > 0)	){  /* assert an estate was in supply */

			if (getTopDiscard(&G) != estate) /* ESTATE NOT FOUND */
			{
				char name[MAX_STRING_LENGTH];
				memset(name, '\0', sizeof(name));

				printf("Error: Chose to discard an estate, but no estate found in discard !\n");
				int currentPlayer = whoseTurn(&G);
				cardNumToName(G.discard[currentPlayer][G.discardCount[currentPlayer]], name);
				printf("Top of discard contains: %s\n", name);
				numErrors++;

				/* check coins increased */
				if (backup.coins + 4 == G.coins)
				{
					printf("Error: No estate in discard, but coins is +4 previous anyway !\n");
					printf("     : previous coins: %d\n", backup.coins);
					printf("     : current  coins: %d\n", G.coins);
					numErrors++;
				}
			}
			else if (getTopDiscard(&G) == estate)	/* FOUND ESTATE */
			{
				/* check coins increased */
				if (backup.coins + 4 != G.coins)
				{
					printf("Error: Estate in discard, but coins is not +4 previous !\n");
					printf("     : previous coins: %d\n", backup.coins);
					printf("     : current  coins: %d\n", G.coins);
					numErrors++;
				}
			}

		}
		else if (	(zero_or_one == 0) && 
					(prevEstateSupply > 0)	){

			if (getTopDiscard(&G) == estate)	/* FOUND ESTATE */
			{
				/* check coins increased */
				if (backup.coins + 4 == G.coins)
				{
					printf("Error: Illegal coin gain !\n");
					printf("     : Chose to gain an estate, but coins now +4 previous.\n");
					printf("     : previous coins: %d\n", backup.coins);
					printf("     : current  coins: %d\n", G.coins);
					numErrors++;
				}
			}
			else if (getTopDiscard(&G) != estate)	/* ESTATE NOT FOUND */
			{

				printf("Error: Chose to gain an estate, but no estate found in discard !\n");
				printf("     : supply count of estate was: %d\n", prevEstateSupply);
				numErrors++;
			}

		}

		if ((getTopDiscard(&G) == estate) && (prevEstateSupply < 1))
		{
			printf("Error: No estates in supply, but an estate found in discard !\n");

			/* check coins increased */
			if (backup.coins + 4 == G.coins)
			{
				printf("Error: Illegal coin gain !\n");
				printf("     : There were no estates AND coins now +4 previous.\n");
				printf("     : previous coins: %d\n", backup.coins);
				printf("     : current  coins: %d\n", G.coins);
				numErrors++;
			}
		}

	} while( numErrors < 5 );
}

