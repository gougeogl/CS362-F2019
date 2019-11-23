/***************************************************************
** FileName: unittest5.c
** Author: Glen Gougeon
** Class : CS362 Software Engineering II
** Date : 11 - 16 - 2019
** Last Mod : 11 - 21 - 2019
*
** Description : Assignment 3 : Unit Testing :
*		Refactored code for Mine, has 2 bugs I introduced.
*		The following test code should catch the errors while
*		executing 80 % branch and function coverage according
*		to gcov coverage tool.
*
** To Compile:	Use included 'Makefile'
*		compile command : make unittest5
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
#define DEBUG_TOP2_DECK 0
#define DEBUG_TOP2_DISCARD 0
#define FILL_DECK_DEBUG 0
#define DEBUG_MINE_TEST_1 0

#define DEBUG_RANDOM 0

enum TEST_FLAGS
{
	SAME_HAND = 800,
	DIFFERENT_HAND,
	SAME_DISCARD,
	DIFFERENT_DISCARD,
	PLUS_2_COINS,
	SAME_COINS,
	/* used by prep functions */
	FILL_DIFF,
	FILL_SAME
};

// TEST PROTO-TYPES
void initTestGame(int numPlayers, int* kDeck, int mySeed, struct gameState* game);
void _prepOnePlayer(int player, struct gameState* state, int newHandSize, int fill_type, int card);

// HELPER PROTOS-TYPES
void fillDeck(int player, struct gameState* state, int newDeckSize);
void emptyDeck(int player, struct gameState* state);
void emptyDiscard(int player, struct gameState* state);

// HAND RELATED PROTO-TYPES
void emptyHand(int player, struct gameState* dState);
void setNewHandCount(int player, struct gameState* state, int newHandSize);
void setAtHandPos(int player, struct gameState* state, int card, int handPos);

// COMPARISION PROTO-TYPES
int compareCoins(int player, struct gameState* before, struct gameState* after , int flag );
int compareNumActionsTribute(int player, struct gameState* before, struct gameState* after);
int compareHand(int player, struct gameState* before, struct gameState* after , int flag );
int compareDeck(int player, struct gameState* before, struct gameState* after, int limit, int flag );
int compareTopsAfter(int player, int* deckTops, int* discardTops);
void setTypesFoundTribute(int* oldDeckTops, int* coin_count, int* draw_card_count, int* num_actions_count);

void saveTop2Deck(int player, struct gameState* state, int* topTwo);
void saveTop2Discard(int player, struct gameState* state, int* topTwo);

/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards();

/* generate an int in range [min..max] */
int _genRandRange(int min, int max);

/* RANDOM TEST */
int unitMineTest1();

