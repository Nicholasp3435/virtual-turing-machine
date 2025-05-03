#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

// Reads the entire file into a dynamically allocated string (null-terminated)
// Returns NULL on error. Caller must free the result.
char* read_file_to_string(const char* filename);

// Reads a line from the file and stores it in buffer (up to buffer_size - 1 chars)
// Returns 1 on success, 0 on EOF, and -1 on error.
char* read_line(FILE* fp);

#endif