#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

char* remove_white_space(char* line) {

    unsigned int line_len = strlen(line);

    char* parsed_line = calloc(line_len, sizeof(char));
    
    unsigned int parsed_index = 0;
    unsigned int line_index = 0;
    bool is_in_quotes = false;

    while(parsed_index < line_len) {
        if (line[line_index] == '"') {
            is_in_quotes = true;
            line_index++;
        } 

        if (line[line_index] != ' ' || is_in_quotes) {
            parsed_line[parsed_index] = line[line_index];
            parsed_index++;
        }

        if (line[line_index] == '"') {
            is_in_quotes = false;
            line_index++;
        } 

        line_index++;
    }

    free(line);

    return parsed_line;
}

char* remove_comments(char* line) {

    unsigned int line_len = strlen(line);

    char* parsed_line = calloc(line_len, sizeof(char));
    
    unsigned int parsed_index = 0;
    unsigned int line_index = 0;
    bool is_in_comment = false; 

    while(parsed_index < line_len) {
        if (line[line_index] == '/' && line[line_index + 1] == '*') {
            is_in_comment = true;
        } 

        if (!is_in_comment) {
            parsed_line[parsed_index] = line[line_index];
            parsed_index++;
        }

        if (line[line_index + 1] == '/' && line[line_index] == '*') {
            is_in_comment = false;
            line_index++;
        }

        line_index++;
    }

    free(line);

    return parsed_line;
}