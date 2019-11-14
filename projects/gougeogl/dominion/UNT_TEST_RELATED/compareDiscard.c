/* *************************************************************************
* discard
* Check is discard of a player is same / different before some saved
* gameState (before)
* if discard is different, will print the difference.
* if discard is same, will warn hand is the same.
* S.T. flag of desired comparison SAME_DISCARD, DIFFERENT_DISCARD
*
****************************************************************************/
int compareDiscard(int player, struct gameState* before, struct gameState* after, int limit, int flag)
{
	int result = 0;
	int printStats = 0;

	int i;
	for (i = 0; i < limit; i++)
	{
		if (flag == SAME_DISCARD)
		{
			if (before->discard[player][i] != after->discard[player][i]) {
				printf("player %d\'s discard is different\n", player);
				fflush(stdout);
				printStats = 1;	
			}
		}
		else if (flag == DIFFERENT_DISCARD)
		{
			if (before->discard[player][i] == after->discard[player][i]) {
				printf("player %d\'s discard is still the same\n", player);
				fflush(stdout);
				printStats = 1;	
			}
		}

		if(printStats == 1)
		{
			char name[MAX_STRING_LENGTH];
			memset(name, '\0', sizeof(name));
			cardNumToName(before->discard[player][i], name);

			char nombre[MAX_STRING_LENGTH];
			memset(nombre, '\0', sizeof(nombre));
			cardNumToName(after->discard[player][i], nombre);

			printf("BEFORE: discard[%d]: %s, AFTER: discard[%d]: %s\n", i, name, i, nombre );
			fflush(stdout);
			result = -1;
		}

	}
	return result;
}
