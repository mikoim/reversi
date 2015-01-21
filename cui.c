#include <stdio.h>
#include <time.h>
#include "ai_mc.h"
#include <stdlib.h>
#include "reversi.h"

#include "cui.h"

#define _OSX

#ifdef _OSX
#include <sys/time.h>
#else
#include <Windows.h>
#endif

int g_tryCount;
extern int g_tryCount;

void benchmark() {
    const long baseline = 2428500; // Mac mini Late 2012 (clang -O4) [microsecond]
    
    int blackhole;
    unsigned long result;
    Reversi board;
#ifdef _OSX
    struct timeval start, end;
#else
    unsigned long start, end;
#endif
    
    double rate;
    
    printf("benchmark() : Wait a moment...\n");
    
    srand(353535);
    
    r_init(&board);
    
#ifdef _OSX
    gettimeofday(&start, NULL);
#else
    start = GetTickCount();
#endif
    
    
    AI_MC(&board, &blackhole, &blackhole);
#ifdef _OSX
    gettimeofday(&end, NULL);
#else
    end = GetTickCount();
#endif
    
#ifdef _OSX
    result = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
#else
    result = (end - start) * 1000;
#endif
    
    rate = (double)baseline / result;
    
    g_tryCount *= rate;
    
    printf("benchmark() : Baseline %ld / Your system %ld => %f\n", baseline, result, rate);
    
    if (rate < 0.8) {
        printf("\n[WARNING] Your system is too slow! [WARNING]\n");
#ifdef _OSX
        printf("Tips : Compile with optimize option. (ex clang -O4 *.c\n\n");
#else
        printf("Tips : Compile with optimize option. (ex cl /O2 *.c\n\n");
#endif
    }
    
    printf("benchmark() : Let's start!\n\n");
}

void cuiTop(Menu* m)
{
    char buf[1000];
    int tmp;
    
    m->isBlackPlayer = -1;
    m->isWhitePlayer = -1;
    
    printf("LoLoLoLo v0.2.2\n\n");
    
#ifdef _DEBUG
    printf("!!! Debug Mode !!!\n");
#endif
    
    benchmark();
    
    do {
        printf("Black [AI : 0 / Player : 1] > ");
        fgets(buf , sizeof(buf) / sizeof(buf[0]) , stdin);
        sscanf(buf, "%d", &m->isBlackPlayer);
    } while (m->isBlackPlayer != 0 && m->isBlackPlayer != 1);
    
    do {
        printf("White [AI : 0 / Player : 1] > ");
        fgets(buf , sizeof(buf) / sizeof(buf[0]) , stdin);
        sscanf(buf, "%d", &m->isWhitePlayer);
    } while (m->isWhitePlayer != 0 && m->isWhitePlayer != 1);
    
    if (m->isBlackPlayer != 1 || m->isWhitePlayer != 1) {
        do {
            printf("AI [Monte Carlo : 0 / Stub : 1] > ");
            fgets(buf , sizeof(buf) / sizeof(buf[0]) , stdin);
            sscanf(buf, "%d", &m->AI);
        } while (m->AI != 0 && m->AI != 1);
        
        if (m->AI == 0) {
            do {
                printf("Difficulty [Normal : 0 / Custom : 1] > ");
                fgets(buf , sizeof(buf) / sizeof(buf[0]) , stdin);
                sscanf(buf, "%d", &tmp);
            } while (tmp != 0 && tmp != 1);
            
            if (tmp == 1) {
                do {
                    printf("Difficulty [1000 - %d] > ", g_tryCount * 2);
                    fgets(buf , sizeof(buf) / sizeof(buf[0]) , stdin);
                    sscanf(buf, "%d", &tmp);
                } while (tmp < 1000 || tmp > g_tryCount * 2);
                
                g_tryCount = tmp;
            }
        }
    }
}

void cuiPlaying(int* x, int* y)
{
    char buf[1000];
    
    *x = (*y = -1);
    
    do {
        printf("[x,y] > ");
        fgets(buf , sizeof(buf) / sizeof(buf[0]) , stdin);
        sscanf(buf, "%d,%d", x, y);
    } while (*x < 0 || *x >= 8 || *y < 0 || *y >= 8);
}