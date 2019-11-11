/***************************************************************
** FileName: unittest3.c
** Author: Glen Gougeon
** Class : CS362 Software Engineering II
** Date : 10 - 25 - 2019
** Last Mod : 11 - 10 - 2019
*
** Description : Assignment 3 : Unit Testing :
*		Refactored code for Ambassador, has 2 bugs I introduced.
*		The following test code should catch the errors while
*		executing 80 % branch and function coverage according
*		to gcov coverage tool.
*
** To Compile:	Use included 'Makefile'
*		compile command : make unittest3
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

enum TEST_FLAGS
{
	SAME_HAND = 800,
	DIFFERENT_HAND,
	SAME_DECK,
	DIFFERENT_DECK,
	SAME_DISCARD,
	DIFFERENT_DISCARD,
	PLUS_2_COINS,
	SAME_COINS
};

// TEST PROTO-TYPES
void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game);
void prepAmbassador(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card);
int prepOthers(int originalPlayer, struct gameState* state, int handSize );

// HELPER PROTOS-TYPES
void wipeDeck(int player, struct gameState* state);
void wipeDiscard(int player, struct gameState* state);

void resetHand(int player, struct gameState* dState);
void setHandCount(int player, struct gameState* state, int newHandSize);
void setHandPos(int player, struct gameState* state, int card, int handPos);

int compareCoins(int player, struct gameState* before, struct gameState* after , int flag );
int compareNumActions(int player, struct gameState* before, struct gameState* after);
int compareHand(int player, struct gameState* before, struct gameState* after , int flag );
int compareDeck(int player, struct gameState* before, struct gameState* after, int limit, int flag );
void savePreviousHandCounts(int* container, struct gameState* state );
void saveZerosHand(int player, struct gameState* state, int* ambosHand);

/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards();

int ambassadorTest1();
//int ambassadorTest2();

int main()
{
	ambassadorTest1();
	//ambassadorTest2();
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

/* *************************************************************************
* prep
* Sets up player zero for testing minionCard() 
* CALLS:
* 	initTestGame() to initialize game 
* 	wipeDeck()
* 	wipeDiscard()
* 	updateCoins()
* 	resetHand()
* 	setHandCount()
* SETS:
* 	state->coins = 0
*
* APPLIES TO: PLAYER ZERO
*
* RETURNS: n/a 
*
****************************************************************************/
void prepAmbassador(int players, int seed, int* kingdom, struct gameState* state, int handSize, int handPos, int card)
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

/* *************************************************************************
* prepOthers 
* Sets up other players for testing ambassadorCard() with handSize of 5
* CALLS:
* 	wipeDeck()
* 	wipeDiscard()
* 	updateCoins()
* 	resetHand()
* 	setHandCount()
* SETS:
* 	state->coins = 0
*
* APPLIES TO: ALL PLAYERS EXCEPT PLAYER ZERO
*
* RETURNS originalPlayer (the player who used the minion card)
*		* should NOT change from input parameter
****************************************************************************/
int prepOthers(int originalPlayer, struct gameState* state, int handSize )
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

			int j;
			for (j = 0; j < handSize; j++)
			{
				number = _rand_of_kingdomCards();
				setHandPos(i, state, number, j);
			}
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

// sets handCount of player to newHandSize
void setHandCount(int player, struct gameState* state, int newHandSize)
{
	state->handCount[player] = newHandSize;
}

// adds indicated card in current player's hand at handPos 
void setHandPos(int player, struct gameState* state, int card, int handPos)
{
	state->hand[player][handPos] = card;
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
int compareCoins(int player, struct gameState* before, struct gameState* after , int flag )
{
	int result = 0;
	int printStats = 0;

	if(flag == PLUS_2_COINS)
	{
		if (before->coins + 2 != after->coins)
		{
			printf("Player %d\'s coins is not +2 previous !\n", player);
			fflush(stdout);
			printStats = 1;
		}
	}
	else if(flag == SAME_COINS)
	{
		if( before->coins != after->coins )
		{
			printf("Player %d\'s coins changed, but shouldn't have.\n", player);
			fflush(stdout);
			printStats = 1;
		}
	}
	if(printStats == 1)
	{
		printf("BEFORE: coins: %d\n", before->coins);
		printf("AFTER:  coins: %d\n", after->coins);
		fflush(stdout);
		result = -1;
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
* save - previous - hand - counts 
* Saves handCounts of all players after initial testing setup 
*
****************************************************************************/
void savePreviousHandCounts(int* container, struct gameState* state )
{
	int numPlayers = state->numPlayers;

	int i;
	for(i = 0; i < numPlayers; i++)
	{
		container[i] = state->handCount[i];	
	}
}

/* *************************************************************************
* save - player zero's hand
*
****************************************************************************/
void saveZerosHand(int player, struct gameState* state, int* ambosHand )
{
	int i;
	for (i = 0; i < state->handCount[player]; i++)
	{
		ambosHand[i] = state->hand[player][i];
		printf("IN saveZerosHand: ambosHand[i %d]: %d\n", i, ambosHand[i]);
	}
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

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
int ambassadorTest1()
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
		printf("unittest3.c bug #.\n");
		printf("AMBASSADOR TEST 1: Rules:\n");
		printf("                 : Use same kingdomCards deck as in Baron Card testing.\n");
		printf("                 : 4 Players.\n");
		printf("	             : Each player starts with 5 random cards in hand from kingdomCards.\n");
		printf("	             : Assign 2 Ambassador cards to primary player.\n"); 
		printf("                 : Because of bug, should remove BOTH Ambassador cards !\n");
		printf("OUTPUT:\n\n");
	}
	prepAmbassador(4, seed, kingdomCards, &G, 5, 0, ambassador)
	prepOthers(0, &G, 5);

	int number = -1;

	/* FILL HAND WITH RANDOM KINGDOM CARDS */
	int i;
	for (i = 0; i < 5; i++)
	{
		number = _rand_of_kingdomCards();
		setHandPos(0, &G, number, i);
	}

	/* place 1st ambassador in hand */
	int firstSlot = rand() % (4 - 0 + 1) + 0;
	setHandPos(0, &G, firstSlot, ambassador); /* <== need an ambassador in hand to use it right ? */
	/* place 2nd ambassador in hand */
	int secondSlot = rand() % (4 - 0 + 1) + 0;
	setHandPos(0, &G, secondSlot, ambassador); 

	/* SAVE HAND COUNTS  */
	int handBox[MAX_PLAYERS];
	savePreviousHandCounts(handBox, &G);

	/* SAVE PLAYER OF AMBASSADORS HAND */
	int primePlayersHand[G.hand[0]];
	saveZerosHand(0, primePlayersHand, &G);

	memset(&backup, '\0', sizeof(backup));
	backup = G;

	/** ==> CALL <================================================= */
	ambassadorCard(secondSlot, 2, &G, firstSlot);

	printf("\n\n");

	return result;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/




