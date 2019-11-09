# ********************************************************
# FileName: 	Makefile
# Edited by: 	Glen Gougeon
# Date: 	10-26-2019
#
# Description: 	Makefile for Assignment 3 unit testing
# 		compilation of refactored functions
#
# ********************************************************
CC = gcc # compiler
CFLAGS = -Wall # warnings all 
CFLAGS += -fpic 
CFLAGS += -coverage # needed for gcov 
CFLAGS += -lm # needed for floor() function in <math.h> 
#CFLAGS += -fprofile-arcs # needed for .gcda files related to gcov 

# gcov call and flags
GCOV = gcov # coverage tool 
GCOV += -b # branch info
GCOV += -f # function info

rngs.o: rngs.h rngs.c
	${CC} -c rngs.c -g  $(CFLAGS) 

dominion.o: dominion.h dominion_helpers.h dominion.c rngs.o
	${CC} -c dominion.c -g  $(CFLAGS) 

playdom: dominion.o playdom.c
	${CC} -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS) 
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	${CC} -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS) 

interface.o: interface.h interface.c
	${CC} -c interface.c -g  $(CFLAGS) 

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	${GCOV} dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
#***********************************************************************

unittest1: unittest1.c dominion.o rngs.o interface.o 
	${CC} -o unittest1 unittest1.c -g dominion.o rngs.o interface.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o interface.o 
	${CC} -o unittest2 unittest2.c -g dominion.o rngs.o interface.o $(CFLAGS)

#unittestresults: unittest1 
#	./unittest1 &> unittestresults.out 
#	${GCOV} unittest1.c >> unittestresults.out
#	cat unittest1.c.gcov >> unittestresults.out
#	${GCOV} dominion.c >> unittestresults.out
#	cat dominion.c.gcov >> unittestresults.out

#***********************************************************************

unittestresults: unittest1 unittest2 
	./unittest1 &> unittestresults.out 
	./unittest2 >> unittestresults.out
	${GCOV} dominion.c >> unittestresults.out
	${GCOV} unittest1.c >> unittestresults.out
	${GCOV} unittest2.c >> unittestresults.out
	cat unittest1.c.gcov >> unittestresults.out
	cat unittest2.c.gcov >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

#***********************************************************************
testDiscardCard: testDiscardCard.c dominion.o rngs.o interface.o 
	${CC} -o testDiscardCard testDiscardCard.c -g dominion.o rngs.o interface.o $(CFLAGS)

discardresults: testDiscardCard 
	./testDiscardCard &> discardresults.out 
	${GCOV} testDiscardCard.c >> discardresults.out 
	cat testDiscardCard.c.gcov >> discardresults.out 
	${GCOV} dominion.c >> discardresults.out 
	cat dominion.c.gcov >> discardresults.out 

#***********************************************************************

testInitGame: testInitGame.c dominion.o rngs.o interface.o
	${CC} -o testInitGame testInitGame.c -g dominion.o rngs.o interface.o ${CFLAGS}

testInitGameResults: testInitGame
	./testInitGame &> testinitgameresults.out
	${GCOV} dominion.c >> testinitgameresults.out
	${GCOV} testInitGame.c >> testinitgameresults.out
	cat dominion.c.gcov >> testinitgameresults.out
	cat testInitGame.c.gcov >> testinitgameresults.out

#***************************************************************************

player: player.c interface.o
	${CC} -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player unittest1 

clean:
	rm -f *.o playdom player unittest1 *.gcov *.gcda *.gcno *.so *.out *.exe
	rm -f testDiscardCard testDrawCard testInitGame unittest1 unittest2  
