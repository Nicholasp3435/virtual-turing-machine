// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

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

