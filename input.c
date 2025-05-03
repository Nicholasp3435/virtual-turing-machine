#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define verbose true

void print_str_arr(char** strings, unsigned long num_strings) {
    printf("{");
    for (unsigned long i = 0; i < num_strings; i++) {
        printf("%s, ", strings[i]);
    }
    printf("\b\b}\n");
}

void print_transitions(Transition* transitions, unsigned long count) {
    for (unsigned long i = 0; i < count; i++) {
        const Transition* t = &transitions[i];

        const char* dir = (t->move == LEFT) ? "L" :
                          (t->move == RIGHT) ? "R" : "S";

        printf("d(%s, %c) -> (%s, %c, %s)\n",
               t->current_state,
               t->read_symbol,
               t->next_state,
               t->write_symbol,
               dir);
    }
}


char* read_line(FILE* fp) {
    char* line = NULL;
    unsigned long len = 0;
    long nread = getline(&line, &len, fp);

    if (nread == -1) {
        free(line);
        return NULL;
    }

    return line;
}

char* clean_line(char* line) {
    unsigned long len = strlen(line);
    char* result = malloc(len + 1);
    if (!result) return NULL;

    bool in_quotes = false;
    bool in_comment = false;

    // i is the line index, j is the result index
    unsigned long i = 0, j = 0;

    while (i < len) {
        if (!in_quotes && !in_comment && line[i] == '/' && line[i + 1] == '*') {
            in_comment = true;
            i += 2;
            continue;
        }

        if (in_comment && line[i] == '*' && line[i + 1] == '/') {
            in_comment = false;
            i += 2;
            continue;
        }

        if (in_comment) {
            i++;
            continue;
        }

        if (line[i] == '"') {
            in_quotes = !in_quotes;
            result[j++] = line[i++];
            continue;
        }

        if (!in_quotes && (line[i] == ' ' || line[i] == '\t')) {
            i++;
            continue;
        }

        result[j++] = line[i++];
    }

    result[j] = '\0';
    return result;
}

char* extract(char* line, unsigned char preceding) {
    unsigned long data_len = strlen(line) - preceding;

    if (line[preceding + data_len - 1] == '\n' || line[preceding + data_len - 1] == ' ') {
        data_len--;
    }

    char* data = calloc(data_len + 1, sizeof(char)); 
    strncpy(data, line + preceding, data_len);
    data[data_len] = '\0'; 

    return data;
}

char** extract_set(const char* line, unsigned char preceding, unsigned long* count) {
    *count = 0;
    char** elements = NULL;

    const char* ptr = line + preceding;

    ptr++; // skip '{'

    // Buffers for building one string
    char* current = NULL;
    unsigned long current_len = 0;

    while (*ptr && *ptr != '}') {
        if (*ptr == ',') {
            if (current_len > 0) {
                current = realloc(current, current_len + 1);
                current[current_len] = '\0';

                elements = realloc(elements, (*count + 1) * sizeof(char*));
                elements[*count] = current;
                (*count)++;

                current = NULL;
                current_len = 0;
            }
            ptr++;
            continue;
        }

        current = realloc(current, current_len + 1);
        current[current_len++] = *ptr++;

    }

    if (current_len > 0) {
        current = realloc(current, current_len + 1);
        current[current_len] = '\0';

        elements = realloc(elements, (*count + 1) * sizeof(char*));
        elements[*count] = current;
        (*count)++;
    }

    elements = realloc(elements, (*count + 1) * sizeof(char*));
    elements[*count] = NULL;

    return elements;
}

