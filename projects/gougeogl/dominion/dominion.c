/* *************************************************
* editor: Glen Gougeon 
* class: CS362 400
* last mod: 10-12-2019
* assignment: Assignment 2
* documentation: SEE separate .pdf for  explanation

****************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int compare(const void* a, const void* b) {
    if (*(int*)a > *(int*)b)
        return 1;
    if (*(int*)a < *(int*)b)
        return -1;
    return 0;
}

struct gameState* newGame() {
    struct gameState* g = malloc(sizeof(struct gameState));
    return g;
}

int* kingdomCards(int k1, int k2, int k3, int k4, int k5, int k6, int k7,
                  int k8, int k9, int k10) {
    int* k = malloc(10 * sizeof(int));
    k[0] = k1;
    k[1] = k2;
    k[2] = k3;
    k[3] = k4;
    k[4] = k5;
    k[5] = k6;
    k[6] = k7;
    k[7] = k8;
    k[8] = k9;
    k[9] = k10;
    return k;
}

int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed,
                   struct gameState *state) {
    int i;
    int j;
    /*int it;*/

    //set up random number generator
    SelectStream(1);
    PutSeed((long)randomSeed);

    //check number of players
    if (numPlayers > MAX_PLAYERS || numPlayers < 2)
    {
        return -1;
    }

    //set number of players
    state->numPlayers = numPlayers;

    //check selected kingdom cards are different
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (j != i && kingdomCards[j] == kingdomCards[i])
            {
                return -1;
            }
        }
    }


    //initialize supply
    ///////////////////////////////

    //set number of Curse cards
    if (numPlayers == 2)
    {
        state->supplyCount[curse] = 10;
    }
    else if (numPlayers == 3)
    {
        state->supplyCount[curse] = 20;
    }
    else
    {
        state->supplyCount[curse] = 30;
    }

    //set number of Victory cards
    if (numPlayers == 2)
    {
        state->supplyCount[estate] = 8;
        state->supplyCount[duchy] = 8;
        state->supplyCount[province] = 8;
    }
    else
    {
        state->supplyCount[estate] = 12;
        state->supplyCount[duchy] = 12;
        state->supplyCount[province] = 12;
    }

    //set number of Treasure cards
    state->supplyCount[copper] = 60 - (7 * numPlayers);
    state->supplyCount[silver] = 40;
    state->supplyCount[gold] = 30;

    //set number of Kingdom cards
    for (i = adventurer; i <= treasure_map; i++)       	//loop all cards
    {
        for (j = 0; j < 10; j++)           		//loop chosen cards
        {
            if (kingdomCards[j] == i)
            {
                //check if card is a 'Victory' Kingdom card
                if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens)
                {
                    if (numPlayers == 2) {
                        state->supplyCount[i] = 8;
                    }
                    else {
                        state->supplyCount[i] = 12;
                    }
                }
                else
                {
                    state->supplyCount[i] = 10;
                }
                break;
            }
            else    //card is not in the set choosen for the game
            {
                state->supplyCount[i] = -1;
            }
        }

    }

    ////////////////////////
    //supply intilization complete

    //set player decks
    for (i = 0; i < numPlayers; i++)
    {
        state->deckCount[i] = 0;
        for (j = 0; j < 3; j++)
        {
            //state->deck[i][j] = estate;
		gainCard(estate, state, 1, i);
            /*state->deckCount[i]++;*/
        }
        for (j = 3; j < 10; j++)
        {
            //state->deck[i][j] = copper;
		gainCard(copper, state, 1, i); // supplyPos, gameState, to Deck, player is 'i'
            /*state->deckCount[i]++;*/
        }
    }

    //shuffle player decks
    for (i = 0; i < numPlayers; i++)
    {
        if ( shuffle(i, state) < 0 )
        {
            return -1;
        }
    }

    //draw player hands
    for (i = 0; i < numPlayers; i++)
    {
        //initialize hand size to zero
        state->handCount[i] = 0;
        state->discardCount[i] = 0;
        //draw 5 cards
        for (j = 0; j < 5; j++)
        {
       		drawCard(i, state);
	}
    }

    //set embargo tokens to 0 for all supply piles
    for (i = 0; i <= treasure_map; i++)
    {
        state->embargoTokens[i] = 0;
    }

    //initialize first player's turn
    state->outpostPlayed = 0;
    state->phase = 0;
    state->numActions = 1;
    state->numBuys = 1;
    state->playedCardCount = 0;
    state->whoseTurn = 0;
    //state->handCount[state->whoseTurn] = 0;

    updateCoins(state->whoseTurn, state, 0);

    return 0;
}

int shuffle(int player, struct gameState *state) {

    int newDeck[MAX_DECK];
    int newDeckPos = 0;
    int card;
    int i;

    if (state->deckCount[player] < 1)
        return -1;
    qsort ((void*)(state->deck[player]), state->deckCount[player], sizeof(int), compare);
    /* SORT CARDS IN DECK TO ENSURE DETERMINISM! */

    while (state->deckCount[player] > 0) {
        card = floor(Random() * state->deckCount[player]);
        newDeck[newDeckPos] = state->deck[player][card];
        newDeckPos++;
        for (i = card; i < state->deckCount[player]-1; i++) {
            state->deck[player][i] = state->deck[player][i+1];
        }
        state->deckCount[player]--;
    }
    for (i = 0; i < newDeckPos; i++) {
        state->deck[player][i] = newDeck[i];
        state->deckCount[player]++;
    }
    return 0;
}

