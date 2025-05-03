// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"

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

int main(int argc, char** argv) {

    FILE* fp = fopen(argv[1], "r");

    if (fp) {
        char* line;
        while ((line = read_line(fp)) != NULL) {

            char* no_spaces = remove_white_space(line);
            char* parsed_line = remove_comments(no_spaces);
            printf("%s", parsed_line);

            free(parsed_line);
        }
        printf("\n");
        fclose(fp);
    }
    

    return 0;
}

