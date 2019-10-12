int mineCard(int choice1, int choice2, struct gameState *state, int handPos)
{
	int i;
	int j;
	int index;
	int currentPlayer = whoseTurn(state);
	int nextPlayer = whoseNext(state);

	j = state->hand[currentPlayer][choice1];  //store card we will trash

	if (state->hand[currentPlayer][choice1] < copper || state->hand[currentPlayer][choice1] > gold)
	{
		return -1;
	}

	if (choice2 > treasure_map || choice2 < curse)
	{
		return -1;
	}

	if ((getCost(state->hand[currentPlayer][choice1]) + 3) > getCost(choice2))
	{
		return -1;
	}

	gainCard(choice2, state, 2, currentPlayer);

	//discard card from hand
	discardCard(handPos, currentPlayer, state, 0);

	//discard trashed card
	for (i = 0; i < state->handCount[currentPlayer]; i++)
	{
		if (state->hand[currentPlayer][i] == j)
		{
			discardCard(i, currentPlayer, state, 1);
			break;
		}
	}

	return 0;
}