int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state)
{
    int card;
    int coin_bonus = 0; 		//tracks coins gain from actions

    //check if it is the right phase
    if (state->phase != 0)
    {
        return -1;
    }

    //check if player has enough actions
    if ( state->numActions < 1 )
    {
        return -1;
    }

    //get card played
    card = handCard(handPos, state);

    //check if selected card is an action
    if ( card < adventurer || card > treasure_map )
    {
        return -1;
    }

    //play card
    if ( cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus) < 0 )
    {
        return -1;
    }

    //reduce number of actions
    state->numActions--;

    //update coins (Treasure cards may be added with card draws)
    updateCoins(state->whoseTurn, state, coin_bonus);

    return 0;
}

// doesn't check phase first..who is responsible for checking that??
int buyCard(int supplyPos, struct gameState *state) {
    int who;
    if (DEBUG) {
        printf("Entering buyCard...\n");
    }
		
    who = state->whoseTurn;

    if (state->numBuys < 1) {
        if (DEBUG)
            printf("You do not have any buys left\n");
        return -1;
    } else if (supplyCount(supplyPos, state) <1) {
        if (DEBUG)
            printf("There are not any of that type of card left\n");
        return -1;
    } else if (state->coins < getCost(supplyPos)) {
        if (DEBUG)
            printf("You do not have enough money to buy that. You have %d coins.\n", state->coins);
        return -1;
    } else {
        state->phase=1;
        //state->supplyCount[supplyPos]--; <-- don't need this gainCard() handles tally of supplyPos (the card #) in Supply
        gainCard(supplyPos, state, 0, who); //card goes in discard, this was wrong.. (2 means goes into hand, 0 goes into discard)

        state->coins = (state->coins) - (getCost(supplyPos));
        state->numBuys--;
        if (DEBUG)
            printf("You bought card number %d for %d coins. You now have %d buys and %d coins.\n", supplyPos, getCost(supplyPos), state->numBuys, state->coins);
    }

    //state->discard[who][state->discardCount[who]] = supplyPos;
    //state->discardCount[who]++;

    return 0;
}

int numHandCards(struct gameState *state) {
    return state->handCount[ whoseTurn(state) ];
}

int handCard(int handPos, struct gameState *state) {
    int currentPlayer = whoseTurn(state);
    return state->hand[currentPlayer][handPos];
}

int supplyCount(int card, struct gameState *state) {
    return state->supplyCount[card];
}

int fullDeckCount(int player, int card, struct gameState *state) {
    int i;
    int count = 0;

    for (i = 0; i < state->deckCount[player]; i++)
    {
        if (state->deck[player][i] == card) count++;
    }

    for (i = 0; i < state->handCount[player]; i++)
    {
        if (state->hand[player][i] == card) count++;
    }

    for (i = 0; i < state->discardCount[player]; i++)
    {
        if (state->discard[player][i] == card) count++;
    }

    return count;
}

int whoseTurn(struct gameState *state) {
    return state->whoseTurn;
}

/* *******************************************************  <---------------------------------------------------------------- NEW  FUNCTION 
	Determines who the next player is from whoseTurn member
	of gameState and adding 1. If after adding 1, that is
	greater than the numPlayers, we start over at player 0.
** ********************************************************/
int whoseNext(struct gameState *state) {
	int nextPlayer = state->whoseTurn + 1;
	if (nextPlayer > (state->numPlayers - 1)) {
		nextPlayer = 0;
	}
	return nextPlayer;
}

int endTurn(struct gameState *state) {
    int k;
    int i;
    int currentPlayer = whoseTurn(state);

    //Discard hand
    for (i = 0; i < state->handCount[currentPlayer]; i++) {
        state->discard[currentPlayer][state->discardCount[currentPlayer]++] = state->hand[currentPlayer][i];//Discard
        state->hand[currentPlayer][i] = -1;//Set card to -1
    }
    state->handCount[currentPlayer] = 0;//Reset hand count

    //Code for determining the player
    if (currentPlayer < (state->numPlayers - 1)) {
        state->whoseTurn = currentPlayer + 1;//Still safe to increment
    }
    else {
        state->whoseTurn = 0;//Max player has been reached, loop back around to player 1
    }

    state->outpostPlayed = 0;
    state->phase = 0;
    state->numActions = 1;
    state->coins = 0;
    state->numBuys = 1;
    state->playedCardCount = 0;
    state->handCount[state->whoseTurn] = 0;

    //int k; move to top
    //Next player draws hand
    for (k = 0; k < 5; k++) {
        drawCard(state->whoseTurn, state);//Draw a card
    }

    //Update money
    updateCoins(state->whoseTurn, state, 0);

    return 0;
}

int isGameOver(struct gameState *state) {
    int i;
    int j;

    //if stack of Province cards is empty, the game ends
    if (state->supplyCount[province] == 0)
    {
        return 1;
    }

    //if three supply pile are at 0, the game ends
    j = 0;
    for (i = 0; i < 25; i++)
    {
        if (state->supplyCount[i] == 0)
        {
            j++;
        }
    }
    if ( j >= 3)
    {
        return 1;
    }

    return 0;
}

