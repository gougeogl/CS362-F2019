# **********************************************************
# FileName: 		Makefile
# Author:			Glen Gougeon
# GroupName:		EvenBetter
# GroupMemebers:	Samnang Penh, Reuben Youngblom, & Author
# GitHubRepo:		tbd
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Class:	CS362 Software Engineering II
# Date:		11-11-2019
# Last Mod:	11-11-2019
#
# Assignment:	Final Project Part B:
#				Dominion Unit Test/Fix Bugs
#
# Description:	Compiles and runs unit tests of code base
#				for Dominion Code Base, for listed bugs in
#				list given to class.
#
# ***********************************************************

CC = gcc # <== compiler option
CFLAGS = -Wall 
CFLAGS += -fpic 
CFLAGS += -coverage 
CFLAGS += -lm # <== req'd. for floor() of <math.h> & others??
CFLAGS += -std=c99

# ** Code Coverage **
COV = gcov # coverage tool
COV += -f # provides function coverage
COV += -b # provides branch coverage

rngs.o: rngs.h rngs.c
	${CC} -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	${CC} -c dominion.c -g  $(CFLAGS)

interface.o: interface.h interface.c
	${CC} -c interface.c -g  $(CFLAGS)

# **** UNIT TESTS ********************************************************************
testDrawCard: testDrawCard.c dominion.o rngs.o
	${CC} -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	${COV} dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

# **** GAME PLAY *********************************************************************	
playdom: dominion.o playdom.c
	${CC} -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

player: player.c interface.o
	${CC} -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

# **** DESTRUCTION *******************************************************************
clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out *.exe
	rm -f testDrawCard 
