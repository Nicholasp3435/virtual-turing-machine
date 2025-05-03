#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

typedef enum { LEFT, RIGHT } Direction;

typedef struct {
    char* current_state;
    char read_symbol;
    char* next_state;
    char write_symbol;
    Direction move;
} Transition;

typedef struct {
    char* name;
    char* description;

    char** states;
    unsigned long num_states;

    char** input_symbols;
    unsigned long num_input_symbols;

    char** tape_symbols;
    unsigned long num_tape_symbols;

    char* start_state;
    char* blank_symbol;

    char** accept_states;
    unsigned long num_accept_states;

    Transition* transitions;
    unsigned long num_transitions;
} TuringMachine;


char* read_line(FILE* fp);

TuringMachine* parse_input(FILE* fp);

#endif