int scoreFor (int player, struct gameState *state) {

    int i;
    int score = 0;
    //score from hand
    for (i = 0; i < state->handCount[player]; i++)
    {
        if (state->hand[player][i] == curse) {
            score = score - 1;
        };
        if (state->hand[player][i] == estate) {
            score = score + 1;
        };
        if (state->hand[player][i] == duchy) {
            score = score + 3;
        };
        if (state->hand[player][i] == province) {
            score = score + 6;
        };
        if (state->hand[player][i] == great_hall) {
            score = score + 1;
        };
        if (state->hand[player][i] == gardens) {
            score = score + ( fullDeckCount(player, 0, state) / 10 );
        };
    }

    //score from discard
    for (i = 0; i < state->discardCount[player]; i++)
    {
        if (state->discard[player][i] == curse) {
            score = score - 1;
        };
        if (state->discard[player][i] == estate) {
            score = score + 1;
        };
        if (state->discard[player][i] == duchy) {
            score = score + 3;
        };
        if (state->discard[player][i] == province) {
            score = score + 6;
        };
        if (state->discard[player][i] == great_hall) {
            score = score + 1;
        };
        if (state->discard[player][i] == gardens) {
            score = score + ( fullDeckCount(player, 0, state) / 10 );
        };
    }

    //score from deck
    for (i = 0; i < state->discardCount[player]; i++)
    {
        if (state->deck[player][i] == curse) {
            score = score - 1;
        };
        if (state->deck[player][i] == estate) {
            score = score + 1;
        };
        if (state->deck[player][i] == duchy) {
            score = score + 3;
        };
        if (state->deck[player][i] == province) {
            score = score + 6;
        };
        if (state->deck[player][i] == great_hall) {
            score = score + 1;
        };
        if (state->deck[player][i] == gardens) {
            score = score + ( fullDeckCount(player, 0, state) / 10 );
        };
    }

    return score;
}

int getWinners(int players[MAX_PLAYERS], struct gameState *state) {
    int i;
    int j;
    int highScore;
    int currentPlayer;

    //get score for each player
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        //set unused player scores to -9999
        if (i >= state->numPlayers)
        {
            players[i] = -9999;
        }
        else
        {
            players[i] = scoreFor (i, state);
        }
    }

    //find highest score
    j = 0;
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if (players[i] > players[j])
        {
            j = i;
        }
    }
    highScore = players[j];

    //add 1 to players who had less turns
    currentPlayer = whoseTurn(state);
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if ( players[i] == highScore && i > currentPlayer )
        {
            players[i]++;
        }
    }

    //find new highest score
    j = 0;
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if ( players[i] > players[j] )
        {
            j = i;
        }
    }
    highScore = players[j];

    //set winners in array to 1 and rest to 0
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if ( players[i] == highScore )
        {
            players[i] = 1;
        }
        else
        {
            players[i] = 0;
        }
    }

    return 0;
}

int drawCard(int player, struct gameState *state)
{   int count;
    int deckCounter;

    if (state->deckCount[player] <= 0) { //Deck is empty

	/*
	if(DEBUG){
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("Deck Count before %d\n", state->deckCount[player] );
		printDeck(player, state);
		printf("Discard Count before %d\n", state->discardCount[player] );
		printDiscard(player, state);
	}
	*/
        //Step 1 Shuffle the discard pile back into a deck
        int i;
        //Move discard to deck
        for (i = 0; i < state->discardCount[player]; i++) {
            state->deck[player][i] = state->discard[player][i];
		//printf("deck[%d]: %d\n", i, state->deck[player][i] );
		//printDeck(player, state);
            state->discard[player][i] = -1;
        }

        state->deckCount[player] = state->discardCount[player];
        state->discardCount[player] = 0;//Reset discard

	/*	
	if(DEBUG){
		printf("Deck Count after %d\n", state->deckCount[player] );
		printDeck(player, state);
		printf("Discard Count after %d\n", state->discardCount[player] );
		printDiscard(player, state);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	}
	*/

        //Shufffle the deck
        shuffle(player, state);//Shuffle the deck up and make it so that we can draw

        if (DEBUG) { //Debug statements
		//printf("++++++++++++++++++ after shuffle *****************\n");
            printf("Deck count now: %d\n", state->deckCount[player]);
        }

        //state->discardCount[player] = 0; <--- BUG ! if you use this..you'll only have coppers !!

	//Step 2 Draw Card
	count = state->handCount[player];//Get current player's hand count

	if (DEBUG) { //Debug statements
		printf("Current hand count: %d\n", count);
	}

	deckCounter = state->deckCount[player];//Create a holder for the deck count

	if (deckCounter == 0)
		return -1;

	state->hand[player][count] = state->deck[player][deckCounter -1 ];//Add card to hand
	state->deckCount[player]--;
	state->handCount[player]++;//Increment hand count
	/*
	if(DEBUG){
		printf("Hand Count MID %d\n", state->handCount[player] );
		printHand(player, state);

		printf("Deck Count MID %d\n", state->deckCount[player] );
		printDeck(player, state);
		
		printf("Discard Count MID %d\n", state->discardCount[player] );
		printDiscard(player, state);
		printf("END DRAW CARD --\n");
	}
	*/
   }
   else {
   	count = state->handCount[player];//Get current hand count for player
	if (DEBUG) { //Debug statements
		printf("Current hand count: %d\n", count);
	}

	deckCounter = state->deckCount[player];//Create holder for the deck count
	state->hand[player][count] = state->deck[player][deckCounter -1 ];//Add card to the hand
	state->deckCount[player]--;
	state->handCount[player]++;//Increment hand count

	/*
	if(DEBUG){
		printf("Hand Count ELSE %d\n", state->handCount[player] );
		printHand(player, state);

		printf("Deck Count ELSE %d\n", state->deckCount[player] );
		printDeck(player, state);
		
		printf("Discard Count ELSE %d\n", state->discardCount[player] );
		printDiscard(player, state);
		printf("END DRAW CARD --\n");
	}
	*/

   }
	/*
	if(DEBUG){
		printf("==================================================================\n");
		printf("Hand Count END %d\n", state->handCount[player] );
		printHand(player, state);

		printf("Deck Count END %d\n", state->deckCount[player] );
		printDeck(player, state);
		
		printf("Discard Count END %d\n", state->discardCount[player] );
		printDiscard(player, state);
		printf("END DRAW CARD --\n");
		printf("==================================================================\n");
	}
	*/

    	return 0;
}

