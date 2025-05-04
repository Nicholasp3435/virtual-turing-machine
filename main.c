// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

typedef struct {
    char* cells;
    unsigned long capacity;
    unsigned long head;
    char blank;
    char* cur_state;
} Tape;

Tape* init_tape(unsigned long initial_size, char blank_symbol, char* q0) {
    Tape* tape = calloc(1, sizeof(Tape));
    tape->cells = calloc(initial_size, sizeof(char));
    tape->cur_state = q0;

    for (unsigned long i = 0; i < initial_size; i++)
        tape->cells[i] = blank_symbol;

    tape->capacity = initial_size;
    tape->head = 0;
    tape->blank = blank_symbol;
    return tape;
}

void expand_tape(Tape* tape) {
    unsigned long new_capacity = tape->capacity * 2;
    char* new_cells = calloc(new_capacity, sizeof(char));

    // Copy old values
    for (unsigned long i = 0; i < tape->capacity; i++) {
        new_cells[i] = tape->cells[i];
    }

    // Fill rest with blanks
    for (unsigned long i = tape->capacity; i < new_capacity; i++) {
        new_cells[i] = tape->blank;
    }

    free(tape->cells);
    tape->cells = new_cells;
    tape->capacity = new_capacity;
}

char read_symbol(Tape* tape) {
    if (tape->head >= tape->capacity) {
        expand_tape(tape);
    }

    return tape->cells[tape->head];
}

void write_symbol(Tape* tape, char symbol) {
    if (tape->head >= tape->capacity) {
        expand_tape(tape);
    }
    tape->cells[tape->head] = symbol;
}

void move_right(Tape* tape) {
    tape->head++;
    if (tape->head >= tape->capacity) {
        expand_tape(tape);
    }
}

void move_left(Tape* tape) {
    if (tape->head > 0) {
        tape->head--;
    }
}

void load_input(Tape* tape, const char* input) {
    unsigned long len = strlen(input);
    if (len > tape->capacity) {
        while (len > tape->capacity)
            expand_tape(tape);
    }
    for (unsigned long i = 0; i < len; i++) {
        tape->cells[i] = input[i];
    }
}

void print_tape(Tape* tape) {
    for (unsigned long i = 0; i < tape->capacity; i++) {
        if (i == tape->head)
            printf("[%c]", tape->cells[i]);
        else
            printf(" %c ", tape->cells[i]);
    }
    printf("\nState: %s\n", tape->cur_state);
}

void run_turing_machine(TuringMachine* TM, Tape* tape) {
    while (1) {
        char sym = read_symbol(tape);
        int matched = 0;

        for (int i = 0; i < TM->num_transitions; i++) {
            Transition t = TM->transitions[i];

            if (strcmp(t.current_state, tape->cur_state) == 0 && t.read_symbol == sym) {
                // Apply transition
                write_symbol(tape, t.write_symbol);
                tape->cur_state = t.next_state; // or strdup(...) if needed
                if (t.move == LEFT) move_left(tape);
                else move_right(tape);

                matched = 1;
                break;
            }
        }

        print_tape(tape);

        if (!matched) {
            printf("No matching transition. Halting.\n");
            break;
        }

        // Check if in accept state
        for (int j = 0; j < TM->num_accept_states; j++) {
            if (strcmp(tape->cur_state, TM->accept_states[j]) == 0) {
                printf("Machine accepted input.\n");
                return;
            }
        }
    }

    printf("Machine rejected input.\n");
}

int main(int argc, char** argv) {

    TuringMachine* TM = parse_input(fopen(argv[1], "r"));
    Tape* tape = init_tape(16, *TM->blank_symbol, TM->start_state);
    load_input(tape, argv[2]);
    print_tape(tape);

    run_turing_machine(TM, tape);


    return EXIT_SUCCESS;
}

