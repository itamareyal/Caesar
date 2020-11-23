#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STATUS_CODE_FAILURE -2 
#define SUCCESS_CODE 0 
#define INITIAL_SIZE 10
#define ERROR_CODE_FILE -3


FILE* open_file(FILE* file, char* file_name, char* file_open_type)
{
    errno_t retval;
    // Open file
    retval = fopen_s(&file, file_name, file_open_type);
    if (0 != retval)
    {
        printf("Failed to open file: %s\n", file_name);
        return file;
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
        return STATUS_CODE_FAILURE;
    }
    return SUCCESS_CODE;
}


char* read_line(FILE* file, int* loop)
{
    int index = 0, capacity = INITIAL_SIZE;
    char* buffer;
    char c;

    if (NULL == (buffer = (char*)malloc(capacity)))
        return NULL;

    for (c = fgetc(file); c != '\n'; c = fgetc(file))
    {
        if (index == capacity - 1)
        {
            if (NULL == (buffer = (char*)realloc(buffer, capacity * 2)))
                return NULL;
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

char* decrypte_line(char* line, int key) {

    int i;
    char ch;
    size_t len = strlen(line);


    for (i = 0; i < len-1; i++) {
        ch = *(line + i);
        if (isdigit(ch)) {
            ch = '0' + (ch - '0' - key) % 10;
        }
        if (isalpha(ch)) {
            if (islower(ch)) {
                if ((ch - 'a' - key) < 0) ch += 26;
                ch = 'a' + (ch - 'a' - key) % 26;
            }
            else {
                if ((ch - 'A' - key) < 0) ch += 26;
                ch = 'A' + (ch - 'A' - key) % 26;
            }
        }
        *(line + i) = ch;
    }

    return line;
}


int write_new_line_to_output(FILE* output_file, char* line) {
    fprintf(output_file, "%s", line);
    return 0;
}


int main(int argc, char* argv[]){

    // check num of argc

    long key = atoi(argv[2]);
    // check that argv2 is int
    int loop;
    char* line = NULL;
    
    FILE* p_input_file = NULL;
    char* in_file_name = argv[1];
    char* in_file_open_type = "r";

    FILE* p_output_file = NULL;
    char* out_file_name = "output.txt";
    char* out_file_open_type = "w";

    // Open file with check
    if (NULL == (p_input_file = open_file(p_input_file, in_file_name, in_file_open_type))) return ERROR_CODE_FILE;

    // Open file with check
    if (NULL == (p_output_file = open_file(p_output_file, out_file_name, out_file_open_type))) return ERROR_CODE_FILE;
    loop = 1;
    while (loop) {
        line = read_line(p_input_file, &loop);
        line = decrypte_line(line, key);
        if (0 != write_new_line_to_output(p_output_file, line)) return STATUS_CODE_FAILURE;
    }

    // Close file with check
    if (close_file(p_input_file, in_file_name) == STATUS_CODE_FAILURE) return ERROR_CODE_FILE;

    // Close file with check
    if (close_file(p_output_file, out_file_name) == STATUS_CODE_FAILURE) return ERROR_CODE_FILE;

    return 0;
}