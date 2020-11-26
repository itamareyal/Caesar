
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
    FILE* in_file; 
    FILE* out_file;
} THREADDATA, * PTHREADDATA;


void dispatch_threads(FILE* in_file, FILE* out_file, int key, int threads_required, int* rows_endings) {
    int rows_remaining = (int)(sizeof(rows_endings) / sizeof(rows_endings[0]));
    int i = 0, threads = threads_required, thread_start, thread_end;

    PTHREADDATA pData;
    DWORD   dwThreadId;
    HANDLE  hThread;

    while (threads) {

        pData = (PTHREADDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
            sizeof(THREADDATA));

        if (pData == NULL)
        {
            return ERROR_CODE_FILE;
        }

        int thread_rows = ceil(rows_remaining / threads);
        if (i == 0) thread_start = rows_endings[i];
        else
        {
            thread_start = rows_endings[i] + 1;
        }

        i += thread_rows;
        thread_end = rows_endings[i];

        pData->key = key;
        pData->start = thread_start;
        pData->end = thread_end;
        pData->in_file = in_file;
        pData->out_file = out_file;

        hThread = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            ThreadMain,             // thread function name
            pData,                  // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadId);           // returns the thread identifier 


        //create th

        rows_remaining -= thread_rows;
        threads--;
    }
}