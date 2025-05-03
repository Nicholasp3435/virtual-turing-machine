#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

char* read_line(FILE* fp);

char* remove_comments(char* line);

char* remove_white_space(char* line);

#endif