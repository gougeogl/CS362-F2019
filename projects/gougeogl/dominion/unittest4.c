/***************************************************************
** FileName: unittest4.c
** Author: Glen Gougeon
** Class : CS362 Software Engineering II
** Date : 11 - 15 - 2019
** Last Mod : 11 - 15 - 2019
*
** Description : Assignment 3 : Unit Testing :
*		Refactored code for Tribute, has 2 bugs I introduced.
*		The following test code should catch the errors while
*		executing 80 % branch and function coverage according
*		to gcov coverage tool.
*
** To Compile:	Use included 'Makefile'
*		compile command : make unittest4
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

enum TEST_FLAGS
{
	SAME_HAND = 800,
	DIFFERENT_HAND,
	SAME_DECK,
	DIFFERENT_DECK,
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
void prepPlayers(int fromPlayer, int toPlayer, struct gameState* state, int newHandSize, int fill_type, int card);
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

// SAVE VALUES PROTO-TYPES
void savePreviousHandCounts(int* container, struct gameState* state );
void saveTop2Deck(int player, struct gameState* state, int* topTwo);
void saveTop2Discard(int player, struct gameState* state, int* topTwo);
int compareTopsAfter(int player, int* deckTops, int* discardTops);

/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards();

int tributeTest1();

int main()
{
	tributeTest1();
	printf("\n\n");
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
* prepPlayers 
* calls _prepOnePlayer for players 'fromPlayer' to 'toPlayer'  
*
****************************************************************************/
void prepPlayers(
	int fromPlayer, 
	int toPlayer, 
	struct gameState* state, 
	int newHandSize, 
	int fill_type, 
	int card
)
{
	int i;
	for (i = fromPlayer; i <= toPlayer; i++)
	{
		_prepOnePlayer(i, state, newHandSize, fill_type, card);
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

void fillDeck(int player, struct gameState* state, int newDeckSize )
{
	int card = 0;
	char name[MAX_STRING_LENGTH];

	printf("START: FILL DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	int i = 0;
	while (i < newDeckSize)
	{
		card = _rand_of_kingdomCards();
		memset(name, '\0', sizeof(name));

		cardNumToName(card, name);
		printf("TRACE: card#%d: %s\n", card ,name);

		state->deck[player][i] = card;
		memset(name, '\0', sizeof(name));

		cardNumToName(state->deck[player][i], name);
		printf("TRACE: state->deck[player %d][i %d]:%s\n", player, i, name);
		i++;
	}
	state->deckCount[player] = newDeckSize;
	printf("TRACE: state->deckCount[player %d]: %d\n", player, state->deckCount[player] );
	printf("END: FILL DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\n");
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
* numActions
* Check is numActions of a player is same before some saved gameState (before) and
* if numActions is not +1 previous  will print error, nothing otherwise.
*
****************************************************************************/
int compareNumActionsTribute(int player, struct gameState* before, struct gameState* after)
{
	int result = 0;

	if (before->numActions + 2 != after->numActions)
	{
		printf("Player %d\'s numActions is not +2 previous !\n", player);
		printf("BEFORE: numActions: %d\n", before->numActions);
		printf("AFTER:  numActions: %d\n", after->numActions);
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
* save - top - 2 - deck
* Saves the top 2 cards in the player's deck
*
****************************************************************************/
void saveTop2Deck(int player, struct gameState* state, int* topTwo )
{
	//int* result = NULL;
	//int topTwo[2] = { 0 };
	int topCard = state->deckCount[player];
	topCard--;
	int nextTop = topCard;
	nextTop--;
	int deckCount = state->deckCount[player];

	topTwo[0] = state->deck[player][topCard];
	topTwo[1] = state->deck[player][nextTop];

	if (DEBUG_TOP2_DECK)
	{
		printf("TRACE: saveTop2Deck\n");
		printf("player %d\n", player);
		printf("state->deckCount[player]:%d\n", state->deckCount[player] );
		printf("deckCount:%d\n", deckCount);

		printf("ORIGINAL: state->deck[player][state->deckCount[player]]:%d\n", 
			state->deck[player][state->deckCount[player]]);
		printf("COPY OF: state->deck[player][deckCount]:%d\n", 
			state->deck[player][state->deckCount[player]]);

		deckCount--;
		printf("DECREMENT 1: deckCount:%d\n", deckCount);
		printf("DECREMENT 1: state->deck[player][deckCount]:%d\n", 
			state->deck[player][deckCount]);

		printf("topTwo[0]: %d vs. state->deck[player %d][deckCount]: %d\n",
			topTwo[0], player, state->deck[player][deckCount]);

		deckCount--;
		printf("DECREMENT 2: deckCount:%d\n", deckCount);
		printf("DECREMENT 2: state->deck[player][deckCount]:%d\n", 
			state->deck[player][deckCount]);
		printf("topTwo[1]: %d vs. state->deck[player %d][deckCount]: %d\n",
			topTwo[1], player, state->deck[player][deckCount]);
	}

	//result = topTwo;
	printf("topTwo[0]: %d\n", topTwo[0] );
	printf("topTwo[1]: %d\n", topTwo[1] );
	//return result;
}

/* *************************************************************************
* save - top - 2 - discard
* Saves the top 2 cards in the player's discard
*
****************************************************************************/
void saveTop2Discard(int player, struct gameState* state, int* topTwo)
{
	//int* result = NULL;
	//int topTwo[2] = { 0 };
	int topCard = state->discardCount[player];
	topCard--;
	int nextTop = topCard;
	nextTop--;
	int discardCount = state->discardCount[player];

	topTwo[0] = state->discard[player][topCard];
	topTwo[1] = state->discard[player][nextTop];

	if (DEBUG_TOP2_DISCARD)
	{
		printf("TRACE: saveTop2Discard\n");
		printf("player %d\n", player);
		printf("state->discardCount[player]:%d\n", state->discardCount[player]);
		printf("discardCount:%d\n", discardCount);

		printf("ORIGINAL: state->discard[player][state->discardCount[player]]:%d\n",
			state->discard[player][state->discardCount[player]]);
		printf("COPY OF: state->discard[player][discardCount]:%d\n",
			state->discard[player][state->discardCount[player]]);

		discardCount--;
		printf("DECREMENT 1: discardCount:%d\n", discardCount);
		printf("DECREMENT 1: state->discard[player][discardCount]:%d\n",
			state->discard[player][discardCount]);

		printf("topTwo[0]: %d vs. state->discard[player %d][discardCount]: %d\n",
			topTwo[0], player, state->discard[player][discardCount]);

		discardCount--;
		printf("DECREMENT 2: discardCount:%d\n", discardCount);
		printf("DECREMENT 2: state->discard[player][discardCount]:%d\n",
			state->discard[player][discardCount]);
		printf("topTwo[1]: %d vs. state->discard[player %d][discardCount]: %d\n",
			topTwo[1], player, state->discard[player][discardCount]);
	}

	//result = topTwo;
	printf("topTwo[0]: %d\n", topTwo[0]);
	printf("topTwo[1]: %d\n", topTwo[1]);
	//return result;
}

/* ***********************************************************************
* Used after calls to saveTop2Deck() & saveTop2Discard().
* Compares both sets of value for equality. 
* If all values equal in both sets, prints nothing and returns 1.
* If not, prints both sets of values, returns -1.
****************************************************************************/
int compareTopsAfter(int player, int* deckTops, int* discardTops)
{
	int result = -1;
	if (	( deckTops[0] != discardTops[0] && 
		  deckTops[0] != discardTops[1] ) ||
		( deckTops[1] != discardTops[0] && 
		deckTops[1] != discardTops[1]   ) 	){

		char deck1[MAX_STRING_LENGTH];
		memset(deck1, '\0', sizeof(deck1));
		cardNumToName(deckTops[0], deck1);

		char deck2[MAX_STRING_LENGTH];
		memset(deck2, '\0', sizeof(deck2));
		cardNumToName(deckTops[1], deck2);

		char disc1[MAX_STRING_LENGTH];
		memset(disc1, '\0', sizeof(disc1));
		cardNumToName(discardTops[0], disc1);

		char disc2[MAX_STRING_LENGTH];
		memset(disc2, '\0', sizeof(disc2));
		cardNumToName(discardTops[0], disc2);

		printf("Player %d\'s previous top 2 deck cards..\n", player);
		printf(".. do not match the top 2 cards in their discard !\n");
		printf("deck[0]: %s != discard[0]: %s || discard[1]: %s\n",
			deck1, disc1, disc2);
		printf("-- OR --\n");
		printf("deck[1]: %s != discard[0]: %s || discard[1]: %s\n",
			deck2, disc1, disc2);
	}
	else {
		result = 1;
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

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
int tributeTest1()
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
		printf("unittest4.c\n");
		printf("TRIBUTE TEST 1: Rules:\n");
		printf("              : Use same kingdomCards deck as in Baron Card testing.\n");
		printf("              : 2 Players.\n");
		//printf("	          : Each player starts with 5 random cards in hand.\n");
		//printf("	          : Randomly assign cards from kingdomCards to each player.\n"); 
		printf("OUTPUT:\n\n");
	}

	/* # players, cards to use, seed, gameState */
	initTestGame(2, kingdomCards, seed, &G);

	/* set up all players with 5 random cards in each hand */
	prepPlayers(0, 1, &G, 5, FILL_DIFF, -1);

	/* set a tribute card in player zero's hand at position zero */
	setAtHandPos(0, &G, tribute, 0);

	/* set deck for players according to size */
	//printf("FILL DECK PLAYER 0 <-----------------------------------------\n");
	//fillDeck(0, &G, 5);
	printf("FILL DECK PLAYER 1 <-----------------------------------------\n");
	fillDeck(1, &G, 5);

	/* SAVE HAND COUNTS  */
	int handBox[MAX_PLAYERS];
	savePreviousHandCounts(handBox, &G);

	/* SAVE TOP 2 DECK */
	printf("BEFORE: saveTop2Deck() PLAYER 1 <------------------------\n");
	int topTwoDeck[2] = { 0 };
	saveTop2Deck(1, &G, topTwoDeck );

	//int discardCard(int handPos, int currentPlayer, struct gameState *state)
	/*
	printf("DISCARD FIRST CARD FROM PLAYER 1 <------------------------\n");
	discardCard(0, 1, &G);
	printf("DISCARD SECOND CARD FROM PLAYER 1 <------------------------\n");
	discardCard(1, 1, &G);
	*/

	/* SAVE TOP 2 DISCARD BEFORE */
	printf("BEFORE: saveTop2Discard() PLAYER 1 <------------------------\n");
	int beforeTopTwoDiscard[2] = { 0 };
	saveTop2Discard(1, &G, beforeTopTwoDiscard);

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	printf("TRACE: Player 1 Deck before call to tribute..\n");
	printDeck(1, &G);
	printf("TRACE: Player 1 Discard before call to tribute..\n");
	printDiscard(1, &G);
	/** ==> CALL <================================================= */
	tributeCard(&G);

	/* SAVE TOP 2 DISCARD AFTER */
	printf("AFTER: saveTop2Discard() PLAYER 1 <------------------------\n");
	int afterTopTwoDiscard[2] = { 0 };
	saveTop2Discard(1, &G, afterTopTwoDiscard);

	printf("TRACE: Player 1 Deck after call to tribute..\n");
	printDeck(1, &G);
	printf("TRACE: Player 1 Discard after call to tribute..\n");
	printDiscard(1, &G);

	printf("PLAYER 1 TOP 2 DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("topTwoDeck[0]: %d\n", topTwoDeck[0]);
	printf("topTwoDeck[1]: %d\n", topTwoDeck[1]);

	/* PRINT TOP TWO DISCARD BEFORE AND AFTER */
	printf("BEFORE: beforeTopTwoDiscard [ ] MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("beforeTopTwoDiscard[0]: %d\n", beforeTopTwoDiscard[0]);
	printf("beforeTopTwoDiscard[1]: %d\n", beforeTopTwoDiscard[1]);
	printf("\n");
	printf("AFTER: afterTopTwoDiscard [ ] MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("afterTopTwoDiscard[0]: %d\n", afterTopTwoDiscard[0]);
	printf("afterTopTwoDiscard[1]: %d\n", afterTopTwoDiscard[1]);

	//int compareTopsAfter(int player, int* deckTops, int* discardTops)
	printf("CALLING: compareTopsAfter <==========================================\n");
	result = compareTopsAfter(1, topTwoDeck, afterTopTwoDiscard);

	printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	printf("PLAYER 0 DECK AFTER\n");
	printDeck(0, &G);
	printf("PLAYER 0 DISCARD AFTER\n");
	printDiscard(0, &G);
	printf("\n\n");

	return result;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/


