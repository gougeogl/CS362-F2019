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
CFLAGS += -fprofile-arcs # needed for .gcda files related to gcov 

# gcov call and flags
GCOV = gcov # coverage tool 
GCOV += -b # branch info
GCOV += -f # function info

rngs.o: rngs.h rngs.c
	${CC} -c rngs.c -g  $(CFLAGS) 

dominion.o: dominion.h dominion.c rngs.o
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

unittest1: unittest1.c dominion.o rngs.o 
	${CC} -o unittest1 unittest1.c -g dominion.o rngs.o $(CFLAGS)

unittestresults: unittest1 
	./unittest1 &> unittestresults.out 
	${GCOV} unittest1.c >> unittestresults.out
	cat unittest1.c.gcov >> unittestresults.out

player: player.c interface.o
	${CC} -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player unittest1 

clean:
	rm -f *.o playdom.exe playdom player player.exe unittest1 unittest1.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
