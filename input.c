#include "input.h"
#include <stdlib.h>
#include <string.h>

char* read_file_to_string(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    fread(buffer, 1, size, fp);
    buffer[size] = '\0';

    fclose(fp);
    return buffer;
}

char* read_line(FILE* fp) {
    char* line = NULL;
    unsigned long len = 0;
    long nread = getline(&line, &len, fp);

    if (nread == -1) {
        free(line);
        return NULL;
    }

    return line;  // caller must free this
}