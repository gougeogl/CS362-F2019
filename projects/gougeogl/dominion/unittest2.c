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

enum TEST_FLAGS
{
	SAME_HAND = 800,
	DIFFERENT_HAND,
	SAME_DECK,
	DIFFERENT_DECK,
	SAME_DISCARD,
	DIFFERENT_DISCARD
};

// TEST PROTO-TYPES
void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game);
void prepMinion(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card);
int prepOthersMinion(int originalPlayer, struct gameState* state, int handSize );

// HELPER PROTOS-TYPES
void wipeDeck(int player, struct gameState* state);
void wipeDiscard(int player, struct gameState* state);

void resetHand(int player, struct gameState* dState);
void setHandCount(int player, struct gameState* state, int newHandSize);
void setHandPos(int player, struct gameState* state, int card, int handPos);

int compareCoins(int player, struct gameState* before, struct gameState* after );
int compareNumActions(int player, struct gameState* before, struct gameState* after);
int compareHand(int player, struct gameState* before, struct gameState* after , int flag );
int compareDeck(int player, struct gameState* before, struct gameState* after, int limit, int flag );
int compareDiscard(int player, struct gameState* before, struct gameState* after, int limit, int flag);

/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards();

int minionTest1();
int minionTest2();

int main()
{
	//minionTest1();
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
	wipeDeck(0, state);
	wipeDiscard(0, state);
	updateCoins(0, state, 0); /* <== added this */
	resetHand(0, state);
	setHandCount(0, state, handSize);
	setHandPos(0, state, card, handPos);
	state->coins = 0;
}

int prepOthersMinion(int originalPlayer, struct gameState* state, int handSize )
{
	if(state->numPlayers > 1 && state->numPlayers < 5)
	{
		int currentPlayer = originalPlayer;
		currentPlayer++;

		int numPlayers = state->numPlayers;
		int number = -1;

		int i;
		for(i = currentPlayer; i < numPlayers; i++)
		{
			wipeDeck(i, state);
			wipeDiscard(i, state);
			updateCoins(i, state, 0);
			resetHand(i, state);
			setHandCount(i, state, handSize);
			state->coins = 0;
/*
			printf("IN: setUpOtherPlayers..before assign hand\n");
			printf("player %d handCount: %d\n", i, state->handCount[i] );
			printHand(currentPlayer, state);
			printf("player %d deckCount: %d\n", i, state->deckCount[i] );
			printDeck(currentPlayer, state);
			printf("player %d discardCount: %d\n", i, state->discardCount[i] );
			printDiscard(currentPlayer, state);
*/
			int j;
			for (j = 0; j < handSize; j++)
			{
				number = _rand_of_kingdomCards();
	//			printf("number: %d\n", number);
				setHandPos(i, state, number, j);
	//			printf("%d\n", state->hand[i][j] );
			}
/*
			printf("STATS IN : player %d: setUpOtherPlayers\n", i);
			printf("player %d handCount: %d\n", i, state->handCount[i] );
			printHand(currentPlayer, state);
			printf("player %d deckCount: %d\n", i, state->deckCount[i] );
			printDeck(currentPlayer, state);
			printf("player %d discardCount: %d\n", i, state->discardCount[i] );
			printDiscard(currentPlayer, state);
*/
		}

	}
	return originalPlayer;
}
// set player to remove all estates from current player's deck  
void wipeDeck(int player, struct gameState* state)
{
	int i = 0;
	while (i < state->deckCount[player])
	{
		state->deck[player][i] = -1;
		i++;
	}
	state->deckCount[player] = 0;
}

// eliminate all estates from discard.   
void wipeDiscard(int player, struct gameState* state)
{
	int i = 0;
	while (i < state->discardCount[player])
	{
		state->discard[player][i] = -1;
		i++;
	}
	state->discardCount[player] = 0;
}

/* Sets current player's handCount to newHandSize, then
	overwrites everything in hand with -1 */
void resetHand(int player, struct gameState* dState)
{
	int i;
	for (i = 0; i < dState->handCount[player]; i++)
	{
		dState->hand[player][i] = -1;
	}
	dState->handCount[player] = 0;
}

void setHandCount(int player, struct gameState* state, int newHandSize)
{
	state->handCount[player] = newHandSize;
}

