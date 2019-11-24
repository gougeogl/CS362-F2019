/***************************************************************
** FileName: unittest4.c
** Author: Glen Gougeon
** Class : CS362 Software Engineering II
** Date : 11 - 16 - 2019
** Last Mod : 11 - 23 - 2019
*
** Description : Assignment 5 : Unit Testing :
*		Refactored code by Reuben Youngblom.
*		The following test code should catch the errors while
*		executing 80 % branch and function coverage according
*		to gcov coverage tool.
*
** To Compile:	Use included 'Makefile'
*		compile command : make unittest4
*		generate output file: make unittest4results
*		output: unittest4results.out
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
#define DEBUG_TRIBUTE_TEST_1 0
#define DEBUG_TRIBUTE_TEST_2 0
#define DEBUG_TRIBUTE_TEST_3 0
#define DEBUG_TRIBUTE_TEST_4 0

#define DEBUG_RANDOM 0

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
int compareTopsAfter(int player, int* deckTops, int* discardTops);
void setTypesFoundTribute(int* oldDeckTops, int* coin_count, int* draw_card_count, int* num_actions_count);

// SAVE VALUES PROTO-TYPES
void savePreviousHandCounts(int* container, struct gameState* state);
void savePreviousDeckCounts(int* container, struct gameState* state);
void savePreviousDiscardCounts(int* container, struct gameState* state);

void saveTop2Deck(int player, struct gameState* state, int* topTwo);
void saveTop2Discard(int player, struct gameState* state, int* topTwo);

/* selects a random card from kingdomCards deck */
int _rand_of_kingdomCards();

/* generate an int in range [min..max] */
int _genRandRange(int min, int max);

/* RANDOM TEST */
int unitTributeTest1();
int unitTributeTest2();
int unitTributeTest3();
int unitTributeTest4();

