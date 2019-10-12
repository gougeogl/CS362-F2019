int whoseNext(struct gameState *state) {
	int nextPlayer = state->whoseTurn + 1;
	if (nextPlayer > (state->numPlayers - 1)) {
		nextPlayer = 0;
	}
	return nextPlayer;
}
