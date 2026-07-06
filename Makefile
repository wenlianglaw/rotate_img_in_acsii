
CFLAGS= -std=c++20 -Wall
CC=clang++

PROGRAM=main

SRC_FILES= $(wildcard *.cpp)

OBJ := $(patsubst %.cpp, %.o, $(SRC_FILES))


main:	$(OBJ)
	$(CC) $(OBJ) -o $(PROGRAM) $(CFLAGS)


%.o:	%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)
	

.PHONY: main


# Or use -B
force:
	touch main.cpp
	$(MAKE)