int getCost(int cardNumber)
{
    switch( cardNumber )
    {
    case curse:
        return 0;
    case estate:
        return 2;
    case duchy:
        return 5;
    case province:
        return 8;
    case copper:
        return 0;
    case silver:
        return 3;
    case gold:
        return 6;
    case adventurer:
        return 6;
    case council_room:
        return 5;
    case feast:
        return 4;
    case gardens:
        return 4;
    case mine:
        return 5;
    case remodel:
        return 4;
    case smithy:
        return 4;
    case village:
        return 3;
    case baron:
        return 4;
    case great_hall:
        return 3;
    case minion:
        return 5;
    case steward:
        return 3;
    case tribute:
        return 5;
    case ambassador:
        return 3;
    case cutpurse:
        return 4;
    case embargo:
        return 2;
    case outpost:
        return 5;
    case salvager:
        return 4;
    case sea_hag:
        return 4;
    case treasure_map:
        return 4;
    }

    return -1;
}

/* BARON */ 
int baronCard(int choice1, struct gameState *state)
{
	int currentPlayer = whoseTurn(state);

	state->numBuys++;//Increase buys by 1!
	if (choice1 > 0) { //Boolean true or going to discard an estate

		int p;//Iterator for hand!
		for (p = 0; p < state->handCount[currentPlayer]; p++)
		{
			if (state->hand[currentPlayer][p] == estate) { //Found an estate card!
				state->coins += 4;//Add 4 coins to the amount of coins
				discardCard(p, currentPlayer, state);
				//break; <== NEW BUG # 1 !

				/* Since I had to refactor code so much, I am adding a new bug that DOES NOT stop
					after finding the first estate..subsequent estates will be discarded as well
					and +4 coins added. Super-cheating */
			}
		}
	}

	else {
		if (supplyCount(estate, state) >= 0) { /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BUG 2: >= wrong, > correct */
			gainCard(estate, state, 0, currentPlayer);//Gain an estate

			if (supplyCount(estate, state) == 0) {
				isGameOver(state);
			}
		}
	}

	return 0;
}

/* MINION */ /* <------------------------------------------------------------------------------------- NEW MINION FUNCTION HERE */
int minionCard(int choice1, int choice2, struct gameState *state, int handPos)
{
	int i;
	int j;

	int currentPlayer = whoseTurn(state);

	//+1 action
	state->numActions++;

	//discard card from hand
	//discardCard(handPos, currentPlayer, state);

	if (choice1)
	{
		state->coins = state->coins + 2;
	}
	/* vvvvvvvvvvvvvvvvv <<<<<<<<<<<<<<<<<< BUG 4: else if = correct, if = WRONG. Doing this..so long as choice2 is a positive enum value it
	                                        will pass meaning.. you get +2 coin AND you get to discard ...but BUG 3 below will only allow you
											to gain 3 cards instead of 4 */

	if (choice2)		//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
	{
		//discard hand
		while (numHandCards(state) > 0)
		{
			discardCard(0, currentPlayer, state); /* CHANGED: since shiftCards() goes left..this will work to remove all  */
		}

		//draw 4
		for (i = 0; i < 3; i++) /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BUG 3: Player only draws 3 cards, so everyone
								                                                 else playing will get 4 cards. Oops. */
		{
			drawCard(currentPlayer, state);
		}

		//other players discard hand and redraw if hand size > 4
		for (i = 0; i < state->numPlayers; i++)
		{
			if (i != currentPlayer)
			{
				if (state->handCount[i] > 4)
				{
					//discard hand
					while (state->handCount[i] > 0)
					{
						discardCard(0, i, state); /* CHANGED: b/c of shiftCards() can start at 0..all others will SHIFT left  */
					}

					//draw 4
					for (j = 0; j < 4; j++)
					{
						if(DEBUG){
							printf("MC <== before drawCard() called\n");
							printf("player %d Discard Count: %d\n", i, state->discardCount[i] );
							printDiscard(i, state);
							printf("Player %d Hand Count: %d\n", i, state->handCount[i] );
							printHand(i, state);
							printf("MC\n");
						}	

						drawCard(i, state);

						if(DEBUG){
							printf("MC <== after drawCard() called\n");
							printf("player %d Discard Count: %d\n", i, state->discardCount[i] );
							printDiscard(i, state);
							printf("Player %d Hand Count: %d\n", i, state->handCount[i] );
							printHand(i, state);
							printf("MC\n");
						}	

					}
				}
			}
		}

	}
	return 0;
}

