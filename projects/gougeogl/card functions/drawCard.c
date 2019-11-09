/*************************************************
* FileName: drawCard.c
* Description: testing alternate version of drawCard() function
***************************************************************/

int drawCard(int player, struct gameState *state)
{
	int count;
	int deckCounter;
	if (state->deckCount[player] <= 0) { //Deck is empty

		//Step 1 Shuffle the discard pile back into a deck
		int i;
		//Move discard to deck
		for (i = 0; i < state->discardCount[player]; i++) {
			state->deck[player][i] = state->discard[player][i];
			state->discard[player][i] = -1;
		}

		state->deckCount[player] = state->discardCount[player];
		state->discardCount[player] = 0;//Reset discard

		//Shuffle the deck
		shuffle(player, state);//Shuffle the deck up and make it so that we can draw

		if (DEBUG) { //Debug statements
			printf("GLEN: TRACE: uno.\n");
			printf("Deck count now: %d\n", state->deckCount[player]);
		}

		//state->discardCount[player] = 0; <--- BUG ! if you use this..you'll only have coppers !!

		//Step 2 Draw Card
		count = state->handCount[player];//Get current player's hand count

		if (DEBUG) { //Debug statements
			printf("GLEN: TRACE: dos.\n");
			printf("Current hand count: %d\n", count);
		}

		deckCounter = state->deckCount[player];//Create a holder for the deck count

		if (deckCounter == 0)
			return -1;

		if (DEBUG) {
			printf("IN drawCard() toward end:\n");
			printf("BEFORE: state->hand[player][count] = %d\n", state->hand[player][count]);
			printf("BEFORE: state->deckCount[player] = %d\n", state->deckCount[player]);
			printf("BEFORE: state->handCount[player] = %d\n", state->handCount[player]);
			printf("BEFORE: deckCounter = %d, deckCounter - 1 = %d\n", deckCounter, deckCounter - 1);
			printf("BEFORE: state->deck[player][deckCounter - 1] = %d\n", state->deck[player][deckCounter - 1]);
		}
		state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to hand
		state->deckCount[player]--;
		state->handCount[player]++;//Increment hand count

		if (DEBUG) {
			printf("IN drawCard() toward end:\n");
			printf("AFTER: state->hand[player][count] = %d\n", state->hand[player][count]);
			printf("AFTER: state->deckCount[player] = %d\n", state->deckCount[player]);
			printf("AFTER: state->handCount[player] = %d\n", state->handCount[player]);
			printf("AFTER: deckCounter = %d, deckCounter - 1 = %d\n", deckCounter, deckCounter - 1);
			printf("AFTER: state->deck[player][deckCounter - 1] = %d\n", state->deck[player][deckCounter - 1]);
		}
	}
	else {
		count = state->handCount[player];//Get current hand count for player
		if (DEBUG) { //Debug statements
			printf("GLEN: TRACE: tres.\n");
			printf("Current hand count: %d\n", count);
		}
		
		if (DEBUG) {
			printf("IN drawCard() toward end:\n");
			printf("BEFORE: state->hand[player][count] = %d\n", state->hand[player][count]);
			printf("BEFORE: state->deckCount[player] = %d\n", state->deckCount[player]);
			printf("BEFORE: state->handCount[player] = %d\n", state->handCount[player]);
			printf("BEFORE: deckCounter = %d, deckCounter - 1 = %d\n", deckCounter, deckCounter - 1);
			printf("BEFORE: state->deck[player][deckCounter - 1] = %d\n", state->deck[player][deckCounter - 1]);
		}

		deckCounter = state->deckCount[player];//Create holder for the deck count
		state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to the hand
		state->deckCount[player]--;
		state->handCount[player]++;//Increment hand count

		if (DEBUG) {
			printf("IN drawCard() toward end:\n");
			printf("AFTER: state->hand[player][count] = %d\n", state->hand[player][count]);
			printf("AFTER: state->deckCount[player] = %d\n", state->deckCount[player]);
			printf("AFTER: state->handCount[player] = %d\n", state->handCount[player]);
			printf("AFTER: deckCounter = %d, deckCounter - 1 = %d\n", deckCounter, deckCounter - 1);
			printf("AFTER: state->deck[player][deckCounter - 1] = %d\n", state->deck[player][deckCounter - 1]);
		}
	}
}