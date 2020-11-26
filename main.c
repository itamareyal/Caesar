/*
main.c
----------------------------------------------------------------------------
AUTHORS - Itamar Eyal 302309539 and Eran Weil 204051353

PROJECT - CAESAR

DESCRIPTION - this program can decrypte or encrypte a text using the Caesar system given a specific key.

*/


#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "HardCodedData.h"
#include "io.h"
#include "decrypte.h"



void error_handler(int err_code) {
    printf("\n----------ERROR----------\n");
    switch (err_code) {
    case -1: {
        printf("\tFailiure.");
        break;
    }
    case -2: {
        printf("\tError with file handeling.");
        break;
    }
    case -3: {
        printf("\tError writing to the output file.");
        break;
    }
    case -4: {
        printf("Incorrect number of arguments. %d requird.", ARGS_REQUIRED - 1);
        break;
    }
    default: printf("\tFailiure.");
    }
}



int check_arguments(int argc, char* argv[]) {
    
    if (argc != ARGS_REQUIRED) {
        printf("Incorrect number of arguments. %d requird.", ARGS_REQUIRED - 1);
        printf("\t%d given.", argc - 1);
        error_handler(ERROR_CODE_ARGS);
    }
    char* p;
    errno = 0;
    long key = strtol(argv[2], &p, 10);

    if (errno != 0 || *p != '\0' || key < 0) {
        error_handler( ERROR_CODE_ARGS);
    }
    else {
        return 1;
    }
}



int main(int argc, char* argv[]){

    // check num of argc and that argv[2] is a number
    check_arguments(argc, argv);

    long key = atoi(argv[2]);

    int loop;
    char* line = NULL;
    int* row_ending = NULL;
    
    FILE* p_input_file = NULL;
    char* in_file_name = argv[1];
    char* in_file_open_type = "r";

    FILE* p_output_file = NULL;
    char* out_file_name = "decryped.txt";
    char* out_file_open_type = "w";

    // Open file with check
    if (NULL == (p_input_file = open_file(p_input_file, in_file_name, in_file_open_type))) return ERROR_CODE_FILE;

    // Open file with check
    if (NULL == (p_output_file = open_file(p_output_file, out_file_name, out_file_open_type))) return ERROR_CODE_FILE;

    row_ending = count_row_endings(p_input_file);

    loop = 1;
    while (loop) {
        line = read_line(p_input_file, &loop);
        line = decrypte_line(line, key);
        if (0 != write_new_line_to_output(p_output_file, line)) error_handler(ERROR_CODE_WRITE);
    }

    // Close file with check
    if (close_file(p_input_file, in_file_name) == STATUS_CODE_FAILURE) error_handler(ERROR_CODE_FILE);

    // Close file with check
    if (close_file(p_output_file, out_file_name) == STATUS_CODE_FAILURE) error_handler(ERROR_CODE_FILE);

    return 0;
}