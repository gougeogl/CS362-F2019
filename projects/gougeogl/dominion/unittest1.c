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
int testBaron(int choice, struct gameState* state, int shouldDump, int handCount, int handPos);
int zero_or_one();

void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game );

/* ESTATE CARD RELATED */
void removeEstatesDeck(struct gameState *gameS);
void setEstateSupply(struct gameState *state, int qtyEstates);
void removeEstatesDiscard(struct gameState *st);

/* HAND RELATED */
void resetHand(struct gameState* dState);
void setHandCount(struct gameState* state, int newHandSize);
void setHandPos(struct gameState* state, int card, int handPos);

void get_stats_before_call(struct gameState* my_game, int my_arr[], int card);

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

	int stats[13];
	//initTestGame(2, kingdomCards, seed, &G);
	//baronCard(1, &G);	
	// test execution for every position in hand, with an estate in that position
	int i;
	for (i = 0; i < 5; i++)
	{
		// checks G is initialized properly.. calls initializeGame() from dominion.h
		initTestGame(2, kingdomCards, seed, &G);
		
		// for testing purposes..assume there is only 1 card in entire supply
		removeEstatesDeck(&G);
		removeEstatesDiscard(&G);
		setEstateSupply(&G, 1); /* <-- add 1 estate to supply here */

		// collect all info. of gameState before call
		get_stats_before_call(&G, stats, estate);

		check = testBaron( zero_or_one() , &G, 1, 5, i); /* choose to use estate, dump current hand, place estate at pos i of 5 */
		if (check == -1)
		{
			printf("Test %d failed.\n", i + 1);
			overall_stat = check;
		}
		else { overall_stat = 0; }
	}

	if( overall_stat == -1 )
	{ printf("ALL Baron Tests Failed!!\n"); }
	
	return 0;
}

/* returns 0 if pass, -1 if fail */
int testBaron(int choice, struct gameState* state, int shouldDump, int handCount, int handPos)
{
	int test_stat = -1; // return flag

	/* SEE resetHand() : runs if shouldDump flag == 1 */
	if (shouldDump == 1)
	{
		resetHand(state); /* <-- make hand size 5 */
	}

	// use 5 cards for size of hand
	setHandCount(state, handCount);

	// place new estate card at position indicated
	setHandPos(state, estate, handPos);

	test_stat = baronCard(choice, state); /* <= MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM CALL TO BARON !! */

	return test_stat;
}

int zero_or_one()
{
	int num = rand() % ( 1 - 0 + 1 ) + 0;
	return num;
}

/* ** TEST SUITE FUNCTIONS *************************************************************/

/* This function adds memset initialization, and prints error if fail  */
void initTestGame(int players, int* kDeck, int mySeed, struct gameState* game )
{
	memset(game, '\0', sizeof(struct gameState));   // clear mem of 
	
	int checkInit = initializeGame(2, kDeck, mySeed, game); // initialize 2 player game 
	
	if(checkInit == -1)
	{ printf("Bad game initialization.\n"); }

}

// set player to remove all estates from current player's deck  
void removeEstatesDeck(struct gameState* gameS)  
{  
	int currentPlayer = whoseTurn(gameS);

	int i = 0;   
	while(i < gameS->deckCount[currentPlayer])   
	{    
		if(gameS->deck[currentPlayer][i] == estate)    
		{     
			gameS->deck[currentPlayer][i] = -1;     
			gameS->deckCount[currentPlayer] -= 1;

		}   
		i++;
	}  
} 

// eliminate all estates from discard.   
void removeEstatesDiscard(struct gameState *st)  
{   
	int currentPlayer = whoseTurn(st);   
	
	int i = 0;   
	while(i < st->discardCount[currentPlayer])   
	{    
		// search for an estate in discard of current player    
		if(st->discard[currentPlayer][i] == estate)    
		{     
			// if found, wipe out, set to -1     
			st->discard[currentPlayer][i] = -1;    
			st->discardCount[currentPlayer] -= 1;
		}   
		i++;
	}  
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
		dState->handCount[currentPlayer] -= 1;
	}
}

void setHandCount(struct gameState* state, int newHandSize )
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

/**********************************************************************************************/
void get_stats_before_call(struct gameState* my_game, int my_arr[], int card)
{
	int currentPlayer = whoseTurn(my_game);

	my_arr[0] = my_game->supplyCount[card]; /* how many of card left in supply */ 
	my_arr[1] = my_game->playedCards[my_game->playedCardCount]; /* # cards played */ 
	my_arr[2] = my_game->playedCardCount;  /* how many cards played */ 

	my_arr[3] = my_game->numBuys; /* buys */ 
	my_arr[4] = my_game->numActions; /* actions */ 
	my_arr[5] = my_game->coins; /* coins */ 


	my_arr[6] = my_game->trashPile[my_game->trashCount]; /* top of trash */ 
	my_arr[7] = my_game->trashCount; /* trash size */ 

	my_arr[8] = my_game->discard[currentPlayer][my_game->discardCount[currentPlayer] ]; /* top of discard */ 
	my_arr[9] = my_game->discardCount[currentPlayer]; /* discard count */ 

	my_arr[10] = my_game->deck[currentPlayer][my_game->discardCount[currentPlayer] ]; /* top of deck */ 
	my_arr[11] = my_game->deckCount[currentPlayer]; /* deck count */ 

	my_arr[12] = my_game->handCount[currentPlayer]; /*  hand count */ 

}
