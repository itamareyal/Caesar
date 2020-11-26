#pragma once

/*
io.h
----------------------------------------------------------------------------
Handeling input & output files, any function that get a ptr to a file.
Header for io.c
*/


#include "HardCodedData.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

FILE* open_file(FILE* file, char* file_name, char* file_open_type);
int close_file(FILE* file, char* file_name);
int* count_row_endings(FILE* file);
char* read_line(FILE* file, int* loop);
int write_new_line_to_output(FILE* output_file, char* line);