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

testSupplyCount(int card, struct gameState* state);

int main()
{
	int kingdomCards[10] = { adventurer, ambassador, baron, estate, tribute, minion, mine, gardens, remodel, smithy };
	
	// Set up Game State 
	struct gameState G;
	memset(&G, '\0', sizeof(struct gameState));   // clear mem of 

	addToSupply(adventurer, &G, 1);

	int result = supplyCount(adventurer, &G);
	printf("TEST: supplyCount\n");
	printf("    : after adding 1 adventurer card..\n");
	printf("    : G.supplyCount[%d]: %d\n", adventurer, G.supplyCount[adventurer]);
	fflush(stdout);

	
	return 0;
}

/* adds a specific quantity of a particular card into the supply */
void addToSupply(int card, struct gameState* state, int qty)
{
	if (card > curse && card <= treasure_map)
	{
		state->supplyCount[card] += gty;
	}
}
