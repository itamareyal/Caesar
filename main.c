/*
main.c
----------------------------------------------------------------------------
AUTHORS - Itamar Eyal 302309539 and Eran Weil 204051353

PROJECT - CAESAR

DESCRIPTION - This program can decrypte or encrypte a text using the Caesar system given a specific key.
                 
              In cryptography, a Caesar cipher, also known as Caesar's cipher, the shift cipher,
              Caesar's code or Caesar shift, is one of the simplest and most widely known encryption techniques.
              It is a type of substitution cipher in which each letter in the plaintext is replaced by a letter
              some fixed number of positions down the alphabet. For example, with a left shift of 3, D would be
              replaced by A, E would become B, and so on. The method is named after Julius Caesar, who used it in
              his private correspondence. (from wikipedia: https://en.wikipedia.org/wiki/Caesar_cipher).

    consists of 3 modules:
        io - handles files and cmd line args
        threadManager - creates and operates threads.
        decrypte - utilizies the Caesar coding system to encrypte or decrypte text.

*/

//-------------------------------------------------------------//
// ----------------------LIBRARY INCLUDES--------------------- //
//-------------------------------------------------------------//

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//-------------------------------------------------------------//
// ----------------------PROJECT INCLUDES--------------------- //
//-------------------------------------------------------------//

#include "HardCodedData.h"
#include "io.h"
#include "decrypte.h"
#include "threadManager.h"




int main(int argc, char* argv[]){

    // check that the arguments are suitable. if not, exit program.
    check_arguments(argc, argv);

    // Parse arguments for mode of operation, num of threades and key.
    char mode = '\0';
    long key, threads_required;
    parse_arguments(&mode, &threads_required, &key, argv);
    
    int* row_ending = NULL;
    
    // File parameters
    FILE* p_input_file = NULL;
    char* in_file_name = argv[1];
    char* in_file_open_type = "r";

    FILE* p_output_file = NULL;
    char* out_file_name = "decrypted.txt";
    if (mode == 'e') out_file_name = "encrypted.txt";
    char* out_file_open_type = "w";

    // Open file with check
    if (NULL == (p_input_file = open_file(p_input_file, in_file_name, in_file_open_type))) error_handler(ERROR_CODE_FILE);

    // Open file with check
    if (NULL == (p_output_file = open_file(p_output_file, out_file_name, out_file_open_type))) error_handler(ERROR_CODE_FILE);

    // Array holding locations of all line ending in the input file.
    row_ending = count_row_endings(p_input_file);

    // thread manager function
    dispatch_threads(p_input_file, p_output_file, key, threads_required, row_ending, mode);

    // Close file with check
    if (close_file(p_input_file, in_file_name) == STATUS_CODE_FAILURE) error_handler(ERROR_CODE_FILE);

    // Close file with check
    if (close_file(p_output_file, out_file_name) == STATUS_CODE_FAILURE) error_handler(ERROR_CODE_FILE);

    print_exit_msg(in_file_name, out_file_name);

    return SUCCESS_CODE;
}