/* AMBASSADOR */ /* <------------------------------------------------------------------------------------- NEW AMBASSADOR FUNCTION HERE */
int ambassadorCard(int choice1, int choice2, struct gameState *state, int handPos)
{
	int i;
	int j;
	int currentPlayer = whoseTurn(state);

	j = 0;		//used to check if player has enough cards to discard

	if (choice2 > 2 || choice2 < 0)
	{
		return -1;
	}

	if (choice1 == handPos) 
	{
		return -1;
	}

	for (i = 0; i < state->handCount[currentPlayer]; i++)
	{
		if (i == handPos && i == state->hand[currentPlayer][choice1] && i != choice1) /* <<<<<<<<<<<<<<<<<< BUG 5: GOOD = i != handPos, BAD = i == handPos
														    This bug allows for the player to discard the
														    card in hand at handPos. You get rid of more
														    more of your "junk" cards this way. */
		{
			j++;
		}
	}
	if (j < choice2)
	{
		return -1;
	}

	if (DEBUG)
	{
		printf("Player %d reveals card number: %d\n", currentPlayer, state->hand[currentPlayer][choice1]);
	}

	//increase supply count for choosen card by amount being discarded
	state->supplyCount[state->hand[currentPlayer][choice1]] += choice2;

	//each other player gains a copy of revealed card
	for (i = 0; i < state->numPlayers; i++)
	{
		if (i != currentPlayer)
		{
			gainCard(state->hand[currentPlayer][choice1], state, 0, i);
		}
	}

	//discard played card from hand
	discardCard(handPos, currentPlayer, state ); /* <<<<<<<<<<<<<<<<<<<<<<< BUG 6: Incorrect function called..should be trashCard not discardCard */

	//trash copies of cards returned to supply
	for (j = 0; j < choice2; j++)
	{
		for (i = 0; i < state->handCount[currentPlayer]; i++)
		{
			if (state->hand[currentPlayer][i] == state->hand[currentPlayer][choice1])
			{
				trashCard(i, currentPlayer, state, TRASH);
				break;
			}
		}
	}

	return 0;

}

/* TRIBUTE */ /* <------------------------------------------------------------------------------------- NEW TRIBUTE FUNCTION HERE */
int tributeCard(struct gameState *state)
{
	int i;
	int currentPlayer = whoseTurn(state);
	int nextPlayer = whoseNext(state);

	int tributeRevealedCards[2] = { -1, -1 };

	if ((state->discardCount[nextPlayer] + state->deckCount[nextPlayer]) < 1) { /* <<<<<<<<<<<<<<<< BUG 7: should be <= 1 NOT < 1
																				                     If this is < 1, the nested branches below it
																									 will never be true. You cannot have > 0 and < 0
																									 unless you are at 0, which means the lower
																									 condition will always execute, and the nextPlayer
																									 will always have their deck reshuffled. This can
																									 affect game play*/
		if (state->deckCount[nextPlayer] > 0) {
			tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
			state->deckCount[nextPlayer]--;
		}
		else if (state->discardCount[nextPlayer] > 0) {
			tributeRevealedCards[0] = state->discard[nextPlayer][state->discardCount[nextPlayer] - 1];
			state->discardCount[nextPlayer]--;
		}
		else {
			//No Card to Reveal
			if (DEBUG) {
				printf("No cards to reveal\n");
			}
		}
	}

	else {
		if (state->deckCount[nextPlayer] == 0) {
			for (i = 0; i < state->discardCount[nextPlayer]; i++) {
				state->deck[nextPlayer][i] = state->discard[nextPlayer][i];//Move to deck
				/* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BUG 8: forgot to increment player's deckCount.
				                                                                                Not incrementing it in this case while their
																								discard increases, but combined with BUG 7, means
																								they would always have 0 (ZERO) cards in their hand,
																								but it would also mean the positions of the cards in
																								the deck would be off */
				//state->deckCount[nextPlayer]++; 
				state->discard[nextPlayer][i] = -1;
				state->discardCount[nextPlayer]--;
			}
			shuffle(nextPlayer, state);//Shuffle the deck
		}
		tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
		state->deck[nextPlayer][state->deckCount[nextPlayer]--] = -1;
		state->deckCount[nextPlayer]--;
		tributeRevealedCards[1] = state->deck[nextPlayer][state->deckCount[nextPlayer] - 1];
		state->deck[nextPlayer][state->deckCount[nextPlayer]--] = -1;
		state->deckCount[nextPlayer]--;
	}

	if (tributeRevealedCards[0] == tributeRevealedCards[1]) { //If we have a duplicate card, just drop one
		state->playedCards[state->playedCardCount] = tributeRevealedCards[1];
		state->playedCardCount++;
		tributeRevealedCards[1] = -1;
	}

	for (i = 0; i <= 2; i++) {
		if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold) { //Treasure cards
			state->coins += 2;
		}

		else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall) { //Victory Card Found
			drawCard(currentPlayer, state);
			drawCard(currentPlayer, state);
		}
		else { //Action Card
			state->numActions = state->numActions + 2;
		}
	}

	return 0;

}

