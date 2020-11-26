#pragma once
/*
threadManager.h
----------------------------------------------------------------------------
Decryption or encryption of txt by key.
Header file for decrypte.c
*/
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "HardCodedData.h"
#include "io.h"
#include "decrypte.h"


typedef struct ThreadData {
    int key, start, end;
    char mode;
    FILE* in_file;
    FILE* out_file;
} THREADDATA, * PTHREADDATA;
int count_elements(int* array);

void wait_for_thread_execution_and_free(HANDLE hThread, PTHREADDATA pData);

void free_thread_and_data(HANDLE hthread, PTHREADDATA pdata);

DWORD WINAPI thread_main(LPVOID lpParam);

int dispatch_threads(FILE* in_file, FILE* out_file, int key, int threads_required, int* rows_endings, char mode);

