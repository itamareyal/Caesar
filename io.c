/*
io.c
----------------------------------------------------------------------------
Handeling input & output files, any function that get a ptr to a file.
*/

#include <Windows.h>
#include <stdio.h>
#include "HardCodedData.h"


FILE* open_file(FILE* file, char* file_name, char* file_open_type)
{
    errno_t retval;
    // Open file
    retval = fopen_s(&file, file_name, file_open_type);
    if (0 != retval)
    {
        printf("Failed to open file: %s\n", file_name);
        return ERROR_CODE_FILE;
    }
    return file;
}


int close_file(FILE* file, char* file_name)
{
    errno_t retval;
    // Close file
    retval = fclose(file);
    if (0 != retval)
    {
        printf("Failed to close file: %s\n", file_name);
        return ERROR_CODE_FILE;
    }
    return SUCCESS_CODE;
}

int* count_row_endings(FILE* file) {
    // return arrayof ints. every index hold the num of chars in that line including \n.
    int index = 0, capacity = 1, char_counting = 0;
    int* row_ending_array;
    char c;


    if (NULL == (row_ending_array = (int*)malloc(capacity * sizeof(int))))
        return ERROR_CODE_ALLOCATION;

    for (c = fgetc(file); c != EOF; c = fgetc(file))
    {
        char_counting++;

        if (index == capacity - 1)
        {
            if (NULL == (row_ending_array = (int*)realloc(row_ending_array, (capacity++) * sizeof(int))))
                return ERROR_CODE_ALLOCATION;
        }

        if (c == '\n') {
            row_ending_array[index] = char_counting;
            index++;
        }
    }

    row_ending_array[index] = '\0';
    index = 0;
    /*while (char_counting != '\0') {
        char_counting = row_ending_array[index];
        printf("%d, ", row_ending_array[index]);
        index++;
    }*/
    return row_ending_array;
}


char* read_line(FILE* file, int* loop)
{
    int index = 0, capacity = INITIAL_SIZE;
    char* buffer;
    char c;

    if (NULL == (buffer = (char*)malloc(capacity)))
        return ERROR_CODE_ALLOCATION;

    for (c = fgetc(file); c != '\n'; c = fgetc(file))
    {
        if (index == capacity - 1)
        {
            if (NULL == (buffer = (char*)realloc(buffer, capacity * 2)))
                return ERROR_CODE_ALLOCATION;
            capacity *= 2;
        }

        if (c == EOF) {
            //close_file(file);
            *loop = 0;
            break;
        }
        buffer[index++] = c;
    }

    buffer[index++] = '\n';
    buffer[index++] = '\0';
    //close_file(file);
    return buffer;

}


int write_new_line_to_output(FILE* output_file, char* line) {
    fprintf(output_file, "%s", line);
    return 0;
}