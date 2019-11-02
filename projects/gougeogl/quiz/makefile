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
CFLAGS += -Wall
CFLAGS += -pedantic
CFLAGS += -coverage
CFLAGS += -fpic
CFLAGS += -fprofile-arcs
CFLAGS += -ftest-coverage
CFLAGS += -lgcov

GCOV = gcov
GCOV += -f 
GCOV += -b

SRC = testme.c

testme.o: ${SRC} 
	${CC} -c ${SRC} -g ${CFLAGS} 

testme: ${SRC} testme.o
	${CC} -o testme ${SRC} -g ${CFLAGS} 

results: testme
	./testme 
	${GCOV} ${SRC} > testmeresults.out
	cat testme.c.gcov >> testmeresults.out

clean: 
	rm -rf *.o *.gcov *.gcda *.gcno *.out .so testme testme.exe 

# NOTE: .exe & .so extensions are Windows Executable and 
# 	Windows DLL Library file related 