/* MINE */ /* <------------------------------------------------------------------------------------- NEW MINE FUNCTION HERE */
int mineCard(int choice1, int choice2, struct gameState *state, int handPos)
{
	int i;
	int j;
	int currentPlayer = whoseTurn(state);

	j = state->hand[currentPlayer][choice1];  //store card we will trash

	if (state->hand[currentPlayer][choice1] < copper || state->hand[currentPlayer][choice1] > gold)
	{
		return -1;
	}

	if (choice2 > treasure_map || choice2 < curse)
	{
		return 1; /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BUG 9: forgot the -1. Good = -1, Bad = 1. This means it will 
				                                                      return true if the choice is "off the map" or not in the deck */
	}

	if ((getCost(state->hand[currentPlayer][choice1]) + 3) >= getCost(choice2))
	{
		return -1;
	}

	gainCard(choice2, state, 2, currentPlayer);

	//discard card from hand
	discardCard(handPos, currentPlayer, state);

	//discard trashed card
	for (i = 0; j < state->handCount[currentPlayer]; i++) /* <<<<<<<<<<<<< BUG 10: Good = i < state->handCount[currentPlayer];
														                           BAD = j < state->handCount[currentPlayer];
																				   if j happens to be an enum of a card larger
																				   than the handCount of the current player then
																				   the player ends up keeping the treasure that
																				   they were supposed to trash. Unfair advantage */
	{
		if (state->hand[currentPlayer][i] == j)
		{
			trashCard(i, currentPlayer, state, TRASH);
			break;
		}
	}

	return 0;
}

