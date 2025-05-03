// main.c
#include <stdio.h>
#include <stdlib.h>
#include "input.h"

int main(int argc, char** argv) {

    FILE* fp = fopen(argv[1], "r");

    if (fp) {
        char* line;
        while ((line = read_line(fp)) != NULL) {
            printf("%s", line);
            free(line); // Don't forget to free after use!
        }
        printf("\n");
        fclose(fp);
    }
    

    return 0;
}