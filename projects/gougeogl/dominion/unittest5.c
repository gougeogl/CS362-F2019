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
	EXPECT_ZERO,
	EXPECT_NEG_ONE,
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
int compareHand(int player, struct gameState* before, struct gameState* after , int flag );
int compareDeck(int player, struct gameState* before, struct gameState* after, int limit, int flag );

/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards();

/* generate an int in range [min..max] */
int _genRandRange(int min, int max);

/* PROTO-TYPE: PRINTS INPUTS IF EXPECTATIONS NOT MET */
void expectedReturn(
	int expect_flag,
	int actual,
	int moneyToTrashHandLocation,
	int moneyToGain,
	int discardCardFromThisHandLocation
);

/* OTHER UNIT TESTS */
int unitMineTest1();
int unitMineTest2();
int unitMineTest3();
int unitMineTest4();

/* PROTO-TYPE: CALLS UNIT TEST ON MINE CARD */
int unitMineTestDeluxe(
	/* start params */
	int test_no,
	int newHandSize,
	int choice1pos, // choice1
	int choice1card,
	int choice2, // choice2
	struct gameState* previousState,
	struct gameState* state, // state
	int* kingdomToUse,
	int handPos // handPos
	/* end params */
);

int main()
{
	int qty_errors = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, province, smithy };
	struct gameState backup;
	struct gameState G;
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("ASSIGNMENT 3: UNIT TESTING: MINE CARD.\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	/* TEST 1: expect fail. Invalid choice 1. Condition 1 if < Copper */
	qty_errors = unitMineTest1();

	/* TEST 2: expect fail. Invalid choice 2. Condition 2 if > Gold */
	qty_errors += unitMineTest2();

	/* TEST 3: expect fail. Invalid choice 2. Not in kindom too large */
	qty_errors += unitMineTest3();

	/* TEST 4: expect fail. Invalid choice 2. Not in kindom too small */
	qty_errors += unitMineTest4();

	/* TEST 5: expect success */
	qty_errors += unitMineTestDeluxe(5, 7, 0, copper, silver, &backup, &G, kingdomCards, -1);

	/* TEST 6: expect success */
	qty_errors += unitMineTestDeluxe(6, 7, 0, silver, gold, &backup, &G, kingdomCards, -1);

	/* TEST 7: expect fail: choice1 not trashed and found in hand still */
	qty_errors += unitMineTestDeluxe(7, 4, 0, copper, silver, &backup, &G, kingdomCards, -1);

	/* TEST 8: expect fail: choice1 not trashed & found in hand still */
	qty_errors += unitMineTestDeluxe(8, 5, 0, silver, gold, &backup, &G, kingdomCards, -1);

	/* TEST 9: expect fail: choice1 found in discard*/
	qty_errors += unitMineTestDeluxe(9, 7, 0, copper, silver, &backup, &G, kingdomCards, 0);

	/* TEST 10: expect fail: choice2 in discard not in hand */
	qty_errors += unitMineTestDeluxe(10, 7, 0, copper, silver, &backup, &G, kingdomCards, 8);

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

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
void expectedReturn(
	int expect_flag, 
	int actual,
	int moneyToTrashHandLocation, 
	int moneyToGain, 
	int discardCardFromThisHandLocation
)
{
	if ((expect_flag == EXPECT_ZERO && actual != 0) ||
	    (expect_flag == EXPECT_NEG_ONE && actual != -1)){

		printf("ERROR: mineCard returned %d but expected ", actual);
		if (expect_flag == EXPECT_ZERO) {
			printf(" 0\n");
		}
		else if (expect_flag == EXPECT_NEG_ONE) {
			printf(" -1\n");
		}

		char choice1Buffer[MAX_STRING_LENGTH];
		char choice2Buffer[MAX_STRING_LENGTH];
		char handPosBuffer[MAX_STRING_LENGTH];

		memset(choice1Buffer, '\0', sizeof(choice1Buffer));
		cardNumToName(moneyToTrashHandLocation, choice1Buffer);

		memset(choice2Buffer, '\0', sizeof(choice2Buffer));
		cardNumToName(moneyToGain, choice2Buffer);

		memset(handPosBuffer, '\0', sizeof(handPosBuffer));
		cardNumToName(discardCardFromThisHandLocation, handPosBuffer);

		printf("choice1 was: %s\n", choice1Buffer);
		printf("choice2 was: %s\n", choice2Buffer);
		printf("handPos was: %s\n", handPosBuffer);

	}
}

/****************************************************************
* Description:
*	Function to set up and execute a call to unit test the
*	mineCard() function.
*
* Designed to handle test cases 5 - 10. 
* Adaptable random testing elsewhere.
*
* **************************************************************/
int unitMineTestDeluxe(
	/* start params */
	int test_no, 
	int newHandSize, 
	int choice1pos, // choice1
	int choice1card,
	int choice2, // choice2
	struct gameState* previousState,
	struct gameState* state, // state
	int* kingdomToUse,
	int handPos // handPos
	/* end params */
){ 

	int numErrors = 0;

	/* # players, cards to use, seed, gameState */
	initTestGame(2, kingdomToUse, 1, state);

	int currentPlayer = 0;

	// CALL _prepOnePlayer
	_prepOnePlayer(
		currentPlayer,
		state,
		newHandSize,	// 5
		FILL_SAME,			// fill hand with the same 
		-1					// use -1 to fill a.k.a. unused
	);

	setAtHandPos(currentPlayer, state, choice1card, choice1pos);

	/* BACK UP STATE BEFORE CALL */
	memset(previousState, '\0', sizeof(*previousState));
	previousState = state;

		
	/* INPUTS FOR MINE CARD */
	int moneyToTrashHandLocation = choice1pos; // choice1
	int moneyToGain = choice2; // choice2
	int discardCardFromThisHandLocation = handPos; // a.k.a. handPos of mineCard() call

	/** ==> CALL <================================================= */
	mineCard(
		moneyToTrashHandLocation, 
		moneyToGain, 
		state, 
		discardCardFromThisHandLocation
	);
	/** ==> END CALL <================================================= */

	printf("MINE: RUNNING SUB-TEST %d OUTPUT ==> **************************************************************\n", test_no);

	char choice1Buffer[MAX_STRING_LENGTH];
	char choice2Buffer[MAX_STRING_LENGTH];

	/* COMPARE TRASH PILE COUNT & CONTENTS */
	if( previousState->trashCount + 1 != state->trashCount)
	{
		printf("MINE: trashCount is not + 1 previous.\n");
		numErrors++;

		// if top of trashPile unchanged
		if (previousState->trashPile[previousState->trashCount] == state->trashPile[state->trashCount - 1])
		{
			memset(choice1Buffer, '\0', sizeof(choice1Buffer));
			cardNumToName(choice1card, choice1Buffer);
			printf("      expected: %s\n", choice1Buffer);

			memset(choice2Buffer, '\0', sizeof(choice2Buffer));
			cardNumToName(state->trashPile[state->trashCount - 1], choice2Buffer);
			printf("      found: %s\n", choice2Buffer);

			numErrors++;
		}
		// if top of trashPile changed but..
		else if (previousState->trashPile[previousState->trashCount] != state->trashPile[state->trashCount - 1])
		{
			// top of trashPile doesn't contain the card at choice1 (choice1card)
			if (state->trashPile[state->trashCount - 1] != choice1card)
			{
				printf("MINE: top of trash conents changed, but NOT as expected.\n");

				memset(choice1Buffer, '\0', sizeof(choice1Buffer));
				cardNumToName(choice1card, choice1Buffer);
				printf("      expected: %s\n", choice1Buffer);

				memset(choice2Buffer, '\0', sizeof(choice2Buffer));
				cardNumToName(state->trashPile[state->trashCount - 1], choice2Buffer);
				printf("      found: %s\n", choice2Buffer);

				numErrors++;
			}
		}
	}

	/* COMPARE HAND COUNT & CONTENTS */
	if (previousState->handCount[currentPlayer] == state->handCount[currentPlayer])
	{
		if (previousState->hand[currentPlayer][moneyToTrashHandLocation] == copper ||
			previousState->hand[currentPlayer][moneyToTrashHandLocation] == silver ||
			previousState->hand[currentPlayer][moneyToTrashHandLocation] == gold) {

			printf("MINE: You had valid money at position %d but your handCount didn't change.\n", moneyToTrashHandLocation);
			numErrors++;

			// counter & bool if found choice2 in hand
			int foundInHand = 0;

			// check if choice2 found in hand after call & tally how many
			int i;
			for (i = 0; i < state->handCount[currentPlayer]; i++)
			{
				if (state->hand[currentPlayer][i] == choice2)
				{
					foundInHand++;
				}
			}

			if (foundInHand < 1)
			{
				memset(choice2Buffer, '\0', sizeof(choice2Buffer));
				cardNumToName(choice2, choice2Buffer);
				printf("MINE: choice2 %s not found in hand.\n", choice2Buffer);
				numErrors++;
			}
			else if (foundInHand > 0)
			{
				int wasAlreadyThere = 0;

				// count how many were there in previous state
				int j;
				for (j = 0; j < previousState->handCount[currentPlayer]; j++)
				{
					if (previousState->hand[currentPlayer][j] == choice2)
					{
						wasAlreadyThere++;
					}
				}

				if (wasAlreadyThere + 1 != foundInHand)
				{
					memset(choice2Buffer, '\0', sizeof(choice2Buffer));
					cardNumToName(choice2, choice2Buffer);

					printf("MINE: found %s in hand, but the quantity is wrong.\n", choice2Buffer);
					printf("      You had %d before\n", wasAlreadyThere);
					printf("      now you have %d\n", foundInHand);

					numErrors++;
				}
			}

		}
	}

	/* COMPARE DISCARD COUNT & CONTENTS */
	if (previousState->discardCount[currentPlayer] != state->discardCount[currentPlayer])
	{
		printf("MINE: mineCard() function SHOULD NOT change discard pile.\n");
		if (previousState->discard[currentPlayer][previousState->discardCount[currentPlayer]] !=
			state->discard[currentPlayer][state->discardCount[currentPlayer]]){ 
			if (state->discard[currentPlayer][state->discardCount[currentPlayer]] == choice1card){
				printf("      found choice1 in discard.\n");
				numErrors++;
			}
			else if (state->discard[currentPlayer][state->discardCount[currentPlayer]] == choice2) {
				printf("      found choice2 in discard.\n");
			}
			
			memset(choice2Buffer, '\0', sizeof(choice2Buffer));
			cardNumToName(state->discard[currentPlayer][state->discardCount[currentPlayer]], choice2Buffer);
			printf("      top of discard is: %s\n", choice2Buffer);
			numErrors++;
		}

	}
	printf("\n\n");
	return numErrors;
}
// *****************************************************************************************
int unitMineTest1()
{
	int numErrors = 0;
	int result = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, province, smithy };

	// Game States 
	struct gameState G;

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
		newHandSize,	// 5
		FILL_SAME,			// fill hand with the same 
		-1					// use -1 to fill a.k.a. unused
	);

	/* set an estate card at handPos 0 as choice1 .. to fail */
	setAtHandPos(currentPlayer, &G, province, 0);

	/* INPUTS FOR MINE CARD */
	int moneyToTrashHandLocation = 0; // choice1
	int moneyToGain = copper; // choice2
	int discardCardFromThisHandLocation = -1; // a.k.a. handPos of mineCard() call

	/** ==> CALL <================================================= */
	result = mineCard(
		moneyToTrashHandLocation,
		moneyToGain,
		&G,
		discardCardFromThisHandLocation
	);
	/** ==> END CALL <================================================= */

	if (result != -1)
	{
		printf("MINE: Invalid choice1 didn't return -1..\n");
		printf("    ..Invalid choice1 accepted as card to trash.\n");
		numErrors++;
	}

	return numErrors;
}

