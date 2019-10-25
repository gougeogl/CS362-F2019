/****************************************************************
** FileName: unittest1.c
** Author: Glen Gougeon
** Class: CS362 Software Engineering II
** Date: 10-25-2019
** Last Mod: 10-25-2019
*
** Description:	Assignment 3: Unit Testing : 
*		Refactored code for Baron, has 2 bugs I introduced. 
*		The following test code should catch the errors while
*		executing 80% branch and function coverage according
*		to gcov coverage tool.
*
****************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include "rngs.h"

int test1();
int test2();

int main()
{
	int overall_stat = -1, check;

	check = test1();
	if (check == -1)
	{ printf("Test 1 failed.\n"); }
	else { overall_stat = 0; }

	check = test2();
	if (check == -1)
	{ printf("Test 2 failed.\n"); }
	else { overall_stat = 0; }

	if( overall_stat == -1 )
	{ printf("ALL Baron Tests Failed!!\n"); }

	return 0;
}

/* returns 0 if pass, -1 if fail */
int test1()
{
	int test_1_stat = -1;

	if(test_1_stat != -1)
	{ test_1_stat = 0; }

	return test_1_stat;
}

/* returns 0 if pass, -1 if fail */
int test2()
{
	int test_2_stat = -1;

	if (test_2_stat != -1)
	{ test_2_stat = 0; }

	return test_2_stat;
}
