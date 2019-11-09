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
#define RULES 1 /* <========= SET TO 1 TO PRINT RULES FOR TESTS & RELATION TO BUGS I INTRODUCED !! */

// TEST PROTO-TYPES
void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game);
void prepMinion(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card);

// HELPER PROTOS-TYPES
void wipeDeck(struct gameState* state);
void wipeDiscard(struct gameState* state);

void resetHand(struct gameState* dState);
void setHandCount(struct gameState* state, int newHandSize);
void setHandPos(struct gameState* state, int card, int handPos);

int compareHand(int player, struct gameState* before, struct gameState* after, int flag);
int compareDeck(int player, struct gameState* before, struct gameState* after, int limit, int flag);
int compareDiscard(int player, struct gameState* before, struct gameState* after, int limit, int flag);

/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards()

int minionTest1();
int minionTest2();

int main()
{
	minionTest1();
	minionTest2();
	printf("\n\n");
	return 0;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

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
	wipeDeck(state);
	wipeDiscard(state);
	updateCoins(state->whoseTurn, state, 0); /* <== added this */
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

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
/* *************************************************************************
* coins
* Check is coins of a player is same before some saved gameState (before) and
* if coins is +2 will print nothing.
* if coins is < +2 previous will print error.
* if coins is > +2 previous will print that error.
* S.T. flag of desired comparison SAME_HAND, DIFFERENT_HAND
*
****************************************************************************/
int compareCoins(int player, struct gameState* before, struct gameState* after )
{
	int result = 0;
	int printStats = 0;
	if (before->coins + 2 < after->coins)
	{
		printf("Player %d\'s coins is less than +2 previous !\n", player);
		fflush(stdout);
		printStats = 1;
		result = -1;
	}
	else if (before->coins + 2 < after->coins)
	{
		printf("Player %d\'s coins is more than +2 previous !\n", player);
		fflush(stdout);
		printStats = 1;
		result = -1;
	}

	if (printStats == 1)
	{
		printf("BEFORE: coins: %d\n", before->coins);
		printf("AFTER:  coins: %d\n", after->coins);
		fflush(stdout);
	}
	
	return result;
}

/* *************************************************************************
* hand 
* Check is hand of a player is same before some saved gameState (before) and
* if hand is different, will print the difference. 
* if hand is same, will warn the same.
* S.T. flag of desired comparison SAME_HAND, DIFFERENT_HAND
*
****************************************************************************/
int compareHand(int player, struct gameState* before, struct gameState* after , int flag )
{
	int result = 0;

	if(flag == SAME_HAND)
	int i;
	for (i = 0; i < after->handCount[player]; i++) 
	{
		if (flag == SAME_HAND)
		{
			if (before->hand[player][i] != after->hand[player][i]) {
				printf("player %d\'s hand is different\n", player);
				printf("BEFORE: hand[%d]: %s, AFTER: hand[%d]: %s\n", i, before->hand[player][i], i, after->hand[player][i]);
				fflush(stdout);
				result = -1;
			}
		}
		else if (flag == DIFFERENT_HAND)
		{
			if (before->hand[player][i] == after->hand[player][i]) {
				printf("player %d\'s hand is still the same\n", player);
				printf("BEFORE: hand[%d]: %s, AFTER: hand[%d]: %s\n", i, before->hand[player][i], i, after->hand[player][i]);
				fflush(stdout);
				result = -1;
			}
		}
	}

	return result;
}


/* *************************************************************************
* deck
* Check is deck of a player is same / different before some saved 
* gameState (before) 
* if hand is different, will print the difference.
* if hand is same, will warn hand is the same.
* S.T. flag of desired comparison SAME_DECK, DIFFERENT_DECK
*
****************************************************************************/
int compareDeck(int player, struct gameState* before, struct gameState* after, int limit, int flag )
{
	int result = 0;

	int i;
	for (i = 0; i < limit; i++) 
	{
		if (flag == SAME_DECK)
		{
			if (before->deck[player][i] != after->deck[player][i]) {
				printf("player %d\'s deck is different\n", player);
				printf("BEFORE: deck[%d]: %s, AFTER: deck[%d]: %s\n", i, before->deck[player][i], i, after->deck[player][i]);
				fflush(stdout);
				result = -1;
			}
		}
		else if (flag == DIFFERENT_DECK)
		{
			if (before->deck[player][i] == after->deck[player][i]) {
				printf("player %d\'s deck is still the same\n", player);
				printf("BEFORE: deck[%d]: %s, AFTER: deck[%d]: %s\n", i, before->deck[player][i], i, after->deck[player][i]);
				fflush(stdout);
				result = -1;
			}
		}
	}
	return result;
}

/* *************************************************************************
* discard
* Check is discard of a player is same / different before some saved
* gameState (before)
* if discard is different, will print the difference.
* if discard is same, will warn hand is the same.
* S.T. flag of desired comparison SAME_DISCARD, DIFFERENT_DISCARD
*
****************************************************************************/
int compareDiscard(int player, struct gameState* before, struct gameState* after, int limit, int flag)
{
	int result = 0;

	int i;
	for (i = 0; i < limit; i++)
	{
		if (flag == SAME_DISCARD)
		{
			if (before->discard[player][i] != after->discard[player][i]) {
				printf("player %d\'s discard is different\n", player);
				printf("BEFORE: discard[%d]: %s, AFTER: discard[%d]: %s\n", i, before->discard[player][i], i, after->discard[player][i]);
				fflush(stdout);
				result = -1;
			}
		}
		else if (flag == DIFFERENT_DISCARD)
		{
			if (before->discard[player][i] == after->discard[player][i]) {
				printf("player %d\'s discard is still the same\n", player);
				printf("BEFORE: discard[%d]: %s, AFTER: discard[%d]: %s\n", i, before->discard[player][i], i, after->discard[player][i]);
				fflush(stdout);
				result = -1;
			}
		}
	}
	return result;
}

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

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
int minionTest1()
{
	int result = 0;
	int seed = 1;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState backup;

	/* TEST 1 */
	if (RULES)
	{
		printf("unittest2.c bug #3: draws 3 cards instead of 4 for whoever played minion, but not other players\n");
		printf("MINION TEST 1: Rules:\n");
		printf("OUTPUT:\n");
	}

	prepMinion(2, seed, kingdomCards, &G, 0, 5, i, estate);
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	/* int minionCard(int choice1, int choice2, struct gameState *state, int handPos) */
	/* void prepMinion(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card) */

	return result;
}

int minionTest2()
{
	int result = 0;
	int seed = 1;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState backup;

	/* TEST 2 */
	if (RULES)
	{
		printf("unittest2.c bug #4: condition 2 isn't an \'else if\' so you always gain +2 to coin.\n");
		printf("MINION TEST 2: Rules:\n");
		printf("OUTPUT:\n");
	}

	prepMinion(2, seed, kingdomCards, &G, 5, 0, -1);
	int number = -1;

	int i;
	for (i = 0; i < 5; i++)
	{
		number = _rand_of_kingdomCards();
		setHandPos(&G, number, i);
	}

	int otherNumber = rand() % (4 - 0 + 1) + 0;
	setHandPos(&G, otherNumber, minion); /* <== need a minion in hand to use it right ? */

	memset(&backup, '\0', sizeof(backup));
	backup = G;

	/* int minionCard(int choice1, int choice2, struct gameState *state, int handPos) */
	/* void prepMinion(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card) */

	minionCard(1, 1, &G, otherNumber);

	compareCoins(G->whoseTurn, &backup, &G);

	return result;
}



