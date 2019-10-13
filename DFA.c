#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"

struct _Dfa
{
    State **states;
    int nStates;
};

struct _State
{
    char *name;
    StateType type;
    Transition **transitions;
    int nTrans;
};

struct _Transition
{
    char symbol;
    State *dest;
};

Dfa *newDFA(State **states, int nStates)
{
    if (states == NULL)
    {
        return NULL;
    }

    Dfa *new = (Dfa *)malloc(sizeof(Dfa));
    if (new == NULL)
    {
        perror("Error! memory not allocated.\n");
        return NULL;
    }

    new->nStates = nStates;

    new->states = (State **)malloc(nStates * sizeof(State *));
    for (int i = 0; i < nStates; i++)
    {
        new->states[i] = states[i];
    }
    return new;
}

void addTransition(State *origin, char symbol, State *dest)
{
    if (origin == NULL || dest == NULL)
    {
        return;
    }

    Transition *new = (Transition *)malloc(sizeof(Transition));
    if (new == NULL)
    {
        perror("Error! memory not allocated.\n");
        return;
    }

    new->symbol = symbol;
    new->dest = dest;
    origin->nTrans++;
    origin->transitions = (Transition **)realloc(origin->transitions, origin->nTrans * sizeof(Transition *));

    origin->transitions[origin->nTrans - 1] = new;
}

State *newState(char *name, StateType type)
{
    if (name == NULL)
    {
        return NULL;
    }

    State *new = (State *)malloc(sizeof(State));
    if (new == NULL)
    {
        perror("Error! memory not allocated.\n");
        return NULL;
    }

    new->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    if (new->name == NULL)
    {
        free(new);
        perror("Error! memory not allocated.\n");
        return NULL;
    }

    strcpy(new->name, name);

    new->transitions = (Transition **)malloc(sizeof(Transition *));
    if (new->name == NULL)
    {
        free(new->name);
        free(new);
        perror("Error! memory not allocated.\n");
        return NULL;
    }

    new->type = type;
    new->nTrans = 0;

    return new;
}

void printDFA(Dfa *dfa)
{
    if (dfa == NULL)
    {
        return;
    }

    for (int i = 0; i < dfa->nStates; i++)
    {
        printState(dfa->states[i]);
    }
}

void printState(State *state)
{
    if (state == NULL)
    {
        return;
    }
    printf("Transitions from %s (type %d):\n", state->name, state->type);
    for (int i = 0; i < state->nTrans; i++)
    {
        printTransition(state->transitions[i]);
    }
}

void printTransition(Transition *trans)
{
    if (trans == NULL)
    {
        return;
    }
    printf("%c -> %s\n", trans->symbol, trans->dest->name);
}

int runDFA(Dfa *dfa, char *str)
{
    if (dfa == NULL || str == NULL)
    {
        return 0;
    }
    State *current = NULL;
    int k = 0;
    int i = 0;

    for (i = 0; i < dfa->nStates; i++)
    {
        if (dfa->states[i]->type == START)
        {
            current = dfa->states[i];
            break;
        }
    }

    for (i = 0; i < strlen(str); i++)
    {
        for (k = 0; k < current->nTrans; k++)
        {
            if (current->transitions[k]->symbol == str[i])
            {
                current = current->transitions[k]->dest;
                break;
            }
        }
    }
    return current->type == FINAL;
}

void dfaFree(Dfa *dfa)
{
    if (dfa == NULL)
    {
        return;
    }
    for (int i = 0; i < dfa->nStates; i++)
    {
        stateFree(dfa->states[i]);
    }
    free(dfa->states);
    free(dfa);
}

void stateFree(State *state)
{
    if (state == NULL)
    {
        return;
    }
    free(state->name);
    for (int i = 0; i < state->nTrans; i++)
    {
        free(state->transitions[i]);
    }
    free(state->transitions);
    free(state);
}

int main(void)
{
    State **states = NULL;
    Dfa *dfa = NULL;

    states = (State **)malloc(4 * sizeof(State *));

    states[0] = newState("q0", START);
    states[1] = newState("q1", FINAL);
    states[2] = newState("q2", FINAL);
    states[3] = newState("q3", OTHER);

    dfa = newDFA(states, 4);

    addTransition(states[0], 'a', states[3]);
    addTransition(states[0], 'b', states[1]);

    addTransition(states[1], 'a', states[2]);
    addTransition(states[1], 'b', states[1]);

    addTransition(states[2], 'a', states[2]);
    addTransition(states[2], 'b', states[1]);

    printDFA(dfa);

    if (runDFA(dfa, "baaba"))
    {
        printf("OK\n");
    }
    else
    {
        printf("FAILED\n");
    }

    dfaFree(dfa);
    free(states);
}