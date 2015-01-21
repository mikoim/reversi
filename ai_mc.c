#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ai_stub.h"
#include "ai_mc.h"

int g_tryCount = 30000;

extern int g_tryCount;

#define MAX_THREADS 32

#define _OSX

#ifdef _OSX
#include <pthread.h>
#include <unistd.h>
#else
#pragma message ("LoLoLoLo for Windows (You will need to define _OSX on Mac OS X)")
#include <Windows.h>
#endif

int getCpuCount() {
    int count;
#ifdef _OSX
    count = (int)sysconf(_SC_NPROCESSORS_ONLN);
#else
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    count = (int)info.dwNumberOfProcessors;
#endif
    return count;
}
#ifdef _OSX
void* AI_MC_Thread(void* args)
#else
DWORD WINAPI AI_MC_Thread(void* args)
#endif
{
    int j, x_, y_;
    Reversi tmp;
    AI_MC_Thread_Data* output = (AI_MC_Thread_Data*)args;
    
    for (j = 0; j < output->countTry; j++) {
        
        tmp = *output->orig;
        
        r_set(&tmp, tmp.current_, output->data->pos.x, output->data->pos.y);
        
        while (tmp.current_ != None) {
            AI_Stub(&tmp, &x_, &y_);
            r_set(&tmp, tmp.current_, x_, y_);
        }
        
        if (tmp.countBlack > tmp.countWhite) {
            output->countWinBlack++;
        } else if (tmp.countWhite > tmp.countBlack) {
            output->countWinWhite++;
        }
    }
    
#ifdef _OSX
    return NULL;
#else
    return 0;
#endif
}

int qsort_black(const void *a, const void *b)
{
    AI_MC_Data *cc, *dd;
    cc = (AI_MC_Data*)a;
    dd = (AI_MC_Data*)b;
    
    if (cc->rateBlack > dd->rateBlack) {
        return -1;
    } else if(cc->rateBlack < dd->rateBlack) {
        return 1;
    } else {
        return 0;
    }
}

int qsort_white(const void *a, const void *b)
{
    AI_MC_Data *cc, *dd;
    cc = (AI_MC_Data*)a;
    dd = (AI_MC_Data*)b;
    
    if (cc->rateWhite > dd->rateWhite) {
        return -1;
    } else if(cc->rateWhite < dd->rateWhite) {
        return 1;
    } else {
        return 0;
    }
}

void AI_MC(const Reversi* r, int* x, int* y)
{
    AI_MC_Data data[30] = {0}, best;
    int countWinBlack, countWinWhite, i, t, cpu;
    AI_MC_Thread_Data threadData[MAX_THREADS];
#ifdef _OSX
    pthread_t thread[MAX_THREADS];
#else
	HANDLE thread[MAX_THREADS];
#endif
    cpu = getCpuCount();
    if (cpu > MAX_THREADS) cpu = MAX_THREADS;
    
    for (i = 0; i < r->countCanSet; i++) {
        countWinBlack = 0;
        countWinWhite = 0;
        
        data[i].pos = r->canSet_[i];
        
        memset(threadData, 0, sizeof(threadData));
        
        for (t = 0; t < cpu; t++) {
            threadData[t].countTry = g_tryCount / cpu;
            if (g_tryCount % cpu != 0 && i == cpu - 1) threadData[t].countTry += g_tryCount % cpu;
            
            threadData[t].data = &data[i];
            threadData[t].orig = r;
#ifdef _OSX
            pthread_create(&thread[t], NULL, AI_MC_Thread, &threadData[t]);
#else
			thread[t] = CreateThread(NULL, 0, AI_MC_Thread, &threadData[t], 0, NULL);
#endif
        }
        
        for (t = 0; t < cpu; t++) {
#ifdef _OSX
            pthread_join(thread[t], NULL);
#else
			WaitForSingleObject(thread[t], INFINITE);
#endif
            countWinBlack += threadData[t].countWinBlack;
            countWinWhite += threadData[t].countWinWhite;
        }
        
        data[i].rateBlack = (double)countWinBlack / g_tryCount;
        data[i].rateWhite = (double)countWinWhite / g_tryCount;
        
        printf("AI_MC(%d) (%d,%d) B %f / W %f\n", g_tryCount, data[i].pos.x, data[i].pos.y, data[i].rateBlack, data[i].rateWhite);
    }
    
    switch (r->current_) {
        case Black:
            qsort(data, r->countCanSet, sizeof(AI_MC_Data), qsort_black);
            break;
            
        case White:
            qsort(data, r->countCanSet, sizeof(AI_MC_Data), qsort_white);
            break;
            
        case None:
            break;
    }
    
    best = data[0];
    
    *x = best.pos.x;
    *y = best.pos.y;
    
    printf("AI_MC(%d) (%d,%d) B %f / W %f [Best!]\n", g_tryCount, best.pos.x, best.pos.y, best.rateBlack, best.rateWhite);
}