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
//int testBaron(int choice, struct gameState* state, int shouldDump, int handCount, int handPos);
int zero_or_one();
int getTopDiscard( struct gameState* state );

void prep(
	int player, 
	int seed, 
	int* kingdom, 
	struct gameState* state, 
	int numEstates, 
	int handSize, 
	int handPos, 
	int card
);

void initTestGame(
	int players, 
	int* kDeck, 
	int mySeed, 
	struct gameState* game 
);

/* ESTATE CARD RELATED */
void removeEstatesDeck(struct gameState *gameS);
void wipeDeck(struct gameState* state);
void setEstateSupply(struct gameState *state, int qtyEstates);
void removeEstatesDiscard(struct gameState *st);

/* HAND RELATED */
void resetHand(struct gameState* dState);
void setHandCount(struct gameState* state, int newHandSize);
void setHandPos(struct gameState* state, int card, int handPos);

/* GENERIC COMPARE FUNCTIONS */
int increased_as_wanted(int before, int after, char* message, int amt );
int decreased_as_wanted(int before, int after, char* message, int amt );

/* CUSTOM COMPARE FUNCTIONS */ 
int compare_hand(struct gameState* before, struct gameState* after);
int compare_deck(struct gameState* before, struct gameState* after);
int compare_discard(struct gameState* before, struct gameState* after);
int compare_playedCards(struct gameState* before, struct gameState* after);
int compare_trashPile(struct gameState* before, struct gameState* after);

