CC=gcc
CFLAGS= -g -Wall

EXE = DFA

OBJ0 = DFA.o

all: $(EXE) clear

DFA: $(OBJ0)
	$(CC) $(CFLAGS) -o DFA $(OBJ0)

DFA.o: DFA.c DFA.h
	$(CC) $(CFLAGS) -c DFA.c

clean:
	rm -rf $(OBJ0) *~ $(EXE)

clear:
	rm -rf $(OBJ0)
