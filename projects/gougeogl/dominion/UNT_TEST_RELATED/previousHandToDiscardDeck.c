/* *************************************************************************
* previous - hand - to - discard - check
* Check if previous hand of discard is currently found in the player's 
* discard pile, up to 'limit' number of card. 
* S.T. flag of desired limit of cards to check. 
*
****************************************************************************/
int previousHandToDiscardCheck(int player, struct gameState* before, struct gameState* after, int limit )
{
	int result = 0;
	char name[MAX_STRING_LENGTH];
	char nombre[MAX_STRING_LENGTH];


	printf("IN previous HandToDiscardCheck **\n");
	printf("after->discardCount[player %d]: %d\n", player, after->discardCount[player] );

	int last_discard_idx = after->discardCount[player];
	printf("last_discard_idx: %d\n", last_discard_idx ); 

	last_discard_idx--;
	printf("last_discard_idx--: %d\n", last_discard_idx-- ); 

	int container[MAX_DECK] = { -1 };

	int i;
	for(i = 0; i < limit; i++)
	{
		printf("last_discard_idx: %d\n", last_discard_idx );
		printf("after->discard[player %d][last_discard_idx]: %d\n", player, after->discard[player][last_discard_idx] );
		container[i] = after->discard[player][last_discard_idx];
		printf("container[%d]: %d\n", i, container[i] );
		last_discard_idx--;
	}

	int j;
	for (j = 0; j < limit; j++)
	{
		if (before->hand[player][j] != container[j] ) 
		{
			printf("player %d\'s discard doesn't contain previous hand\n", player);
			fflush(stdout);

			memset(name, '\0', sizeof(name));
			cardNumToName(before->hand[player][j], name);

			memset(nombre, '\0', sizeof(nombre));
			cardNumToName(container[j], nombre);

			printf("BEFORE: hand[%d]: %s, AFTER: discard[%d]: %s\n", i, name, i, nombre );
			fflush(stdout);
			result = -1;
		}
	}
	return result;
}
