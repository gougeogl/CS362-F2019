/* *************************************************************************
* save - previous - hands
* Saves hand of all players after initial testing setup 
*
****************************************************************************/
void savePreviousHands(int** table, struct gameState* state )
{
	int numPlayers = state->numPlayers;

	int i, j;
	for(i = 0; i < numPlayers; i++)
	{
		for(j = 0; j < state->hand[i]; j++)
		{
			container[i][j] = state->handCount[i][j];	
			printf("container[player i %d][handPos j %d]: %d\n", i, j, container[i][j] );
		}
	}
}
