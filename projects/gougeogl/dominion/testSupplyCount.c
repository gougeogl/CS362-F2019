/**************************************************
* FileName: testSupplyCount.c
* Author:	Glen Gougeon
* Date:		11-5-2019
* Last Mod: 11-6-2019
*
* Description:	Program to test basic assumptions
*				about how supplyCount().
*
**************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* int supplyCount(int card, struct gameState *state) {
	return state->supplyCount[card];
} */

void addToSupply(int card, struct gameState* state, int qty);
void printResult(int card, struct gameState* state);
void barrage(struct gameState* state);
int getRand();

int main()
{
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine, gardens, remodel, smithy };
	
	// Set up Game State 
	struct gameState G;
	memset(&G, '\0', sizeof(struct gameState));   // clear mem of 

	barrage(&G);
	printSupply(&G);	
	return 0;
}

/* adds a specific quantity of a particular card into the supply */
void addToSupply(int card, struct gameState* state, int qty)
{
	if (card > curse && card <= treasure_map)
	{
		state->supplyCount[card] += qty;
	}
}

void printResult(int card, struct gameState* state)
{
	char name[MAX_STRING_LENGTH]; 
	memset(name, '\0', sizeof(name));
	cardNumToName(card, name);

	printf("TEST: supplyCount\n");
	printf("    : after adding 1 %s card..\n", name);
	printf("    : card #: %d, name:%s\n", card, name);
	printf("    : state->supplyCount[%s]: %d\n", name, state->supplyCount[card]);
	fflush(stdout);
}

void barrage(struct gameState* state)
{
	int i;
	for(i = curse; i <= treasure_map; i++)
	{
		addToSupply(i, state, getRand() );
	}
}

int getRand()
{
	int num = rand() % ( 26 - 0 + 1 ) + 0;
	return num;
}
