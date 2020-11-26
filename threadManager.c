
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

void free_thread_and_data(HANDLE hthread, PTHREADDATA pdata) {
    // closing handle and free pData struct
    CloseHandle(hthread);
    if (pdata != NULL)
    {
        HeapFree(GetProcessHeap(), 0, pdata);
        pdata = NULL;    // Ensure address is not reused.
    }
}

void wait_for_thread_execution_and_free(HANDLE hThread, PTHREADDATA pData) {
    DWORD dwWaitResult = WaitForSingleObject(
        hThread,    // handle to thread
        THREAD_WAIT_TIME);  // no time-out interval

    switch (dwWaitResult)
    {
    case WAIT_OBJECT_0: // thread finished it's work
        free_thread_and_data(hThread, pData);
        break;

    case WAIT_TIMEOUT: // timeout reached before the thread was done
        free_thread_and_data(hThread, pData);
        return ERROR_CODE_WSO_WAIT_TIMEOUT;

    case WAIT_FAILED:
        free_thread_and_data(hThread, pData);
        return ERROR_CODE_WSO_WAIT_FAILED;

    case WAIT_ABANDONED:
        free_thread_and_data(hThread, pData);
        return ERROR_CODE_WSO_WAIT_ABANDONED;
    }
}

DWORD WINAPI thread_main(LPVOID lpParam)
{
    PTHREADDATA thread_params = (PTHREADDATA)lpParam;
    if ((fseek(thread_params->in_file, (sizeof(char) * thread_params->start), SEEK_SET)) != 0) error_handler(ERROR_CODE_FILE);
    int loop = 1;
    char* line = NULL;
    while ((line = read_line(thread_params->in_file, &loop)) != ((thread_params->end) + 1))
    {
        line = decrypte_line(line, thread_params->key, thread_params->mode);
        if (0 != write_new_line_to_output(thread_params->out_file, line)) error_handler(ERROR_CODE_WRITE);
    }
    return SUCCESS_CODE;
}

int count_elements(int* array) {
    int i = 0;
    while (*(array + i) != '\0') {
        i++;
    }
    return i;
}

int dispatch_threads(FILE* in_file, FILE* out_file, int key, int threads_required, int* rows_endings, char mode) {
    float rows_remaining = count_elements(rows_endings);
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
        pData->mode = mode;
        pData->in_file = in_file;
        pData->out_file = out_file;

        hThread = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            thread_main,            // thread function name
            pData,                  // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadId);           // returns the thread identifier 

        if (NULL == hThread) return ERROR_CODE_THREAD;

        wait_for_thread_execution_and_free(hThread, pData);

        rows_remaining -= thread_rows;
        threads--;
    }

}




