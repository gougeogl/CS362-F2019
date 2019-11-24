# ********************************************************
# FileName: 	Makefile
# Edited by: 	Glen Gougeon
# Date: 	1-23-2019
#
# Description: 	Makefile for Assignment 5 
#				testing our tests
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

unittest3: unittest3.c dominion.o rngs.o interface.o 
	${CC} -o unittest3 unittest3.c -g dominion.o rngs.o interface.o $(CFLAGS)
	
unittest4: unittest4.c dominion.o rngs.o interface.o 
	${CC} -o unittest4 unittest4.c -g dominion.o rngs.o interface.o $(CFLAGS)
	
unittest5: unittest5.c dominion.o rngs.o interface.o 
	${CC} -o unittest5 unittest5.c -g dominion.o rngs.o interface.o $(CFLAGS)

#***********************************************************************

unittest1results: unittest1 
	./unittest1 &> unittest1results.out 
	${GCOV} dominion.c >> unittest1results.out
	${GCOV} unittest1.c >> unittest1results.out
	cat unittest1.c.gcov >> unittest1results.out
	cat dominion.c.gcov >> unittest1results.out

#***********************************************************************

unittest2results: unittest2 
	./unittest2 &> unittest2results.out 
	${GCOV} dominion.c >> unittest2results.out
	${GCOV} unittest2.c >> unittest2results.out
	cat unittest2.c.gcov >> unittest2results.out
	cat dominion.c.gcov >> unittest2results.out

#***********************************************************************

unittest3results: unittest3 
	./unittest3 &> unittest3results.out 
	${GCOV} dominion.c >> unittest3results.out
	${GCOV} unittest3.c >> unittest3results.out
	cat unittest3.c.gcov >> unittest3results.out
	cat dominion.c.gcov >> unittest3results.out

#***********************************************************************

unittest4results: unittest4 
	./unittest4 &> unittest4results.out 
	${GCOV} dominion.c >> unittest4results.out
	${GCOV} unittest4.c >> unittest4results.out
	cat unittest4.c.gcov >> unittest4results.out
	cat dominion.c.gcov >> unittest4results.out

#***********************************************************************

unittest5results: unittest5 
	./unittest5 &> unittest5results.out 
	${GCOV} dominion.c >> unittest5results.out
	${GCOV} unittest5.c >> unittest5results.out
	cat unittest5.c.gcov >> unittest5results.out
	cat dominion.c.gcov >> unittest5results.out

#***********************************************************************

randomtest1: randomtest1.c dominion.o rngs.o interface.o 
	${CC} -o randomtest1 randomtest1.c -g dominion.o rngs.o interface.o $(CFLAGS)

randomtest2: randomtest2.c dominion.o rngs.o interface.o 
	${CC} -o randomtest2 randomtest2.c -g dominion.o rngs.o interface.o $(CFLAGS)

randomtest3: randomtest3.c dominion.o rngs.o interface.o 
	${CC} -o randomtest3 randomtest3.c -g dominion.o rngs.o interface.o $(CFLAGS)

#***********************************************************************

randomtest1results: randomtest1 
	./randomtest1 &> randomtest1results.out 
	${GCOV} dominion.c >> randomtest1results.out
	${GCOV} randomtest1.c >> randomtest1results.out
	cat randomtest1.c.gcov >> randomtest1results.out
	cat dominion.c.gcov >> randomtest1results.out

#***********************************************************************

randomtest2results: randomtest2 
	./randomtest2 &> randomtest2results.out 
	${GCOV} dominion.c >> randomtest2results.out
	${GCOV} randomtest2.c >> randomtest2results.out
	cat randomtest2.c.gcov >> randomtest2results.out
	cat dominion.c.gcov >> randomtest2results.out

#***********************************************************************

#randomtest3results: randomtest3 
#	./randomtest3 &> randomtest3results.out 
#	${GCOV} dominion.c >> randomtest3results.out
#	${GCOV} randomtest3.c >> randomtest3results.out
#	cat randomtest3.c.gcov >> randomtest3results.out
#	cat dominion.c.gcov >> randomtest3results.out

#***********************************************************************

allunittestresults: unittest1results unittest2results unittest3results unittest4results unittest5results
	unittest1results unittest2results unittest3results unittest4results unittest5results

allrandomtestresults: randomtest1results randomtest2results
	randomtest1results randomtest2results
	
allresults: allunittestresults allrandomtestresults
	allunittestresults allrandomtestresults
	
#***********************************************************************

player: player.c interface.o
	${CC} -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o  *.gcov *.gcda *.gcno *.so *.out *.exe
	rm -f playdom player testDiscardCard testDrawCard testInitGame 
	rm -f unittest1 unittest2 unittest3 unittest4 unittest5
	rm -f randomtest1 randomtest2 randomtest3