int main( int argc, char* argv[] )
{
	int seed = 1; 
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine,  gardens, remodel, smithy };

	// Game States 
	struct gameState G;
	struct gameState gPtr;

	// helper vars
	int currentPlayer = 0; 
	int check = -1;
	int choice = -1;


	// error strings
	char* stolenCoins = "Error. stolen coins.";
	char* missingCoins = "Error. coins missing.";
	char* illegalEstate = "Error. illegal estate gain.";
	char* missingEstate = "Error. estate missing.";
	char* noEstatesSupply = "Error. no estates in supply.";
	char* noEstatesHand = "Error. no estates in hand.";
	char* noEstatesDiscard = "Error. no estates in discard";

	// message strings
	char* estateString = "Baron. Choice was: discard estate."; 
	char* coinsString = "Baron. Choice was: gain +4 coins.";

	/* neg: invalid players */   
	prep(-1, seed, kingdomCards, &G, 0, 0, 0, -1); 
	baronCard(1, &G);

	/* neg: invalid players */   
	prep( 0, seed, kingdomCards, &G, 0, 0, 0, -1); 
	baronCard(1, &G);

	/* neg: invalid players */   
	prep( 5, seed, kingdomCards, &G, 0, 0, 0, -1); 
	baronCard(1, &G);

	/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
	/* neg: invalid estates: zero supply & no estates in hand */ 
	prep( 2, seed, kingdomCards, &G, 0, 1, 0, -1); 
	gPtr = G;
	memset(&gPtr, '\0', sizeof(gPtr));
	choice = zero_or_one();	
	baronCard(choice, &G);

	if(choice == 1)
	{
		printf("1.%s\n", coinsString);
		check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
	}
	else
	{
		printf("2.%s\n", estateString);
		check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
	}
	if( getTopDiscard(&gPtr) != estate ){
		if( getTopDiscard(&G) == estate ){
			if(choice == 1){
				printf("Baron. Gained an estate to top of discard, but none was in hand or supply!\n"); 
			}
			else if(choice == 0){
				printf("Baron. Chose +4 coins. Gained an estate to top of discard, but none was in hand or supply!\n"); 
			}
			fflush(stdout);
		}
	} 	
	/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/
	/* check no estates supply, but 1 estate at position 'i'  */
	int i;
	for(i = 0; i < 5; i++){
		prep( 2, seed, kingdomCards, &G, 0, 5, i, estate); 
		memset(&gPtr, '\0', sizeof(gPtr));
		gPtr = G;
		choice = zero_or_one();	
		baronCard(choice, &G);
		
		if(choice == 1)
		{
			printf("%s\n", coinsString);
			check = increased_as_wanted(gPtr.coins, G.coins, missingCoins, 4);
		}
		else
		{
			printf("%s\n", estateString);
			check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
		}

		if( getTopDiscard(&gPtr) != estate ){
			if( getTopDiscard(&G) == estate ){
				printf("%s %s\n", noEstatesSupply, illegalEstate);
				fflush(stdout);
			}
		} 	

	}
	/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

	/* check no estates in hand, but one in supply */
	
	int k;
	for(k= 0; k < 5; k++)
	{
		prep( 2, seed, kingdomCards, &G, 1, 5, k, -1);  
		memset(&gPtr, '\0', sizeof(gPtr));
		gPtr = G;
		choice = zero_or_one();	
		baronCard(choice, &G);

		if(choice == 1)
		{
			printf("%s\n", coinsString);
			check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);

			if( getTopDiscard(&G) == estate ){
				printf("%s\n", illegalEstate); 
				fflush(stdout);
			}

		}
		else
		{
			printf("%s\n", estateString);
			check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
	
			if( getTopDiscard(&G) != estate ){
				printf("%s\n", missingEstate); 
				fflush(stdout);
			}
		}
	}

	/* test a hand full of estates, none in supply <==== */ 
	int n;
	for(n = 0; n < 5; n++){
	prep( 2, seed, kingdomCards, &G, 0, 5, n, estate); 
		memset(&gPtr, '\0', sizeof(gPtr));
		gPtr = G;
		choice = zero_or_one();	
		baronCard(choice, &G);

		if(choice == 1)
		{
			printf("%s\n", coinsString);
			check = increased_as_wanted(gPtr.coins, G.coins, missingCoins, 4);
		}
		else
		{
			printf("%s\n", estateString);
			check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
		}	
		if( getTopDiscard(&G) == estate ){
			printf("%s\n", illegalEstate); 
			fflush(stdout);
		}

	}

	/* CHOOSE 1 test a hand full of copper, no estates supply <==== */ 
	int Q;
	for(Q = 0; Q < 5; Q++){
	prep( 2, seed, kingdomCards, &G, 0, 5, Q, copper); 
		memset(&gPtr, '\0', sizeof(gPtr));
		gPtr = G;
		choice = zero_or_one();	
		baronCard(choice, &G);

		if(choice == 1)
		{
			printf("%s\n", coinsString);
			check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
		}
		else
		{
			printf("%s\n", estateString);
			check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
		}
		if( getTopDiscard(&gPtr) != estate ){
			if( getTopDiscard(&G) == estate ){

				printf("%s\n", illegalEstate);
				fflush(stdout);
			}
		} 	
	}

	/* test hand of all different, and no estates in supply  */
	/* CHOOSE 1 test hand of all different, and no estates in supply  <==== */ 
	prep( 2, seed, kingdomCards, &G, 0, 5, 0, copper); 
	G.hand[G.whoseTurn][1] = smithy;
	G.hand[G.whoseTurn][2] = adventurer;
	G.hand[G.whoseTurn][3] = mine;
	G.hand[G.whoseTurn][3] = remodel;

	memset(&gPtr, '\0', sizeof(gPtr));
	gPtr = G;
	choice = zero_or_one();	
	baronCard(choice, &G);

	if(choice == 1)
	{
		printf("%s\n", coinsString);
		check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
	}
	else
	{
		printf("%s\n", estateString);
		check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
	}
	if( getTopDiscard(&gPtr) != estate ){
		if( getTopDiscard(&G) == estate ){

			printf("%s\n", illegalEstate);
			fflush(stdout);
		}
	} 	

	/* CHOOSE 1 test hand of all different (and 1 estate), no estates in supply <==== */
	int s;
	for(s = 0; s < 5; s++){
		prep( 2, seed, kingdomCards, &G, 0, 5, s, -1); 
		G.hand[G.whoseTurn][0] = gold;
		G.hand[G.whoseTurn][1] = smithy;
		G.hand[G.whoseTurn][2] = adventurer;
		G.hand[G.whoseTurn][3] = mine;
		G.hand[G.whoseTurn][4] = remodel;
		G.hand[G.whoseTurn][s] = estate;

		memset(&gPtr, '\0', sizeof(gPtr));
		gPtr = G;
		choice = zero_or_one();	
		baronCard(choice, &G);

		if(choice == 1)
		{
			printf("%s\n", coinsString);
			check = increased_as_wanted(gPtr.coins, G.coins, missingCoins, 4);
		}
		else
		{
			printf("%s\n", estateString);
			check = increased_as_wanted(gPtr.coins, G.coins, stolenCoins, 0);
		}	
		if( getTopDiscard(&G) == estate ){
			printf("%s\n", illegalEstate); 
			fflush(stdout);
		}
	}
	return 0;
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
	
	int checkInit = initializeGame(players, kDeck, mySeed, game); // initialize 2 player game 
	
	if(checkInit == -1)
	{ printf("Bad game initialization.\n"); }

}