int unitMineTest2()
{
	int numErrors = 0;
	int result = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, province, smithy };

	// Game States 
	struct gameState G;

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
		newHandSize,	// 5
		FILL_SAME,			// fill hand with the same 
		-1					// use -1 to fill a.k.a. unused
	);

	/* set an estate card at handPos 0 as choice1 .. to fail */
	setAtHandPos(currentPlayer, &G, copper, 0);

	/* INPUTS FOR MINE CARD */
	int moneyToTrashHandLocation = 0; // choice1
	int moneyToGain = adventurer; // choice2
	int discardCardFromThisHandLocation = -1; // a.k.a. handPos of mineCard() call

	/** ==> CALL <================================================= */
	result = mineCard(
		moneyToTrashHandLocation,
		moneyToGain,
		&G,
		discardCardFromThisHandLocation
	);
	/** ==> END CALL <================================================= */

	if (result != -1)
	{
		printf("MINE: Invalid choice2 didn't return -1..\n");
		printf("    ..Invalid choice2 accepted as card to gain to hand.\n");
		numErrors++;
	}

	return numErrors;
}

int unitMineTest3()
{
	int numErrors = 0;
	int result = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, province, smithy };

	// Game States 
	struct gameState G;

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
		newHandSize,	// 5
		FILL_SAME,			// fill hand with the same 
		-1					// use -1 to fill a.k.a. unused
	);

	/* set an estate card at handPos 0 as choice1 .. to fail */
	setAtHandPos(currentPlayer, &G, copper, 0);

	/* INPUTS FOR MINE CARD */
	int moneyToTrashHandLocation = 0; // choice1
	int moneyToGain = 27; // choice2
	int discardCardFromThisHandLocation = -1; // a.k.a. handPos of mineCard() call

	/** ==> CALL <================================================= */
	result = mineCard(
		moneyToTrashHandLocation,
		moneyToGain,
		&G,
		discardCardFromThisHandLocation
	);
	/** ==> END CALL <================================================= */

	if (result != -1)
	{
		printf("MINE: Invalid choice2 didn't return -1..\n");
		printf("    ..Invalid choice2 of 27 accepted as card to gain to hand.\n");
		numErrors++;
	}

	return numErrors;
}

int unitMineTest4()
{
	int numErrors = 0;
	int result = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, province, smithy };

	// Game States 
	struct gameState G;

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
		newHandSize,	// 5
		FILL_SAME,			// fill hand with the same 
		-1					// use -1 to fill a.k.a. unused
	);

	/* set an estate card at handPos 0 as choice1 .. to fail */
	setAtHandPos(currentPlayer, &G, copper, 0);

	/* INPUTS FOR MINE CARD */
	int moneyToTrashHandLocation = 0; // choice1
	int moneyToGain = -1; // choice2
	int discardCardFromThisHandLocation = -1; // a.k.a. handPos of mineCard() call

	/** ==> CALL <================================================= */
	result = mineCard(
		moneyToTrashHandLocation,
		moneyToGain,
		&G,
		discardCardFromThisHandLocation
	);
	/** ==> END CALL <================================================= */

	if (result != -1)
	{
		printf("MINE: Invalid choice2 didn't return -1..\n");
		printf("    ..Invalid choice2 of -1 accepted as card to gain to hand.\n");
		numErrors++;
	}

	return numErrors;
}
/* EOF */