int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
{
    int i;
    int j;
    int k;
    int x;
    int index;
    int currentPlayer = whoseTurn(state);
	int nextPlayer = whoseNext(state);

    int temphand[MAX_HAND];// moved above the if statement
    int drawntreasure=0;
    int cardDrawn;
    int z = 0;// this is the counter for the temp hand
    if (nextPlayer > (state->numPlayers - 1)) {
        nextPlayer = 0;
    }

    //uses switch to select card and perform actions
    switch( card )
    {
    case adventurer:
        while(drawntreasure<2) {
            if (state->deckCount[currentPlayer] <1) { //if the deck is empty we need to shuffle discard and add to deck
                shuffle(currentPlayer, state);
            }
            drawCard(currentPlayer, state);
            cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
            if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
                drawntreasure++;
            else {
                temphand[z]=cardDrawn;
                state->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
                z++;
            }
        }
        while(z-1>=0) {
            state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
            z=z-1;
        }
        return 0;

    case council_room:
        //+4 Cards
        for (i = 0; i < 4; i++)
        {
            drawCard(currentPlayer, state);
        }

        //+1 Buy
        state->numBuys++;

        //Each other player draws a card
        for (i = 0; i < state->numPlayers; i++)
        {
            if ( i != currentPlayer )
            {
                drawCard(i, state);
            }
        }

        //put played card in played card pile
        discardCard(handPos, currentPlayer, state);

        return 0;

    case feast:
        //gain card with cost up to 5
        //Backup hand
        for (i = 0; i <= state->handCount[currentPlayer]; i++) {
            temphand[i] = state->hand[currentPlayer][i];//Backup card
            state->hand[currentPlayer][i] = -1;//Set to nothing
        }
        //Backup hand

        //Update Coins for Buy
        updateCoins(currentPlayer, state, 5);
        x = 1;//Condition to loop on
        while( x == 1) {//Buy one card
            if (supplyCount(choice1, state) <= 0) {
                if (DEBUG)
                    printf("None of that card left, sorry!\n");

                if (DEBUG) {
                    printf("Cards Left: %d\n", supplyCount(choice1, state));
                }
            }
            else if (state->coins < getCost(choice1)) {
                printf("That card is too expensive!\n");

                if (DEBUG) {
                    printf("Coins: %d < %d\n", state->coins, getCost(choice1));
                }
            }
            else {

                if (DEBUG) {
                    printf("Deck Count: %d\n", state->handCount[currentPlayer] + state->deckCount[currentPlayer] + state->discardCount[currentPlayer]);
                }

                gainCard(choice1, state, 0, currentPlayer);//Gain the card
                x = 0;//No more buying cards

                if (DEBUG) {
                    printf("Deck Count: %d\n", state->handCount[currentPlayer] + state->deckCount[currentPlayer] + state->discardCount[currentPlayer]);
                }

            }
        }

        //Reset Hand
        for (i = 0; i <= state->handCount[currentPlayer]; i++) {
            state->hand[currentPlayer][i] = temphand[i];
            temphand[i] = -1;
        }
        //Reset Hand

        return 0;

    case gardens:
        return -1;

	case mine:	/*<---------------------------------  CHANGE to FUNCTION "mineCard()" FUNCTION */

		if ( mineCard(choice1, choice2, state, handPos) != 0  && DEBUG) 
		{
			fprintf( stderr, "%s\n", "BAD CARD: mine" ); /*<---- TRACE for DEBUG ONLY */
			return -1;
		} 

    case remodel:
        j = state->hand[currentPlayer][choice1];  //store card we will trash

        if ( (getCost(state->hand[currentPlayer][choice1]) + 2) > getCost(choice2) )
        {
            return -1;
        }

        gainCard(choice2, state, 0, currentPlayer);

        //discard card from hand
        discardCard(handPos, currentPlayer, state);

        //discard trashed card
        for (i = 0; i < state->handCount[currentPlayer]; i++)
        {
            if (state->hand[currentPlayer][i] == j)
            {
                trashCard(i, currentPlayer, state, TRASH);
                break;
            }
        }


        return 0;

    case smithy:
        //+3 Cards
        for (i = 0; i < 3; i++)
        {
            drawCard(currentPlayer, state);
        }

        //discard card from hand
        discardCard(handPos, currentPlayer, state);
        return 0;

    case village:
        //+1 Card
        drawCard(currentPlayer, state);

        //+2 Actions
        state->numActions = state->numActions + 2;

        //discard played card from hand
        discardCard(handPos, currentPlayer, state);
        return 0;

    case baron:

		if (baronCard(choice1, state) != 0) /*<----------------------------------------------  CHANGE to FUNCTION "baronCard()" FUNCTION */
		{
			fprintf(stderr, "%s\n", "BAD CARD: baron"); /*<---- TRACE for DEBUG ONLY */
			return -1;
		}

    case great_hall:
        //+1 Card
        drawCard(currentPlayer, state);

        //+1 Actions
        state->numActions++;

        //discard card from hand
        discardCard(handPos, currentPlayer, state);
        return 0;

    case minion: /*<----------------------------------------------  CHANGE to FUNCTION "minionCard()" FUNCTION */

		if ( minionCard(choice1, choice2, state, handPos) != 0 )
		{
			fprintf(stderr, "%s\n", "BAD CARD: minion"); /*<---- TRACE for DEBUG ONLY */
			return -1;
		}

    case steward:
        if (choice1 == 1)
        {
            //+2 cards
            drawCard(currentPlayer, state);
            drawCard(currentPlayer, state);
        }
        else if (choice1 == 2)
        {
            //+2 coins
            state->coins = state->coins + 2;
        }
        else
        {
            //trash 2 cards in hand
            trashCard(choice2, currentPlayer, state, TRASH);
            trashCard(choice3, currentPlayer, state, TRASH);
        }

        //discard card from hand
        discardCard(handPos, currentPlayer, state);
        return 0;

    case tribute: /*<----------------------------------------------  CHANGE to FUNCTION "tributeCard()" FUNCTION */

		if ( tributeCard(state) != 0 )
		{
			fprintf(stderr, "%s\n", "BAD CARD: tribute"); /*<---- TRACE for DEBUG ONLY */
			return -1;
		}

    case ambassador: /*<----------------------------------------------  CHANGE to FUNCTION "ambassadorCard()" FUNCTION */

		if ( ambassadorCard(choice1, choice2, state, handPos) != 0)
		{
			fprintf(stderr, "%s\n", "BAD CARD: ambassador"); /*<---- TRACE for DEBUG ONLY */
			return -1;
		}

    case cutpurse:

        updateCoins(currentPlayer, state, 2);
        for (i = 0; i < state->numPlayers; i++)
        {
            if (i != currentPlayer)
            {
                for (j = 0; j < state->handCount[i]; j++)
                {
                    if (state->hand[i][j] == copper)
                    {
                        discardCard(j, i, state);
                        break;
                    }
                    if (j == state->handCount[i])
                    {
                        for (k = 0; k < state->handCount[i]; k++)
                        {
                            if (DEBUG)
                                printf("Player %d reveals card number %d\n", i, state->hand[i][k]);
                        }
                        break;
                    }
                }

            }

        }

        //discard played card from hand
        discardCard(handPos, currentPlayer, state);

        return 0;


    case embargo:
        //+2 Coins
        state->coins = state->coins + 2;

        //see if selected pile is in play
        if ( state->supplyCount[choice1] == -1 )
        {
            return -1;
        }

        //add embargo token to selected supply pile
        state->embargoTokens[choice1]++;

        //trash card
        trashCard(handPos, currentPlayer, state, TRASH);
        return 0;

    case outpost:
        //set outpost flag
        state->outpostPlayed++;

        //discard card
        discardCard(handPos, currentPlayer, state);
        return 0;

    case salvager:
        //+1 buy
        state->numBuys++;

        if (choice1)
        {
            //gain coins equal to trashed card
            state->coins = state->coins + getCost( handCard(choice1, state) );
            //trash card
            trashCard(choice1, currentPlayer, state, TRASH);
        }

        //discard card
        discardCard(handPos, currentPlayer, state);
        return 0;

    case sea_hag:
        for (i = 0; i < state->numPlayers; i++) {
            if (i != currentPlayer) {
                state->discard[i][state->discardCount[i]] = state->deck[i][state->deckCount[i]--];
                state->deckCount[i]--;
                state->discardCount[i]++;
                state->deck[i][state->deckCount[i]--] = curse;//Top card now a curse
            }
        }
        return 0;

    case treasure_map:
        //search hand for another treasure_map
        index = -1;
        for (i = 0; i < state->handCount[currentPlayer]; i++)
        {
            if (state->hand[currentPlayer][i] == treasure_map && i != handPos)
            {
                index = i;
                break;
            }
        }
        if (index > -1)
        {
            //trash both treasure cards
            trashCard(handPos, currentPlayer, state, TRASH);
            trashCard(index, currentPlayer, state, TRASH);

            //gain 4 Gold cards
            for (i = 0; i < 4; i++)
            {
                gainCard(gold, state, 1, currentPlayer);
            }

            //return success
            return 1;
        }

        //no second treasure_map found in hand
        return -1;
    }

    return -1;
}

