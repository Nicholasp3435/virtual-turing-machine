// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

char* remove_white_space(char* line) {

    unsigned int line_len = strlen(line);

    char* parsed_line = malloc(line_len * sizeof(char));
    
    unsigned int parsed_index = 0;
    unsigned int line_index = 0;

    while(parsed_index < line_len) {
        if (line[line_index] != ' ') {
            parsed_line[parsed_index] = line[line_index];
            parsed_index++;
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

            char* parsed_line = remove_white_space(line);
            free(line); // Don't forget to free after use!
        }
        printf("\n");
        fclose(fp);
    }
    

    return 0;
}