// set player to remove all estates from current player's deck  
void removeEstatesDeck(struct gameState* gameS)  
{  
	int currentPlayer = whoseTurn(gameS);
	int tally = 0;

	int i = 0;   
	while(i < gameS->deckCount[currentPlayer])   
	{    
		if(gameS->deck[currentPlayer][i] == estate)    
		{     
			gameS->deck[currentPlayer][i] = -1;     
			tally++;
		}   
		i++;
	}  
	gameS->deckCount[currentPlayer] -= tally;

} 

// set player to remove all estates from current player's deck  
void wipeDeck(struct gameState* state)  
{  
	int currentPlayer = whoseTurn(state);

	int i = 0;   
	while(i < state->deckCount[currentPlayer])   
	{    
		state->deck[currentPlayer][i] = -1;     
		i++;
	}   
	state->deckCount[currentPlayer] = 0;
} 

// eliminate all estates from discard.   
void removeEstatesDiscard(struct gameState *st)  
{   
	int currentPlayer = whoseTurn(st);   
	int tally = 0;	

	int i = 0;   
	while(i < st->discardCount[currentPlayer])   
	{    
		// search for an estate in discard of current player    

		if(st->discard[currentPlayer][i] == estate)    
		{     
			// if found, wipe out, set to -1     
			st->discard[currentPlayer][i] = -1;    
			tally++;
		}   
		i++;
	}  
	st->discardCount[currentPlayer] -= tally; 
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
	}
	dState->handCount[currentPlayer] = 0;
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

/* getter for top of currentPlayer's discard */
int getTopDiscard( struct gameState* state )
{
	int result = -1;
	int currentPlayer = state->whoseTurn;
	int top = state->discardCount[currentPlayer];

	if(	(state->discard[currentPlayer][top] >= curse ) ||
		(state->discard[currentPlayer][top] <= treasure_map )	){

		result = state->discard[currentPlayer][top];	

	}	
	return result;
}
/**********************************************************************************************/
/*
void get_stats_before_call(struct gameState* my_game, int my_arr[], int card)
{
	int currentPlayer = whoseTurn(my_game);

	my_arr[0] = my_game->supplyCount[card];  how many of card left in supply  
	my_arr[1] = my_game->playedCards[my_game->playedCardCount];  # cards played  
	my_arr[2] = my_game->playedCardCount;   how many cards played  

	my_arr[3] = my_game->numBuys;  buys  
	my_arr[4] = my_game->numActions;  actions  
	my_arr[5] = my_game->coins;  coins  


	my_arr[6] = my_game->trashPile[my_game->trashCount];  top of trash  
	my_arr[7] = my_game->trashCount;  trash size  

	my_arr[8] = my_game->discard[currentPlayer][my_game->discardCount[currentPlayer] ];  top of discard  
	my_arr[9] = my_game->discardCount[currentPlayer];  discard count  

	my_arr[10] = my_game->deck[currentPlayer][my_game->discardCount[currentPlayer] ];  top of deck  
	my_arr[11] = my_game->deckCount[currentPlayer];  deck count  

	my_arr[12] = my_game->handCount[currentPlayer];   hand count  

}
*/
void prep(int player, int seed, int* kingdom, struct gameState* state, int numEstates, int handSize, int handPos, int card)
{
	initTestGame(player, kingdom, seed, state);
	//removeEstatesDeck(state);
	wipeDeck(state);
	updateCoins(player, state, 0); /* <== added this */
	removeEstatesDiscard(state);
	setEstateSupply(state, numEstates); /* <-- add 1 estate to supply here */
	resetHand(state);
	setHandCount(state, handSize);
	setHandPos(state, card, handPos);
	state->coins = 0;
}