/************************************************************************* <---------------------------------------------------------------- NEW  FUNCTION 
*
* Function: shiftCards()
* Params:
*		handPos: integer corresponds to position in gameState hand array
*		currentPlayer: integer of player shifting cards
*		state: struct gameState pointer object that stores specifics
*				of game play for current player's turn
* Description:
*		Shifts cards in currentPlayer's hand over to the left. Used by
*		discardCard() and/or trashCard(). Will not iterate if only 1 card.
* Returns:
*		SUCCESS flag (zero), only if handCount of player > 1
*
**************************************************************************/
int shiftCards(int handPos, int currentPlayer, struct gameState *state)
{
	int result = FAILURE;

	/* check if at least 1 card in hand */
	if (state->handCount[currentPlayer] > 1)
	{
		int i;
		for (i = handPos; i <= state->handCount[currentPlayer]; i++) {

			state->hand[currentPlayer][i] = state->hand[currentPlayer][i + 1];
		}

		/* last place in hand is garbage. Mark unused (= -1 ) */
		state->hand[currentPlayer][i] = UNUSED;
		result = SUCCESS;
	}
	return result;
}


/************************************************************************* <---------------------------------------------------------------- NEW  FUNCTION 
*
* Function: discardCard()
* Params:
*		1. handPos: integer corresponds to position in gameState hand array
*		2. currentPlayer: integer of player shifting cards
*		3. state: struct gameState pointer object that stores specifics
*				of game play for current player's turn
*		4. trashFlag: 1 = TRASH, 0 = DISCARD (MACROS available)
*
* Description:
*		1. Discards a SINGLE card to the currentPlayer's discard pile
*		held by the gameState pointer "state." 
*		2. increments discardCount of currentPlayer
*		3. sets position of hand to UNUSED (-1)
*		4. decrements hand count
*		5. calls shiftCard() function. SEE shiftCard specs.
*
* Returns:
*		if more than 1 card, and shiftCard() executes a FAILURE
*		FAILURE returned by discardCard()
*
**************************************************************************/

int discardCard(int handPos, int currentPlayer, struct gameState *state)
{
	int action_success = FAILURE;

	state->discard[currentPlayer][(state->discardCount[currentPlayer])] = state->hand[currentPlayer][handPos];
	state->discardCount[currentPlayer]++;

	state->hand[currentPlayer][handPos] = UNUSED;

	/* adjust what's in hand */
	action_success = shiftCards(handPos, currentPlayer, state); 

	/* NOTE: important to increment AFTER call to shiftCards() or handCount will be off ! */
	state->handCount[currentPlayer]--;

	if( (state->handCount[currentPlayer] > 1) && (action_success != FAILURE) )
	{ action_success = SUCCESS; }

    return action_success;
}

/************************************************************************* 
*
* Function: trashCard()
* Params:
*		1. handPos: integer corresponds to position in gameState hand array
*		2. currentPlayer: integer of player shifting cards
*		2. state: struct gameState pointer object that stores specifics
*				of game play for current player's turn
*		4. trashFlag: 1 = TRASH, 0 = DISCARD (MACROS available)
*
* Description:
*		1. Trashes a SINGLE card to the gameState trashPile
*		held by the gameState pointer "state."
*		2. increments int trashCount
*		3. sets position of hand to UNUSED (-1)
*		4. decrements hand count
*		5. calls shiftCard() function. SEE shiftCard specs.
*
* Returns:
*		if more than 1 card, and shiftCard() executes a FAILURE
*		FAILURE returned by trashCard()
*
**************************************************************************/
int trashCard(int handPos, int currentPlayer, struct gameState* state )
{
	int action_success = FAILURE;

	//if card is not trashed, added to currentPlayer's discard pile

	state->trashPile[state->trashCount] = state->hand[currentPlayer][handPos];
	state->trashCount++;

	state->hand[currentPlayer][handPos] = UNUSED;

	/* adjust what's in hand */
	action_success = shiftCards(handPos, currentPlayer, state);
		
	state->handCount[currentPlayer]--;

	if ((state->handCount[currentPlayer] > 1) && (action_success != FAILURE))
	{ action_success = SUCCESS;}

	return action_success;
}

int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)
{
    //Note: supplyPos is enum of choosen card

    //check if supply pile is empty (0) or card is not used in game (-1)
    
    /*
    if ( supplyCount(supplyPos, state) < 1 )
    {
        return -1;
    }
    */	

    //added card for [whoseTurn] current player:
    // toFlag = 0 : add to discard
    // toFlag = 1 : add to deck
    // toFlag = 2 : add to hand

    if (toFlag == 1)
    {
        state->deck[ player ][ state->deckCount[player] ] = supplyPos;
        state->deckCount[player]++;
    }
    else if (toFlag == 2)
    {
        state->hand[ player ][ state->handCount[player] ] = supplyPos;
        state->handCount[player]++;
    }
    else
    {
        state->discard[player][ state->discardCount[player] ] = supplyPos;
        state->discardCount[player]++;
    }

    //decrease number in supply pile
    state->supplyCount[supplyPos]--;

    return 0;
}

int updateCoins(int player, struct gameState *state, int bonus)
{
    int i;

    //reset coin count
    state->coins = 0;

    //add coins for each Treasure card in player's hand
    for (i = 0; i < state->handCount[player]; i++)
    {
        if (state->hand[player][i] == copper)
        {
            state->coins += 1;
        }
        else if (state->hand[player][i] == silver)
        {
            state->coins += 2;
        }
        else if (state->hand[player][i] == gold)
        {
            state->coins += 3;
        }
    }

    //add bonus
    state->coins += bonus;

    return 0;
}


//end of dominion.c

