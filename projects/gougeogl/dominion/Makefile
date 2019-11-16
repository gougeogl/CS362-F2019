# ********************************************************
# FileName: 	Makefile
# Edited by: 	Glen Gougeon
# Date: 	11-14-2019
#
# Description: 	Makefile for Assignment 4 random testing
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

dominion.o: dominion.c dominion.h dominion_helpers.h rngs.o
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

randomtest1: randomtest1.c dominion.o rngs.o interface.o 
	${CC} -o randomtest1 randomtest1.c -g dominion.o rngs.o interface.o $(CFLAGS)

randomtest2: randomtest2.c dominion.o rngs.o interface.o 
	${CC} -o randomtest2 randomtest2.c -g dominion.o rngs.o interface.o $(CFLAGS)

unittest4: unittest4.c dominion.o dominion_helpers.h rngs.o interface.o
	${CC} -o unittest4 unittest4.c -g dominion.o rngs.o interface.o $(CFLAGS)

unittest4results: unittest4
	./unittest4 &> unittest4results.out
	${GCOV} dominion.c >> unittest4results.out
	${GCOV} unittest4.c >> unittest4results.out
	cat unittest4.c.gcov >> unittest4results.out
	cat dominion.c.gcov >> unittest4results.out
#***********************************************************************

randomtestresults: randomtest1 randomtest2
	./randomtest1 &> randomtestresults.out 
	./randomtest2 >> randomtestresults.out 
	${GCOV} dominion.c >> randomtestresults.out
	${GCOV} randomtest1.c >> randomtestresults.out
	${GCOV} randomtest2.c >> randomtestresults.out
	cat randomtest1.c.gcov >> randomtestresults.out
	cat randomtest2.c.gcov >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out

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

player: player.c interface.o
	${CC} -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player unittest1 

clean:
	rm -f *.o playdom player *.gcov *.gcda *.gcno *.so *.out *.exe
	rm -f testDiscardCard testDrawCard randomtest1 randomtest2 randomtest3 randomtest4 randomtest5 
	rm -f unittest4