/* adds indicated card in current player's hand at handPos */
void setHandPos(int player, struct gameState* state, int card, int handPos)
{
	//printf("IN setHandPos player is: %d\n", player);
	state->hand[player][handPos] = card;
	//printf("IN setHandPos now at handPos is: %d\n", state->hand[player][handPos] );
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
/* *************************************************************************
* coins
* Check is coins of a player is same before some saved gameState (before) and
* if coins is +2 will print nothing.
* if coins is < +2 previous will print error.
* if coins is > +2 previous will print that error.
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
* numActions
* Check is numActions of a player is same before some saved gameState (before) and
* if numActions is not +1 previous  will print error, nothing otherwise.
*
****************************************************************************/
int compareNumActions(int player, struct gameState* before, struct gameState* after)
{
	int result = 0;

	if (before->numActions + 1 != after->numActions)
	{
		printf("Player %d\'s numActions is not +1 previous !\n", player);
		printf("BEFORE: numActions: %d\n", before->numActions);
		printf("AFTER:  numActions: %d\n", after->numActions);
		fflush(stdout);
		result = -1;	
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
	int printStats = 0; 

	int i;
	for (i = 0; i < after->handCount[player]; i++) 
	{
		if (flag == SAME_HAND)
		{
			if (before->hand[player][i] != after->hand[player][i]) 
			{
				printf("player %d\'s hand is different\n", player);
				fflush(stdout);
				printStats = 1;
			}
		}
		else if (flag == DIFFERENT_HAND)
		{
			if (before->hand[player][i] == after->hand[player][i]) 
			{

				printf("player %d\'s hand is still the same\n", player);
				fflush(stdout);
				printStats = 1;
			}
		}
		
		if(printStats == 1)
		{
			char name[MAX_STRING_LENGTH];
			memset(name, '\0', sizeof(name));
			cardNumToName(before->hand[player][i], name);

			char nombre[MAX_STRING_LENGTH];
			memset(nombre, '\0', sizeof(nombre));
			cardNumToName(after->hand[player][i], nombre);

			printf("BEFORE: hand[%d]: %s, AFTER: hand[%d]: %s\n", i, name, i, nombre );
			fflush(stdout);
			result = -1;
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
	int printStats = 0;

	int i;
	for (i = 0; i < limit; i++) 
	{
		if (flag == SAME_DECK)
		{
			if (before->deck[player][i] != after->deck[player][i]) {
				printf("player %d\'s deck is different\n", player);
				fflush(stdout);
				printStats = 1;	
			}
		}
		else if (flag == DIFFERENT_DECK)
		{
			if (before->deck[player][i] == after->deck[player][i]) {
				printf("player %d\'s deck is still the same\n", player);
				fflush(stdout);
				printStats = 1;	
			}
		}
		
		if(printStats == 1)
		{
			char name[MAX_STRING_LENGTH];
			memset(name, '\0', sizeof(name));
			cardNumToName(before->deck[player][i], name);

			char nombre[MAX_STRING_LENGTH];
			memset(nombre, '\0', sizeof(nombre));
			cardNumToName(after->deck[player][i], nombre);

			printf("BEFORE: deck[%d]: %s, AFTER: deck[%d]: %s\n", i, name, i, nombre );
			fflush(stdout);
			result = -1;
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
	int printStats = 0;

	int i;
	for (i = 0; i < limit; i++)
	{
		if (flag == SAME_DISCARD)
		{
			if (before->discard[player][i] != after->discard[player][i]) {
				printf("player %d\'s discard is different\n", player);
				fflush(stdout);
				printStats = 1;	
			}
		}
		else if (flag == DIFFERENT_DISCARD)
		{
			if (before->discard[player][i] == after->discard[player][i]) {
				printf("player %d\'s discard is still the same\n", player);
				fflush(stdout);
				printStats = 1;	
			}
		}

		if(printStats == 1)
		{
			char name[MAX_STRING_LENGTH];
			memset(name, '\0', sizeof(name));
			cardNumToName(before->discard[player][i], name);

			char nombre[MAX_STRING_LENGTH];
			memset(nombre, '\0', sizeof(nombre));
			cardNumToName(after->discard[player][i], nombre);

			printf("BEFORE: discard[%d]: %s, AFTER: discard[%d]: %s\n", i, name, i, nombre );
			fflush(stdout);
			result = -1;
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

	prepMinion(2, seed, kingdomCards, &G, 5, 0, -1);
	int number = -1;

	int i;
	for (i = 0; i < 5; i++)
	{
		number = _rand_of_kingdomCards();
		setHandPos(0, &G, number, i);
	}

	int otherNumber = rand() % (4 - 0 + 1) + 0;
	setHandPos(0, &G, otherNumber, minion); /* <== need a minion in hand to use it right ? */

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

	prepMinion(3, seed, kingdomCards, &G, 5, 0, -1);
	G.whoseTurn = prepOthersMinion(G.whoseTurn, &G, 5 );

	int number = -1;

	int i;
	for (i = 0; i < 5; i++)
	{
		number = _rand_of_kingdomCards();
		setHandPos(0, &G, number, i);
	}

	int otherNumber = rand() % (4 - 0 + 1) + 0;
	setHandPos(0, &G, otherNumber, minion); /* <== need a minion in hand to use it right ? */

	memset(&backup, '\0', sizeof(backup));
	backup = G;

	/* int minionCard(int choice1, int choice2, struct gameState *state, int handPos) */
	/* void prepMinion(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card) */

	int j;
	for(j = 0; j < G.numPlayers; j++)
	{
		printf("PRE: player %d\n", j);

		printf("PRE: minionTest2() gameState is:\n");
		printState(&G);

		printf("PRE: minionTest2() handCount is: %d\n", G.handCount[j] );
		printHand(j, &G);

		printf("PRE: minionTest2() discard is:\n");
		printDiscard(j, &G);
		printf("TRACE: otherNumber: %d\n", otherNumber);

		printf("PRE: minionTest2() deckCount is: %d\n", G.deckCount[j] );
		printDeck(j, &G);

		printf(" ** NEXT PLAYER PRE ** MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	}

	printf(" <========================== ABOUT to CALL !!\n");
	printf("JUST BEFORE CALL player is: %d\n", G.whoseTurn );
	minionCard(1, 1, &G, otherNumber);
	printf("JUST AFTER CALL player is: %d\n", G.whoseTurn );
	printf(" <========================== ABOUT to CALL !!\n");

	int k;
	for(k = 0; k < G.numPlayers; k++)
	{
		printf("PLAYER %d\n", k);
		printf("POST: minionTest2() gameState is:\n");
		printState(&G);

		printf("POST: minionTest2() handCount is: %d\n", G.handCount[k] );
		printHand(k, &G);

		printf("POST: minionTest2() deckCount is: %d\n", G.deckCount[k] );
		printDeck(k, &G);

		printf("POST: minionTest2() discardCount is: %d\n", G.discardCount[k] );
		printDiscard(k, &G);

		printf(" ** NEXT PLAYER POST ** MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	}

	compareNumActions(G.whoseTurn, &backup, &G);
	compareCoins(G.whoseTurn, &backup, &G);

	return result;
}



