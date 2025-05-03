// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

int main(int argc, char** argv) {

    FILE* fp = fopen(argv[1], "r");

    TuringMachine* TM = parse_input(fp);

    

    return 0;
}