int main()
{
	int qty_errors = 0;

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("ASSIGNMENT 3: UNIT TESTING: TRIBUTE CARD.\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	qty_errors = unitTributeTest1();
	qty_errors += unitTributeTest2();
	qty_errors += unitTributeTest3();
	qty_errors += unitTributeTest4();

		if (qty_errors == 0)
		{
			printf("ALL TRIBUTE TESTS PASSED !!\n\n");
		}
		else
			printf("TRIBUTE TEST FOUND: %d ERRORS !\n\n", qty_errors);

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

	if(FILL_DECK_DEBUG){
		printf("START: FILL DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
	}

	int i = 0;
	while (i < newDeckSize)
	{
		card = _rand_of_kingdomCards();
		memset(name, '\0', sizeof(name));

		cardNumToName(card, name);

		if(FILL_DECK_DEBUG){
			printf("TRACE: card#%d: %s\n", card ,name);
		}

		state->deck[player][i] = card;
		memset(name, '\0', sizeof(name));

		cardNumToName(state->deck[player][i], name);

		if(FILL_DECK_DEBUG){
			printf("TRACE: state->deck[player %d][i %d]:%s\n", player, i, name);
		}

		i++;
	}
	state->deckCount[player] = newDeckSize;
	if(FILL_DECK_DEBUG){
		printf("TRACE: state->deckCount[player %d]: %d\n", player, state->deckCount[player] );
		printf("END: FILL DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n\n");
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
* save - previous - deck - counts
* Saves deckCounts of all players after initial testing setup
*
****************************************************************************/
void savePreviousDeckCounts(int* container, struct gameState* state)
{
	int numPlayers = state->numPlayers;

	int i;
	for (i = 0; i < numPlayers; i++)
	{
		container[i] = state->deckCount[i];
	}
}

/* *************************************************************************
* save - previous - discard - counts
* Saves discardCounts of all players after initial testing setup
*
****************************************************************************/
void savePreviousDiscardCounts(int* container, struct gameState* state)
{
	int numPlayers = state->numPlayers;

	int i;
	for (i = 0; i < numPlayers; i++)
	{
		container[i] = state->discardCount[i];
	}
}

/* *************************************************************************
* save - top - 2 - deck
* Saves the top 2 cards in the player's deck
*
****************************************************************************/
void saveTop2Deck(int player, struct gameState* state, int* topTwo )
{
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

		printf("END: topTwo[0]: %d\n", topTwo[0] );
		printf("END: topTwo[1]: %d\n", topTwo[1] );
	}

}

/* *************************************************************************
* save - top - 2 - discard
* Saves the top 2 cards in the player's discard
*
****************************************************************************/
void saveTop2Discard(int player, struct gameState* state, int* topTwo)
{
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

		printf("END: topTwo[0]: %d\n", topTwo[0]);
		printf("END: topTwo[1]: %d\n", topTwo[1]);
	}

}

/* ***********************************************************************
* Used after calls to saveTop2Deck() & saveTop2Discard().
* Compares both sets of value for equality. 
* If all values equal in both sets, prints nothing and returns 1.
* If not, prints both sets of values, returns -1.
****************************************************************************/
int compareTopsAfter(int player, int* deckTops, int* discardTops)
{
	int result = 0;
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
	
		result = -1;
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
int unitTributeTest1()
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
		printf("unittest4.c\n");
		printf("TRIBUTE TEST 1: Rules:\n");
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
		int nextPlayer = currentPlayer + 1;

		// size of hand to use
		int newHandSize = 5;

		/* set up all players with 5 random cards in each hand */
		prepPlayers(
			currentPlayer,		// prep from this player
			nextPlayer,		// prep to this player
			&G,
			newHandSize,
			FILL_DIFF,
			-1				// not used..only for filling with same card
		);

		// position to place a single kingdom card into hand
		int newHandPos = _genRandRange(0, newHandSize);

		/* set a tribute card in player zero's hand at position zero */
		setAtHandPos(currentPlayer, &G, tribute, newHandPos);

		/* set deck for players according to size */

		if (DEBUG_TRIBUTE_TEST_1) {
			printf("FILL DECK NEXT PLAYER <-----------------------------------------\n");
		}

		int newFillSize = 5;
		fillDeck(nextPlayer, &G, newFillSize);

		// NOTE* on SAVE COUNTS: idx 0 of ___Box[ ] is player 0, et cet.

		/* SAVE HAND COUNTS  */
		int handBox[MAX_PLAYERS];
		savePreviousHandCounts(handBox, &G);

		/* SAVE DECK COUNTS  */
		int deckBox[MAX_PLAYERS];
		savePreviousDeckCounts(deckBox, &G);

		/* SAVE DISCARD COUNTS  */
		int discardBox[MAX_PLAYERS];
		savePreviousDiscardCounts(discardBox, &G);

		/* SAVE TOP 2 DECK */
		if (DEBUG_TRIBUTE_TEST_1) {
			printf("BEFORE: saveTop2Deck() NEXT PLAYER <------------------------\n");
		}

		int topTwoDeck[2] = { 0 };
		saveTop2Deck(nextPlayer, &G, topTwoDeck);

		int coin_tally = 0;
		int draw_card_tally = 0;
		int num_actions_tally = 0;

		/* TALLY TYPES FOUND BEFORE CALL TO KNOW WHAT TO TEST FOR COMPARISON */
		setTypesFoundTribute(topTwoDeck, &coin_tally, &draw_card_tally, &num_actions_tally);

		/* BACK UP STATE BEFORE CALL */
		memset(&backup, '\0', sizeof(backup));
		backup = G;

		if (DEBUG_TRIBUTE_TEST_1) {
			printf("TRACE: NEXT PLAYER Deck before call to tribute..\n");
			printDeck(nextPlayer, &G);
			printf("TRACE: NEXT PLAYER Discard before call to tribute..\n");
			printDiscard(nextPlayer, &G);
		}

		/** ==> CALL <================================================= */
		int tributeRevealedCards[2] = { -1, -1 }; // pulled from dominion.c req'd. for Reuben's version
		int alpha = 0; // same as i from playCard()
		refactoredTribute(alpha, tributeRevealedCards, currentPlayer, nextPlayer, &G);

		if (DEBUG_TRIBUTE_TEST_1) {
			printf("AFTER: saveTop2Discard() NEXT PLAYER <------------------------\n");
		}

		/* SAVE TOP 2 DISCARD AFTER */
		int afterTopTwoDiscard[2] = { 0 };
		saveTop2Discard(nextPlayer, &G, afterTopTwoDiscard);

		if (DEBUG_TRIBUTE_TEST_1) {
			printf("TRACE: NEXT PLAYER Deck after call to tribute..\n");
			printDeck(nextPlayer, &G);
			printf("TRACE: NEXT PLAYER Discard after call to tribute..\n");
			printDiscard(nextPlayer, &G);
		}

		//********************************************************************
		if (coin_tally > 0 || draw_card_tally > 0 || num_actions_tally > 0)
		{
			// ASSERT previous deckCount of nextPlayer was >= 2
			if (deckBox[1] < 2)
			{
				printf("WARNING: Next Player didn't have 2 cards in deck before call.\n");
				numErrors++;
			}
			// ASSERT discardCount of nextPlayer is now >= 2
			if (discardBox[1] < 2)
			{
				printf("Next Player doesnt have 2 cards in discard after call.\n");
				numErrors++;
			}
		}

		//********************************************************************
		// CALL APPROPRIATE COMPARISONS
		if (coin_tally > 0)
		{
			// checks if coins += 2 previous
			result = compareCoins(currentPlayer, &backup, &G, PLUS_2_COINS);
			if (result == -1) { numErrors++; }
		}
		if (draw_card_tally > 0)
		{
			if (handBox[currentPlayer] == G.handCount[currentPlayer])
			{
				printf("Next Player had a victory card within top 2 of deck,\n");
				printf("\tbut Current Player handCount didn't change !\n");
				numErrors++;
			}

			// checks if every card in hand is different
			result = compareHand(currentPlayer, &backup, &G, DIFFERENT_HAND);
			if (result == -1) { numErrors++; }

		}
		if (num_actions_tally > 0)
		{
			// checks if numActions += 2 of previous
			result = compareNumActionsTribute(currentPlayer, &backup, &G);
			if (result == -1) { numErrors++; }

		}
		//********************************************************************

		if (DEBUG_TRIBUTE_TEST_1) {
			printf("CURRENT PLAYER TOP 2 DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
			printf("topTwoDeck[0]: %d\n", topTwoDeck[0]);
			printf("topTwoDeck[1]: %d\n", topTwoDeck[1]);

			/* PRINT TOP TWO DISCARD AFTER */
			printf("AFTER: afterTopTwoDiscard [ ] MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
			printf("afterTopTwoDiscard[0]: %d\n", afterTopTwoDiscard[0]);
			printf("afterTopTwoDiscard[1]: %d\n", afterTopTwoDiscard[1]);

			printf("CALLING: compareTopsAfter <==========================================\n");
		}

		// assert if nextPlayer's previous top 2 deck cards are the top 2 in their discard
		result = compareTopsAfter(nextPlayer, topTwoDeck, afterTopTwoDiscard);
		if (result == -1) { numErrors++; }

	return numErrors;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
int unitTributeTest2()
{
	int numErrors = 0;
	int result = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState backup;

	/* TEST 2 */
	if (RULES)
	{
		printf("TRIBUTE TEST 2: Rules:\n");
		printf("			  : Wipe every player's hand, deck, and discard piles.\n");
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
		printf("              : 4 Players.\n");
		printf("              : Current Player = 3 (means player 4).\n");
		printf("	          : Randomly assign 3 cards from kingdomCards to..\n");
		printf("              : each player's hand.\n");
		printf("              : each player's deck.\n");
		printf("              : Place a tribute card at a random position in hand.\n");
		printf("OUTPUT:\n\n");
	}

	// number of players
	int newNumPlayers = 4;

	/* # players, cards to use, seed, gameState */
	initTestGame(newNumPlayers, kingdomCards, 1, &G);

	int currentPlayer = 3;
	int nextPlayer = currentPlayer + 1;

	// size of hand to use
	int newHandSize = 3;

	/* set up all players with 3 random cards in each hand */
	prepPlayers(
		currentPlayer,		// prep from this player
		nextPlayer,		// prep to this player
		&G,
		newHandSize,
		FILL_DIFF,
		-1				// not used..only for filling with same card
	);

	// position to place a single kingdom card into hand
	int newHandPos = _genRandRange(0, newHandSize);

	/* set a tribute card in player zero's hand at position zero */
	setAtHandPos(currentPlayer, &G, tribute, newHandPos);

	/* set deck for players according to size */

	if (DEBUG_TRIBUTE_TEST_2) {
		printf("FILL DECK NEXT PLAYER <-----------------------------------------\n");
	}

	int newFillSize = 3;
	fillDeck(nextPlayer, &G, newFillSize);

	// NOTE* on SAVE COUNTS: idx 0 of ___Box[ ] is player 0, et cet.

	/* SAVE HAND COUNTS  */
	int handBox[MAX_PLAYERS];
	savePreviousHandCounts(handBox, &G);

	/* SAVE DECK COUNTS  */
	int deckBox[MAX_PLAYERS];
	savePreviousDeckCounts(deckBox, &G);

	/* SAVE DISCARD COUNTS  */
	int discardBox[MAX_PLAYERS];
	savePreviousDiscardCounts(discardBox, &G);

	/* SAVE TOP 2 DECK */
	if (DEBUG_TRIBUTE_TEST_2) {
		printf("BEFORE: saveTop2Deck() NEXT PLAYER <------------------------\n");
	}

	int topTwoDeck[2] = { 0 };
	saveTop2Deck(nextPlayer, &G, topTwoDeck);

	int coin_tally = 0;
	int draw_card_tally = 0;
	int num_actions_tally = 0;

	/* TALLY TYPES FOUND BEFORE CALL TO KNOW WHAT TO TEST FOR COMPARISON */
	setTypesFoundTribute(topTwoDeck, &coin_tally, &draw_card_tally, &num_actions_tally);

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	if (DEBUG_TRIBUTE_TEST_2) {
		printf("TRACE: NEXT PLAYER Deck before call to tribute..\n");
		printDeck(nextPlayer, &G);
		printf("TRACE: NEXT PLAYER Discard before call to tribute..\n");
		printDiscard(nextPlayer, &G);
	}

	/** ==> CALL <================================================= */
	int tributeRevealedCards[2] = { -1, -1 }; // pulled from dominion.c req'd. for Reuben's version
	int alpha = 0; // same as i from playCard()
	refactoredTribute(alpha, tributeRevealedCards, currentPlayer, nextPlayer, &G);

	if (DEBUG_TRIBUTE_TEST_2) {
		printf("AFTER: saveTop2Discard() NEXT PLAYER <------------------------\n");
	}

	/* SAVE TOP 2 DISCARD AFTER */
	int afterTopTwoDiscard[2] = { 0 };
	saveTop2Discard(nextPlayer, &G, afterTopTwoDiscard);

	if (DEBUG_TRIBUTE_TEST_2) {
		printf("TRACE: NEXT PLAYER Deck after call to tribute..\n");
		printDeck(nextPlayer, &G);
		printf("TRACE: NEXT PLAYER Discard after call to tribute..\n");
		printDiscard(nextPlayer, &G);
	}

	//********************************************************************
	if (coin_tally > 0 || draw_card_tally > 0 || num_actions_tally > 0)
	{
		// ASSERT previous deckCount of nextPlayer was >= 2
		if (deckBox[1] < 2)
		{
			printf("WARNING: Next Player didn't have 2 cards in deck before call.\n");
			numErrors++;
		}
		// ASSERT discardCount of nextPlayer is now >= 2
		if (discardBox[1] < 2)
		{
			printf("Next Player doesnt have 2 cards in discard after call.\n");
			numErrors++;
		}
	}

	//********************************************************************
	// CALL APPROPRIATE COMPARISONS
	if (coin_tally > 0)
	{
		// checks if coins += 2 previous
		result = compareCoins(currentPlayer, &backup, &G, PLUS_2_COINS);
		if (result == -1) { numErrors++; }
	}
	if (draw_card_tally > 0)
	{
		if (handBox[currentPlayer] == G.handCount[currentPlayer])
		{
			printf("Next Player had a victory card within top 2 of deck,\n");
			printf("\tbut Current Player handCount didn't change !\n");
			numErrors++;
		}

		// checks if every card in hand is different
		result = compareHand(currentPlayer, &backup, &G, DIFFERENT_HAND);
		if (result == -1) { numErrors++; }

	}
	if (num_actions_tally > 0)
	{
		// checks if numActions += 2 of previous
		result = compareNumActionsTribute(currentPlayer, &backup, &G);
		if (result == -1) { numErrors++; }

	}
	//********************************************************************

	if (DEBUG_TRIBUTE_TEST_2) {
		printf("CURRENT PLAYER TOP 2 DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
		printf("topTwoDeck[0]: %d\n", topTwoDeck[0]);
		printf("topTwoDeck[1]: %d\n", topTwoDeck[1]);

		/* PRINT TOP TWO DISCARD AFTER */
		printf("AFTER: afterTopTwoDiscard [ ] MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
		printf("afterTopTwoDiscard[0]: %d\n", afterTopTwoDiscard[0]);
		printf("afterTopTwoDiscard[1]: %d\n", afterTopTwoDiscard[1]);

		printf("CALLING: compareTopsAfter <==========================================\n");
	}

	// assert if nextPlayer's previous top 2 deck cards are the top 2 in their discard
	result = compareTopsAfter(nextPlayer, topTwoDeck, afterTopTwoDiscard);
	if (result == -1) { numErrors++; }

	return numErrors;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
int unitTributeTest3()
{
	int numErrors = 0;
	int result = 0;
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState backup;

	/* TEST 3 */
	if (RULES)
	{
		printf("TRIBUTE TEST 3: Rules:\n");
		printf("			  : Wipe every player's hand, deck, and discard piles.\n");
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
		printf("              : 3 Players.\n");
		printf("              : Current Player = 1 (means player 2).\n");
		printf("	          : Randomly assign 1 card from kingdomCards to..\n");
		printf("              : each player's hand.\n");
		printf("              : Place a tribute card at a random position.\n");
		printf("              : Assign no cards to each player's deck.\n");
		printf("OUTPUT:\n\n");
	}

	// number of players
	int newNumPlayers = 3;

	/* # players, cards to use, seed, gameState */
	initTestGame(newNumPlayers, kingdomCards, 1, &G);

	int currentPlayer = 1;
	int nextPlayer = currentPlayer + 1;

	// size of hand to use
	int newHandSize = 1;

	/* set up all players with 5 random cards in each hand */
	prepPlayers(
		currentPlayer,		// prep from this player
		nextPlayer,		// prep to this player
		&G,
		newHandSize,
		FILL_DIFF,
		-1				// not used..only for filling with same card
	);

	// position to place a single kingdom card into hand
	int newHandPos = _genRandRange(0, newHandSize);

	/* set a tribute card in player zero's hand at position zero */
	setAtHandPos(currentPlayer, &G, tribute, newHandPos);

	/* set deck for players according to size */

	if (DEBUG_TRIBUTE_TEST_1) {
		printf("FILL DECK NEXT PLAYER <-----------------------------------------\n");
	}

	int newFillSize = 0;
	fillDeck(nextPlayer, &G, newFillSize);

	// NOTE* on SAVE COUNTS: idx 0 of ___Box[ ] is player 0, et cet.

	/* SAVE HAND COUNTS  */
	int handBox[MAX_PLAYERS];
	savePreviousHandCounts(handBox, &G);

	/* SAVE DECK COUNTS  */
	int deckBox[MAX_PLAYERS];
	savePreviousDeckCounts(deckBox, &G);

	/* SAVE DISCARD COUNTS  */
	int discardBox[MAX_PLAYERS];
	savePreviousDiscardCounts(discardBox, &G);

	/* SAVE TOP 2 DECK */
	if (DEBUG_TRIBUTE_TEST_1) {
		printf("BEFORE: saveTop2Deck() NEXT PLAYER <------------------------\n");
	}

	int topTwoDeck[2] = { 0 };
	saveTop2Deck(nextPlayer, &G, topTwoDeck);

	int coin_tally = 0;
	int draw_card_tally = 0;
	int num_actions_tally = 0;

	/* TALLY TYPES FOUND BEFORE CALL TO KNOW WHAT TO TEST FOR COMPARISON */
	setTypesFoundTribute(topTwoDeck, &coin_tally, &draw_card_tally, &num_actions_tally);

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	if (DEBUG_TRIBUTE_TEST_3) {
		printf("TRACE: NEXT PLAYER Deck before call to tribute..\n");
		printDeck(nextPlayer, &G);
		printf("TRACE: NEXT PLAYER Discard before call to tribute..\n");
		printDiscard(nextPlayer, &G);
	}

	/** ==> CALL <================================================= */
	int tributeRevealedCards[2] = { -1, -1 }; // pulled from dominion.c req'd. for Reuben's version
	int alpha = 0; // same as i from playCard()
	refactoredTribute(alpha, tributeRevealedCards, currentPlayer, nextPlayer, &G);


	if (DEBUG_TRIBUTE_TEST_3) {
		printf("AFTER: saveTop2Discard() NEXT PLAYER <------------------------\n");
	}

	/* SAVE TOP 2 DISCARD AFTER */
	int afterTopTwoDiscard[2] = { 0 };
	saveTop2Discard(nextPlayer, &G, afterTopTwoDiscard);

	if (DEBUG_TRIBUTE_TEST_3) {
		printf("TRACE: NEXT PLAYER Deck after call to tribute..\n");
		printDeck(nextPlayer, &G);
		printf("TRACE: NEXT PLAYER Discard after call to tribute..\n");
		printDiscard(nextPlayer, &G);
	}

	//********************************************************************
	if (coin_tally > 0 || draw_card_tally > 0 || num_actions_tally > 0)
	{
		// ASSERT previous deckCount of nextPlayer was >= 2
		if (deckBox[1] < 2)
		{
			printf("WARNING: Next Player didn't have 2 cards in deck before call.\n");
			numErrors++;
		}
		// ASSERT discardCount of nextPlayer is now >= 2
		if (discardBox[1] < 2)
		{
			printf("Next Player doesnt have 2 cards in discard after call.\n");
			numErrors++;
		}
	}

	//********************************************************************
	// CALL APPROPRIATE COMPARISONS
	if (coin_tally > 0)
	{
		// checks if coins += 2 previous
		result = compareCoins(currentPlayer, &backup, &G, PLUS_2_COINS);
		if (result == -1) { numErrors++; }
	}
	if (draw_card_tally > 0)
	{
		if (handBox[currentPlayer] == G.handCount[currentPlayer])
		{
			printf("Next Player had a victory card within top 2 of deck,\n");
			printf("\tbut Current Player handCount didn't change !\n");
			numErrors++;
		}

		// checks if every card in hand is different
		result = compareHand(currentPlayer, &backup, &G, DIFFERENT_HAND);
		if (result == -1) { numErrors++; }

	}
	if (num_actions_tally > 0)
	{
		// checks if numActions += 2 of previous
		result = compareNumActionsTribute(currentPlayer, &backup, &G);
		if (result == -1) { numErrors++; }

	}
	//********************************************************************

	if (DEBUG_TRIBUTE_TEST_2) {
		printf("CURRENT PLAYER TOP 2 DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
		printf("topTwoDeck[0]: %d\n", topTwoDeck[0]);
		printf("topTwoDeck[1]: %d\n", topTwoDeck[1]);

		/* PRINT TOP TWO DISCARD AFTER */
		printf("AFTER: afterTopTwoDiscard [ ] MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
		printf("afterTopTwoDiscard[0]: %d\n", afterTopTwoDiscard[0]);
		printf("afterTopTwoDiscard[1]: %d\n", afterTopTwoDiscard[1]);

		printf("CALLING: compareTopsAfter <==========================================\n");
	}

	// assert if nextPlayer's previous top 2 deck cards are the top 2 in their discard
	result = compareTopsAfter(nextPlayer, topTwoDeck, afterTopTwoDiscard);
	if (result == -1) { numErrors++; }

	return numErrors;
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
int unitTributeTest4()
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
		printf("unittest4.c\n");
		printf("TRIBUTE TEST 4: Rules:\n");
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
		printf("              : 4 Players.\n");
		printf("              : Current Player = 3 (means player 4).\n");
		printf("			  : Randomly assign 7 cards from kingdomCards to..\n");
		printf("              : each player's hand.\n");
		printf("              : to each player's deck.\n");
		printf("              : DO NOT place a tribute card at any position in hand.\n");
			printf("OUTPUT:\n\n");
	}

	// number of players
	int newNumPlayers = 4;

	/* # players, cards to use, seed, gameState */
	initTestGame(newNumPlayers, kingdomCards, 1, &G);

	int currentPlayer = 3;
	int nextPlayer = currentPlayer + 1;

	// size of hand to use
	int newHandSize = 7;

	/* set up all players with 5 random cards in each hand */
	prepPlayers(
		currentPlayer,		// prep from this player
		nextPlayer,		// prep to this player
		&G,
		newHandSize,
		FILL_DIFF,
		-1				// not used..only for filling with same card
	);

	/* set deck for players according to size */

	if (DEBUG_TRIBUTE_TEST_4) {
		printf("FILL DECK NEXT PLAYER <-----------------------------------------\n");
	}

	int newFillSize = 7;
	fillDeck(nextPlayer, &G, newFillSize);

	// NOTE* on SAVE COUNTS: idx 0 of ___Box[ ] is player 0, et cet.

	/* SAVE HAND COUNTS  */
	int handBox[MAX_PLAYERS];
	savePreviousHandCounts(handBox, &G);

	/* SAVE DECK COUNTS  */
	int deckBox[MAX_PLAYERS];
	savePreviousDeckCounts(deckBox, &G);

	/* SAVE DISCARD COUNTS  */
	int discardBox[MAX_PLAYERS];
	savePreviousDiscardCounts(discardBox, &G);

	/* SAVE TOP 2 DECK */
	if (DEBUG_TRIBUTE_TEST_4) {
		printf("BEFORE: saveTop2Deck() NEXT PLAYER <------------------------\n");
	}

	int topTwoDeck[2] = { 0 };
	saveTop2Deck(nextPlayer, &G, topTwoDeck);

	int coin_tally = 0;
	int draw_card_tally = 0;
	int num_actions_tally = 0;

	/* TALLY TYPES FOUND BEFORE CALL TO KNOW WHAT TO TEST FOR COMPARISON */
	setTypesFoundTribute(topTwoDeck, &coin_tally, &draw_card_tally, &num_actions_tally);

	/* BACK UP STATE BEFORE CALL */
	memset(&backup, '\0', sizeof(backup));
	backup = G;

	if (DEBUG_TRIBUTE_TEST_4) {
		printf("TRACE: NEXT PLAYER Deck before call to tribute..\n");
		printDeck(nextPlayer, &G);
		printf("TRACE: NEXT PLAYER Discard before call to tribute..\n");
		printDiscard(nextPlayer, &G);
	}

	/** ==> CALL <================================================= */
	int tributeRevealedCards[2] = { -1, -1 }; // pulled from dominion.c req'd. for Reuben's version
	int alpha = 0; // same as i from playCard()
	refactoredTribute(alpha, tributeRevealedCards, currentPlayer, nextPlayer, &G);

	if (DEBUG_TRIBUTE_TEST_4) {
		printf("AFTER: saveTop2Discard() NEXT PLAYER <------------------------\n");
	}

	/* SAVE TOP 2 DISCARD AFTER */
	int afterTopTwoDiscard[2] = { 0 };
	saveTop2Discard(nextPlayer, &G, afterTopTwoDiscard);

	if (DEBUG_TRIBUTE_TEST_4) {
		printf("TRACE: NEXT PLAYER Deck after call to tribute..\n");
		printDeck(nextPlayer, &G);
		printf("TRACE: NEXT PLAYER Discard after call to tribute..\n");
		printDiscard(nextPlayer, &G);
	}

	//********************************************************************
	if (coin_tally > 0 || draw_card_tally > 0 || num_actions_tally > 0)
	{
		// ASSERT previous deckCount of nextPlayer was >= 2
		if (deckBox[1] < 2)
		{
			printf("WARNING: Next Player didn't have 2 cards in deck before call.\n");
			numErrors++;
		}
		// ASSERT discardCount of nextPlayer is now >= 2
		if (discardBox[1] < 2)
		{
			printf("Next Player doesnt have 2 cards in discard after call.\n");
			numErrors++;
		}
	}

	//********************************************************************
	// CALL APPROPRIATE COMPARISONS
	if (coin_tally > 0)
	{
		// checks if coins += 2 previous
		result = compareCoins(currentPlayer, &backup, &G, PLUS_2_COINS);
		if (result == -1) { numErrors++; }
	}
	if (draw_card_tally > 0)
	{
		if (handBox[currentPlayer] == G.handCount[currentPlayer])
		{
			printf("Next Player had a victory card within top 2 of deck,\n");
			printf("\tbut Current Player handCount didn't change !\n");
			numErrors++;
		}

		// checks if every card in hand is different
		result = compareHand(currentPlayer, &backup, &G, DIFFERENT_HAND);
		if (result == -1) { numErrors++; }

	}
	if (num_actions_tally > 0)
	{
		// checks if numActions += 2 of previous
		result = compareNumActionsTribute(currentPlayer, &backup, &G);
		if (result == -1) { numErrors++; }

	}
	//********************************************************************

	if (DEBUG_TRIBUTE_TEST_4) {
		printf("CURRENT PLAYER TOP 2 DECK MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
		printf("topTwoDeck[0]: %d\n", topTwoDeck[0]);
		printf("topTwoDeck[1]: %d\n", topTwoDeck[1]);

		/* PRINT TOP TWO DISCARD AFTER */
		printf("AFTER: afterTopTwoDiscard [ ] MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
		printf("afterTopTwoDiscard[0]: %d\n", afterTopTwoDiscard[0]);
		printf("afterTopTwoDiscard[1]: %d\n", afterTopTwoDiscard[1]);

		printf("CALLING: compareTopsAfter <==========================================\n");
	}

	// assert if nextPlayer's previous top 2 deck cards are the top 2 in their discard
	result = compareTopsAfter(nextPlayer, topTwoDeck, afterTopTwoDiscard);
	if (result == -1) { numErrors++; }

	return numErrors;
}


/* EOF */

