/****************************************************************
** FileName: unittest1.c
** Author: Glen Gougeon
** Class: CS362 Software Engineering II
** Date: 10-25-2019
** Last Mod: 10-25-2019
*
** Description:	Assignment 3: Unit Testing : 
*		Refactored code for Baron, has 2 bugs I introduced. 
*		The following test code should catch the errors while
*		executing 80% branch and function coverage according
*		to gcov coverage tool.
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

/* PROTO-TYPE DECLARATIONS */
int testBaron(int choice, struct gameState* state, int shouldDump, int handPos);

void initTestGame(struct gameState* game, int* kDeck, int mySeed);
void resetHand(struct gameState* dState, int newHandSize);
void setHandPos(struct gameState* state, int card, int handPos);

int main( int argc, char* argv[] )
{
	int overall_stat = -1;
	int check = -1; 

	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Set up Game State 
	struct gameState G;

	//int seed = atoi(argv[1]);
	int seed = 1; 

	//int baronCard(int choice1, struct gameState *state)

	// test execution for every position in hand, with an estate in that position
	int i;
	for (i = 0; i < 5; i++)
	{
		initTestGame(&G, kingdomCards, seed);

		check = testBaron(1, &G, 1, i); /* choose to use estate, dump current hand, place estate at pos i */
		if (check == -1)
		{
			printf("Test %d failed.\n", i + 1);
		}
		else { overall_stat = 0; }
	}

	if( overall_stat == -1 )
	{ printf("ALL Baron Tests Failed!!\n"); }

	return 0;
}

/* returns 0 if pass, -1 if fail */
int testBaron(int choice, struct gameState* state, int shouldDump, int handPos)
{
	int test_1_stat = -1; // return flag

	/* SEE resetHand() : runs if shouldDump flag == 1 */
	if (shouldDump == 1)
	{
		resetHand(state, 5); /* <-- make hand size 5 */
	}

	// place new estate card at position indicated
	setHandPos(state, estate, handPos);

	test_1_stat = baronCard(choice, state);

	return test_1_stat;
}

/* ** TEST SUITE FUNCTIONS *************************************************************/
void initTestGame(struct gameState* game, int* kDeck, int mySeed )
{
	memset(game, '\0', sizeof(struct gameState));   // clear mem of 
	
	int checkInit = initializeGame(2, kDeck, mySeed, game); // initialize 2 player game 
	
	if(checkInit == -1)
	{ printf("Bad game initialization.\n"); }

	int currentPlayer = whoseTurn(game);

}

/* Sets current player's handCount to newHandSize, then
	overwrites everything in hand with -1 */
void resetHand(struct gameState* dState, int newHandSize)
{
	int currentPlayer = whoseTurn(dstate);
	game->handCount[currentPlayer] = newHandSize;

	int i;
	for (i = 0; i < game->handCount[currentPlayer]; i++)
	{
		game->hand[currentPlayer][i] = -1;
	}

}

// adds indicated card in current player's hand at handPos
void setHandPos(struct gameState* state, int card, int handPos)
{
	int currentPlayer = whoseTurn(state);
	state->hand[currentPlayer][handPos] = card;
}