void add_transition(TuringMachine* TM, const char* line, unsigned long* count) {
    Transition t = {0};

    const char* ptr = strchr(line, '(');
    if (!ptr) {
        return;
    }
    ptr++; // skip '('

    char* curr_state = NULL;
    unsigned long curr_len = 0;
    while (*ptr && *ptr != ',') {
        curr_state = realloc(curr_state, curr_len + 2);
        curr_state[curr_len++] = *ptr++;
    }
    curr_state[curr_len] = '\0';
    t.current_state = curr_state;

    if (*ptr == ',') {
        ptr++;
    }

    t.read_symbol = *ptr++;

    ptr = strchr(ptr, '(');
    if (!ptr) {
        return;
    }
    ptr++;

    char* next_state = NULL;
    unsigned long next_len = 0;
    while (*ptr && *ptr != ',') {
        next_state = realloc(next_state, next_len + 2);
        next_state[next_len++] = *ptr++;
    }
    next_state[next_len] = '\0';
    t.next_state = next_state;

    if (*ptr == ',') {
        ptr++;
    }

    t.write_symbol = *ptr++;

    if (*ptr == ',') {
        ptr++;
    }

    switch (*ptr) {
        case 'L': t.move = LEFT; break;
        case 'R': t.move = RIGHT; break;
        default: break;
    }

    TM->transitions = realloc(TM->transitions, sizeof(Transition) * (TM->num_transitions + 1));
    TM->transitions[TM->num_transitions++] = t;

    (*count)++;
}

TuringMachine* parse_input(FILE* fp) {

    TuringMachine* TM = calloc(1, sizeof(TuringMachine));

    unsigned long* num_transitions = calloc(1, sizeof(long));
    TM->num_transitions = *num_transitions;

    if (fp) {
        char* line;
        while ((line = read_line(fp)) != NULL) {

            char* parsed_line = clean_line(line);
            // printf("%s", parsed_line);

            switch (parsed_line[0]) {
                case 'N': {
                    TM->name = extract(parsed_line, 5);
                    break;
                } case 'D': {
                    TM->description = extract(parsed_line, 12);
                    break;
                } case 'Q': {
                    unsigned long* num_states = calloc(1, sizeof(long));
                    TM->states = extract_set(parsed_line, 2, num_states);
                    TM->num_states = *num_states;
                    break;
                } case 'S': {
                    unsigned long* num_input_symbols = calloc(1, sizeof(long));
                    TM->input_symbols = extract_set(parsed_line, 2, num_input_symbols);
                    TM->num_input_symbols = *num_input_symbols;
                } case 'G': {
                    unsigned long* num_tape_symbols = calloc(1, sizeof(long));
                    TM->tape_symbols = extract_set(parsed_line, 2, num_tape_symbols);
                    TM->num_tape_symbols = *num_tape_symbols;
                } case 'q': {
                    TM->start_state = extract(parsed_line, 3);
                } case 'F': {
                    unsigned long* num_accept_states = calloc(1, sizeof(long));
                    TM->accept_states = extract_set(parsed_line, 2, num_accept_states);
                    TM->num_accept_states = *num_accept_states;
                } case 'b': {
                    TM->blank_symbol = extract(parsed_line, 2);
                } case 'd': {
                    add_transition(TM, parsed_line, num_transitions);
                }
                default:
                    break;
            }

            free(parsed_line);
        }

        if (verbose) {
            printf("\n\n");

            printf("Loaded the Turing Machine, %s\n", TM->name);
    
            printf("Description: %s\n", TM->description);
    
            printf("There are %ld states: ", TM->num_states);
            print_str_arr(TM->states, TM->num_states);
    
            printf("There are %ld input symbols: ", TM->num_input_symbols);
            print_str_arr(TM->input_symbols, TM->num_input_symbols);
    
            printf("There are %ld tape symbols: ", TM->num_tape_symbols);
            print_str_arr(TM->tape_symbols, TM->num_tape_symbols);
    
            printf("The start state is: %s\n", TM->start_state);
    
            printf("There are %ld accept states: ", TM->num_accept_states);
            print_str_arr(TM->accept_states, TM->num_accept_states);
    
            printf("The blank symbol is: %s\n", TM->blank_symbol);
    
            printf("There are %ld transitions:\n", TM->num_transitions);
            print_transitions(TM->transitions, TM->num_transitions);
        }

        fclose(fp);
    }

    return TM;

}

