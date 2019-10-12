int minionCard(int choice1, int choice2, struct gameState *state, int handPos)
{
	int i;
	int j;

	int currentPlayer = whoseTurn(state);
	int nextPlayer = whoseNext(state);

	//+1 action
	state->numActions++;

	//discard card from hand
	discardCard(handPos, currentPlayer, state, 0);

	if (choice1)
	{
		state->coins = state->coins + 2;
	}
	else if (choice2)		//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
	{
		//discard hand
		while (numHandCards(state) > 0)
		{
			discardCard(handPos, currentPlayer, state, 0);
		}

		//draw 4
		for (i = 0; i < 4; i++)
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
						discardCard(handPos, i, state, 0);
					}

					//draw 4
					for (j = 0; j < 4; j++)
					{
						drawCard(i, state);
					}
				}
			}
		}

	}
	return 0;
}