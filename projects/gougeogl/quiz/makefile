# ***********************************************************
# FileName:	makefile 
# Author: 	Glen Gougeon
# Date:		11-4-2019
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

GCOV = gcov
GCOV += -f 
GCOV += -b

SRC = testme.c
OBJ = testme.o
GCDA_FILE = testme.gcda

${OBJ}: ${SRC} 
	${CC} -c ${SRC} -g ${CFLAGS} 

run: ${OBJ}   
	${CC} -o testme ${OBJ} -g ${CFLAGS} 
	./testme

results: 
	${GCOV} ${SRC} &> testmeresults.out
	cat testme.c.gcov >> testmeresults.out
	cat testmeresults.out

clean: 
	rm -rf *.o *.gcov *.gcda *.gcno *.out .so testme testme.exe 

# NOTE: .exe & .so extensions are Windows Executable and 
# 	Windows DLL Library file related 
