/***************************************************************
** FileName: unittest1.c
** Author: Glen Gougeon
** Class : CS362 Software Engineering II
** Date : 10 - 25 - 2019
** Last Mod : 11 - 23 - 2019
*
** Description : Assignment 5 : Unit Testing :
*		Refactored code for Baron by Reuben Youngblom.
*		The following test code should catch the errors while
*		executing 80 % branch and function coverage according
*		to gcov coverage tool.
*
** To Compile:	Use included 'Makefile'
*		compile command : make unittest1
*		generate output file: make unittest1results
*		output: unittest1results.out
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
void prep(int player, int seed, int* kingdom, struct gameState* state, int numEstates, int handSize, int handPos, int card);

// HELPER PROTOS-TYPES
void wipeDeck(struct gameState* state);
void removeEstatesDiscard(struct gameState *st);
void setEstateSupply(struct gameState *state, int qtyEstates);
void resetHand(struct gameState* dState);
void setHandCount(struct gameState* state, int newHandSize);
void setHandPos(struct gameState* state, int card, int handPos);
int getTopDiscard(struct gameState* state);

// TEST PROTO-TYPES
int baronTest1();
int baronTest2();
int baronTest3();

int main()
{
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("NOTE TO GRADER: Rules Available: change RULES MACRO to 1 for Rules used.\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	baronTest1();
	baronTest2();
	baronTest3();
	printf("\n\n");
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

void prep(int player, int seed, int* kingdom, struct gameState* state, int numEstates, int handSize, int handPos, int card)
{
	initTestGame(player, kingdom, seed, state);
	//removeEstatesDeck(state);
	wipeDeck(state);
	updateCoins(player, state, 0); /* <== added this */
	removeEstatesDiscard(state);
	setEstateSupply(state, numEstates); /* <-- add 1 estate to supply here */
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

/******************************************************
 * Description:
 * 	tests standard business logic where there
 * 	isn't an estate in the supply, but there is
 * 	at least one estate in current player's hand.
 *
 * Check against:
 * 	no increase in num buys
 * 		- displays difference
 * 	when discard an estate but,
 * 	not found in discard afterward
 *
 * ***************************************************/
int baronTest1()
{
	int result = 0;
	int seed = 1;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState backup;

	/* TEST 1 No Estates in Supply, handCount 5, 1 estate in Hand at position 'i' */
	if(RULES)
	{
		printf("unittest1.c bug #1: no break out of loop where estate in hand found !\n");
		printf("BARON TEST 1: Rules:\n");
		printf("            : loops 5 times.\n");
		printf("            : Choice1 for baronCard( ) is always 1 = discard an estate for +4 coins.\n");
		printf("	    : There is only 1 estate in a hand of 5.\n");
		printf("	    : There are NO estates in supply, discard, or deck to start.\n");
		printf("OUTPUT:\n");
	}
	int i;
	for (i = 0; i < 5; i++)
	{
		prep(2, seed, kingdomCards, &G, 0, 5, i, estate);
		int currentPlayer = whoseTurn(&G); // <------------ added this for Reuben's version
		memset(&backup, '\0', sizeof(backup));
		backup = G;

		refactoredBaron(1, currentPlayer, &G);

		/* check num buys */
		if ((backup.numBuys + 1) != G.numBuys)
		{
			printf("Error: Baron card played, but numBuys didn't increase.\n");
			printf("     : previous numBuys: %d\n", backup.numBuys);
			printf("     : current numBuys: %d\n", G.numBuys);
			result = -1;
		}
		/* check top of discard */
		if (getTopDiscard(&G) != estate)
		{
			char name[MAX_STRING_LENGTH];
			memset(name, '\0', sizeof(name));

			printf("Error: Chose to discard an estate, but no estate found in discard !\n");
			currentPlayer = whoseTurn(&G); // <------------ declared earlier for Reuben's version
			cardNumToName(G.discard[currentPlayer][G.discardCount[currentPlayer]], name);
			printf("Top of discard contains: %s\n", name);
			result = -1;
		}
		else if (getTopDiscard(&G) == estate)
		{
			/* check coins increased */
			if (backup.coins + 4 != G.coins)
			{
				printf("Error: An estate in discard, but coins is not +4 previous.\n");
				printf("     : previous coins: %d\n", backup.coins);
				printf("     : current  coins: %d\n", G.coins);
				result = -1;
			}
		}
	}
	return result;
}

