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

//int compareCoins(int player, struct gameState* before, struct gameState* after , int flag );
//int compareNumActions(int player, struct gameState* before, struct gameState* after);
//int compareHand(int player, struct gameState* before, struct gameState* after , int flag );
//int compareDeck(int player, struct gameState* before, struct gameState* after, int limit, int flag );
//void savePreviousHandCounts(int* container, struct gameState* state );
void saveZerosHand(int player, struct gameState* state, int* ambosHand);

int tallyType(int player, struct gameState* state, int idx_picked );
/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards();

int ambassadorTest1();

int main()
{
	ambassadorTest1();
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
* save - player zero's hand
*
****************************************************************************/
void saveZerosHand(int player, struct gameState* state, int* ambosHand )
{
	int i;
	for (i = 0; i < state->handCount[player]; i++)
	{
		ambosHand[i] = state->hand[player][i];
		//printf("IN saveZerosHand: ambosHand[i %d]: %d\n", i, ambosHand[i]);
	}
}

/* *************************************************************************
* tally a type in hand 
*
****************************************************************************/
int tallyType(int player, struct gameState* state, int idx_picked )
{
	int tally = 0;

	int i;
	for(i = 0; i < state->handCount[player]; i++)
	{
		if(state->hand[player][i] == state->hand[player][idx_picked])
		{
			tally++;
			//printf("tally now: %d\n", tally);
		} 
	}
	//printf("returning tally total of: %d\n", tally);
	return tally;
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
		printf("	         : Each player starts with 5 random cards in hand from kingdomCards.\n");
		printf("	         : Assign 3 golds cards to primary player.\n"); 
		printf("	         : Assign 2 silvers cards to primary player.\n"); 
		printf("                 : Because of bug, should remove 1 Silver , but there aren't 3 total !\n");
		printf("OUTPUT:\n\n");
	}
	prepAmbassador(4, seed, kingdomCards, &G, 5, 0, ambassador);
	prepOthers(0, &G, 5);

	//int number = -1;

	/* FILL HAND WITH GOLD CARDS */
	int i;
	for (i = 0; i < 5; i++)
	{
		setHandPos(0, &G, gold, i);
	}

	/* add 2 silvers which isn't enough */
	setHandPos(0, &G, silver, 1);
	setHandPos(0, &G, silver, 2);

	/* place the ambassador card */
	setHandPos(0, &G, ambassador, 0); /* <== need an ambassador in hand to use it right ? */

	/* SAVE PLAYER OF AMBASSADORS HAND */
	int primePlayersHand[G.handCount[0]];
	saveZerosHand(0, &G, primePlayersHand);

	/* set up an index of some repeated card to remove.. silver */
	int redundant_card_idx = 1;
	int redundant_count_before = 0;

	/* tally the repeated cound of that card */
	redundant_count_before = tallyType(G.whoseTurn, &G, redundant_card_idx);

	/* save game state */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	int amount_to_remove = 2; 

	/** ==> CALL <================================================= */
	int beta = 0; // same as j from playCard()
	int alpha = 0; // same as i from playCard()
	int currentPlayer = G.whoseTurn; // same as zero..but documentable
	refactoredAmbassador(beta, alpha, 1, amount_to_remove, currentPlayer, &G, 0);

	/* NOW tally that same repeated card AFTERWARD */
	int redundant_count_after = 0;
	redundant_count_after = tallyType(G.whoseTurn, &G, redundant_card_idx);

	/* ERROR STATEMENTS  */
	if((redundant_count_before - amount_to_remove) != redundant_count_after)
	{
		printf("Ambassador did not remove desired amount\n");
		printf("quantity of repeated card to remove BEFORE call: %d\n", redundant_count_before );
		printf("quantity of repeated card to remove AFTER call: %d\n", redundant_count_after);

		if( redundant_count_before - 2 <  redundant_count_after )
		{
			printf("You illegally removed a card !\n");
		}
	}
	printf("\n\n");

	return result;
}
