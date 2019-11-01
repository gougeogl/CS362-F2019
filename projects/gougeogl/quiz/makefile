# ***********************************************************
# FileName:	makefile 
# Author: 	Glen Gougeon
# Date:		11-1-2019
# Due:		??
#
# Description:	makefile to compile testme.c.
# 		Purpose is for Quiz 1, Random Testing Quiz 	
#
# ***********************************************************

CC = gcc
CFLAGS = -Wall
CFLAGS += -pedantic
#GCOV = gcov

SRC = testme.c

testme.o: ${SRC} 
	${CC} ${CFLAGS} -c ${SRC}

compile: testme.o
	${CC} ${CFLAGS} testme.o -o testme

run: compile 
	testme 

clean: 
	rm -rf *.o testme 