int baronTest2()
{
	int result = 0;

	int seed = 1;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	/* TEST 2 */ 
	if(RULES)
	{
		printf("unittest1.c con't.. because no break out of loop when estate in hand found & you have\n");
		printf("                    more than 1 estate..you'll keep adding coins and cheat !\n");
		printf("BARON TEST 2: Rules:\n");
		printf("	    : Never Loops.\n");
		printf("            : Choice1 for baronCard( ) is always 1 = discard an estate for +4 coins.\n");
		printf("	    : There are 5 estates in a hand of 5.\n");
		printf("	    : There are NO estates in supply, discard, or deck to start.\n");
		printf("OUTPUT:\n");
	}

	// Game States 
	struct gameState G;
	struct gameState backup;

	/* TEST 2 */
	prep(2, seed, kingdomCards, &G, 0, 5, 0, -1);

	int currentPlayer = whoseTurn(&G);

	int i;
	for (i = 0; i < G.handCount[currentPlayer]; i++)
	{
		setHandPos(&G, estate, i);
	}

	memset(&backup, '\0', sizeof(backup));
	backup = G;
	refactoredBaron(1, currentPlayer, &G);

	/* check num buys */
	if ((backup.numBuys + 1) != G.numBuys)
	{
		printf("Error: Baron card played, but numBuys didn't increase.\n");
		printf("     : previous numBuys: %d\n", backup.numBuys);
		printf("     : current numBuys: %d\n", G.numBuys);
		result = -1;

	}
	/* check top of discard */
	if (getTopDiscard(&G) != estate)
	{
		printf("Error: Chose to discard an estate, but no estate found in discard !\n");
		result = -1;
	}
	else if (getTopDiscard(&G) == estate)
	{
		/* check coins increased */
		if (backup.coins + 4 != G.coins)
		{
			printf("Error: An estate in discard, but coins is not +4 previous.\n");
			printf("     : previous coins: %d\n", backup.coins);
			printf("     : current  coins: %d\n", G.coins);
			result = -1;
		}
	}
	return result;
}

int baronTest3()
{
	int result = 0;

	int seed = 1;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState backup;

	if(RULES)
	{
		printf("unittest1.c con't.. Bug #2 of baronCard() gains an estate when there aren't any from\n");
		printf("                    the supply. This test locates that error although ONLY by altering\n");
		printf("                    gainCard() 's first condition. That condition is commented out, to\n");
		printf("                    demonstrate what would happen in baronCard if that were not there.\n\n");
		printf("BARON TEST 3: Rules:\n");
		printf("            : loops 5 times.\n");
		printf("            : Choice1 for baronCard( ) is always 0 = Gain an estate from supply.\n");
		printf("	    : There is only 1 estate in a hand of 5.\n");
		printf("	    : There are NO estates in supply, discard, or deck to start.\n");
		printf("OUTPUT:\n");
	}

	/* TEST 3 */
	int i;
	for (i = 0; i < 5; i++)
	{
		prep(2, seed, kingdomCards, &G, 0, 5, i, estate);
		memset(&backup, '\0', sizeof(backup));
		int currentPlayer = whoseTurn(&G); // <------------ added this for Reuben's version
		backup = G;
		refactoredBaron(0, currentPlayer, &G);

		/*
		printf("BEFORE\n");
		printDiscard(G.whoseTurn, &G);
		printSupply(&G);
		*/

		/* check num buys */
		if ((backup.numBuys + 1) != G.numBuys)
		{
			printf("Error: Baron card played, but numBuys didn't increase.\n");
			printf("     : previous numBuys: %d\n", backup.numBuys);
			printf("     : current numBuys: %d\n", G.numBuys);
			result = -1;

		}
		if (backup.supplyCount[estate] == 0)
		{
			if (getTopDiscard(&backup) != estate && getTopDiscard(&G) == estate)
			{
				char name[MAX_STRING_LENGTH];
				memset(name, '\0', sizeof(name));
				cardNumToName(estate, name);
				printf("Error: There weren't any estates in the supply from before.\n");
				printf("     : previous supply of card: %s was %d\n", name, backup.supplyCount[estate]);
				printf("     : current supply of card: %s is %d\n", name, G.supplyCount[estate]);
				printf("   ~~ But you have one in your discard !\n");

				memset(name, '\0', sizeof(name));
				//int currentPlayer = whoseTurn(&G);
				int index = getTopDiscard(&G);
				//cardNumToName(G.discard[currentPlayer][G.discardCount[currentPlayer]], name);
				cardNumToName(index, name);
				printf("Top of discard contains: %s\n", name);
				result = -1;

			}
		}
		/*
		printf("AFTER\n");
		printDiscard(G.whoseTurn, &G);
		printSupply(&G);
		*/
	
	}
	return result;
}
