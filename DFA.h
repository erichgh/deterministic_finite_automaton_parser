#ifndef AF_H
#define AF_H

#include <stdio.h>
#include <stdlib.h>

#define MAXSTACK 100

typedef struct _Dfa Dfa;

typedef struct _State State;

typedef struct _Transition Transition;

typedef enum {
    START, FINAL, OTHER
} StateType;


Dfa * newDFA(State** states, int nStates);

void addTransition(State* origin, char symbol, State* dest);

State * newState(char* name, StateType type);

void printDFA(Dfa* dfa);

void printState(State *state);

void printTransition(Transition *trans) ;

int runDFA(Dfa* dfa, char* str);

void dfaFree(Dfa* dfa);

void stateFree(State* state);


#endif