/* check if value increased by desired amount */ 
int increased_as_wanted(int before, int after, char* error_message, int expected_increase )
{
	int result = 1;

	if( before + expected_increase != after){
		printf("%s\n", error_message);
		fflush(stdout);	
		result = -1;
	}

	return result;
}

/* check if value increased by desired amount */ 
int decreased_as_wanted(int before, int after, char* error_message, int expected_decrease )
{
	int result = 1;

	if( before - expected_decrease != after ){
		printf("%s\n", error_message);
		fflush(stdout);	
		result = -1;
	}

	return result;
}

int check_single_int_array_no_change(int* before, int* after)
{
	int result = 0;

	int i;
	for(i = curse; i <= treasure_map; i++)
	{
		char name[MAX_STRING_LENGTH];
		memset(name, '\0', sizeof(name));

		if(before[i] != after[i]){ 
			cardNumToName(before[i], name);
			printf("changed contents at pos: %d\n", i);
			printf("                 before: %s\n", name);
			fflush(stdout);

			memset(name, '\0', sizeof(name));
			cardNumToName(after[i], name);
			printf("                 after: %s\n", name);
			fflush(stdout);

			result = -1; 
		}

	}	
	return result;
}

/* hand */
int compare_hand(struct gameState* before, struct gameState* after)
{
	int result = 0;

	int j;
	for(j = 0; j < after->numPlayers; j++){
		int k;
		for(k = 0; k < after->handCount[j]; k++){
			if(before->hand[j][k] != after->hand[j][k]){
			printf("player %d\'s card moved:\n", j);

			char name[MAX_STRING_LENGTH];
			memset(name, '\0', sizeof(name));
			cardNumToName(before->hand[j][k], name);
			printf("BEFORE: handPos: %d = %s\n", k, name); 

			memset(name, '\0', sizeof(name));
			cardNumToName(after->hand[j][k], name);
			printf("AFTER: handPos: %d = %s\n", k, name); 
			}	
		}
	}
	return result;
}


/* deck */
int compare_deck(struct gameState* before, struct gameState* after)
{
	int result = 0;

	int m;
	for(m = 0; m < after->numPlayers; m++){
		int n;
		for(n = 0; n < before->deckCount[m]; n++){
			if(before->deck[m][n] != after->deck[m][n]){
			printf("player %d\'s deck is different\n", m);
			fflush(stdout);
			result = -1;
			}
		}		
	}	
	return result;
}

/* discard */
int compare_discard(struct gameState* before, struct gameState* after)
{
	int result = 0;
	int i;
	for(i = 0; i < after->numPlayers; i++){
		int j;
		for(j = 0; j < before->discardCount[i]; j++){

			if(before->discard[i][j] != after->discard[i][j] ){
			printf("player %d\'s discard is different\n", i);
			fflush(stdout);
			result = -1;
			}
		}		
	}	
	return result;
}

/* playedCards */
int compare_playedCards(struct gameState* before, struct gameState* after)
{
	int result = 0;

	int q;
	for(q = 0; q < after->playedCardCount; q++){
		if(before->playedCards[q] != after->playedCards[q]){
		printf("played cards changed\n");
		fflush(stdout);
		result = -1;
		}
	}
	return result;
}

/* trashPile */
int compare_trashPile(struct gameState* before, struct gameState* after)
{
	int result = 0;

	int r;
	for(r = 0; r < after->trashCount; r++){
		if(before->trashPile[r] != after->trashPile[r]){
		printf("trashPile changed\n");
		fflush(stdout);
		result = -1;
		}
	}
	return result;
}