int main()
{
	int qty_errors = 0;

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("ASSIGNMENT 3: UNIT TESTING: MINE CARD.\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	qty_errors = unitMineTest1();

		if (qty_errors == 0)
		{
			printf("ALL MINE TESTS PASSED !!\n\n");
		}
		else
			printf("MINE TEST FOUND: %d ERRORS !\n\n", qty_errors);

	return 0;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

/* This function adds memset initialization, and prints error if fail  */
void initTestGame(int numPlayers, int* kDeck, int mySeed, struct gameState* game)
{
	memset(game, '\0', sizeof(struct gameState));   // clear mem of 

	int checkInit = initializeGame(numPlayers, kDeck, mySeed, game); // initialize 2 player game 

	if (checkInit == -1)
	{
		printf("Bad game initialization.\n");
	}

}

/* *************************************************************************
* prepOnePlayer 
* 
* CALLS:
*	emptyDeck(player, state);
*	emptyDiscard(player, state);
*	updateCoins(player, state, 0);
*	emptyHand(player, state);
*	setNewHandCount(player, state, newHandSize);
*
* APPLIES TO: input param 'player'
*
****************************************************************************/
void _prepOnePlayer( 
	int player, 
	struct gameState* state, 
	int newHandSize, 
	int fill_type, 
	int card 
)
{
	emptyDeck(player, state);
	emptyDiscard(player, state);
	updateCoins(player, state, 0);
	emptyHand(player, state);
	setNewHandCount(player, state, newHandSize);

	int i;
		
	for (i = 0; i < newHandSize; i++)
	{
		if (fill_type == FILL_DIFF)
		{
			card = 0;
			card = _rand_of_kingdomCards();
			setAtHandPos(player, state, card, i);
		}
		else if (fill_type == FILL_SAME)
		{
			setAtHandPos(player, state, card, i);
		}
	}

}

// set player to remove all estates from current player's deck  
void emptyDeck(int player, struct gameState* state)
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
void emptyDiscard(int player, struct gameState* state)
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
void emptyHand(int player, struct gameState* state)
{
	int i;
	for (i = 0; i < state->handCount[player]; i++)
	{
		state->hand[player][i] = -1;
	}
	state->handCount[player] = 0;
}

// sets handCount of player to newHandSize
void setNewHandCount(int player, struct gameState* state, int newHandSize)
{
	state->handCount[player] = newHandSize;
}

// adds indicated card in current player's hand at handPos 
void setAtHandPos(int player, struct gameState* state, int card, int handPos)
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
* hand 
* Check if hand of a player is same before some saved gameState (before) and
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
* discard
* Check is discard of a player is same / different before some saved 
* gameState (before) 
* if discard is different, will print the difference.
* if discard is same, will warn discard is the same.
* S.T. flag of desired comparison SAME_DISCARD, DIFFERENT_DISCARD
*
****************************************************************************/
int compareDiscard(int player, struct gameState* before, struct gameState* after, int limit, int flag )
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

/* *************************************************************************
* Sets int* params to tell which comparitor function to call
* compareCoin(), compareNumActions(), or compareNumBuys()
* oldDeckTops is an array of 2 ints that contains the enum of the cards
* that were in the last 2 slots of the nextPlayer's deck before the call
* to tributeCard()
****************************************************************************/
void setTypesFoundTribute(int* oldDeckTops, int* coin_count, int* draw_card_count, int* num_actions_count)
{
	int i;
	for (i = 0; i < 2; i++) {
		if (oldDeckTops[i] == copper || oldDeckTops[i] == silver || oldDeckTops[i] == gold) { //Treasure cards
			coin_count++;
		}

		else if (oldDeckTops[i] == estate ||
			oldDeckTops[i] == duchy ||
			oldDeckTops[i] == province ||
			oldDeckTops[i] == gardens ||
			oldDeckTops[i] == great_hall) { //Victory Card Found

			draw_card_count += 2;
		}
		else { //Action Card
			num_actions_count += 2;
		}
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
int unitMineTest1()
{
	int numErrors = 0;
	int result = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState backup;

	/* TEST 1 */
	if (RULES)
	{
		printf("unittest5.c\n");
		printf("MINE TEST 1: Rules:\n");
		printf("	          : Wipe every player's hand, deck, and discard piles.\n");
		printf("              : Use these kingdomCards:\n");
		printf("              : 	adventurer\n");
		printf("              : 	ambassador\n");
		printf("              : 	baron\n");
		printf("              : 	estate\n");
		printf("              : 	tribute\n");
		printf("              : 	minion\n");
		printf("              : 	mine\n");
		printf("              : 	gardens\n");
		printf("              : 	remodel\n");
		printf("              : 	smithy\n");
		printf("              : 2 Players.\n");
		printf("              : Current Player = 0 (means player 1).\n");
		printf("			  : Randomly assign 5 cards from kingdomCards to..\n"); 
		printf("              : each player's hand.\n"); 
		printf("              : each player's deck.\n"); 
		printf("              : Place a tribute card at a random position in hand.\n");
		printf("OUTPUT:\n\n");
	}

	// number of players
	int newNumPlayers = 2;

	/* # players, cards to use, seed, gameState */
	initTestGame(newNumPlayers, kingdomCards, 1, &G);

	int currentPlayer = 0;

	// size of hand to use
	int newHandSize = 5;

	_prepOnePlayer(
		currentPlayer,
		&G,
		int newHandSize,	// 5
		FILL_SAME,			// fill hand with the same 
		-1					// use -1 to fill a.k.a. unused
	)

	/* set an estate card at handPos 0 as choice1 .. to fail */
	setAtHandPos(currentPlayer, &G, estate, 0);

	/* SAVE PREVIOUS COUNTS  */
	int previousTrashCount = G.trashCount;
	int previousHandCount = G.handCount[currentPlayer];
	int previousDiscardCount = G.discardCount[currentPlayer];

	/* SAVE PREVIOUS TOPS */
	int previousTopTrash = G.trashPile[trashCount];
	int previousTopDiscard = G.discard[currentPlayer][G.discardCount[currentPlayer]];

	/* SAVE PREVIOUS HAND */
	int previousHandContents[5];
	int tempHandPos = G.handCount[currentPlayer];

	int i;
	for (i = 0; i < 5; i++)
	{
		previousHandContents[i] = G.hand[currentPlayer][tempHandPos];
		tempHandPos--;
	}

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

		
	/* INPUTS FOR MINE CARD */
	int moneyToTrashHandLocation = 0;
	int moneyToGain = copper; // choice2
	int discardCardFromThisHandLocation = 4; // a.k.a. handPos of mineCard() call

	/** ==> CALL <================================================= */
	int result = mineCard(
		moneyToTrashHandLocation, 
		moneyToGain, 
		&G, 
		discardCardFromThisHandLocation
	);
	/** ==> END CALL <================================================= */

	if (result != 0)
	{
		printf("ERROR: mineCard returned %d\n", result);

		char choice1Buffer[MAX_STRING_LENGTH];
		memset(choice1Buffer, '\0', sizeof(choice1Buffer));
		cardNumToName(moneyToTrashHandLocation, choice1Buffer);

		char choice2Buffer[MAX_STRING_LENGTH];
		memset(choice2Buffer, '\0', sizeof(choice2Buffer));
		cardNumToName(moneyToGain, choice2Buffer);

		char handPosBuffer[MAX_STRING_LENGTH];
		memset(handPosBuffer, '\0', sizeof(handPosBuffer));
		cardNumToName(discardCardFromThisHandLocation, handPosBuffer);

		printf("choice1 was: %d\n", choice1Buffer);
		printf("choice2 was: %d\n", choice2Buffer);
		printf("handPos was: %d\n", handPosBuffer);

	}

	if( previousTrashCount + 1 != G.trashCount)
	{
		printf("MINE: trashCount is not + 1 previous.\n");
		numErrors++;
	}
	if (previousHandCount == G.handCount[currentPlayer])
	{
		if (previousHandConents[moneyToTrashHandLocation] == copper ||
			previousHandConents[moneyToTrashHandLocation] == silver ||
			previousHandConents[moneyToTrashHandLocation] == gold) {

			printf("MINE: You had valid money at position %d but your handCount didn't change.\n");
			numErrors++;

			compareHand(0, &backup, &G, DIFFERENT_HAND);
		}
	}
	if (previousDiscardCount != G.discardCount[currentPlayer])
	{
		if (previousHandConents[moneyToTrashHandLocation] == G.discard[currentPlayer][G.discardCount[currentPlayer]])
		{
			printf("MINE: choice1 Mine found in DISCARD !\n");
			numErrors++;
		}
		if (previousHandConents[moneyToTrashHandLocation] != G.trashPile[G.trashCount] )
		{
			printf("MINE: choice1 Mine MISSING from trashPile !\n");
			numErrors++;
		}
	}

	return numErrors;
}

/* EOF */

