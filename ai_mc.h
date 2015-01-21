#ifndef lol_ai_mc_h
#define lol_ai_mc_h

#include "reversi.h"

typedef struct {
    Point pos;
    double rateBlack;
    double rateWhite;
} AI_MC_Data;

typedef struct {
    int countTry, countWinBlack, countWinWhite;
    AI_MC_Data* data;
    const Reversi* orig;
} AI_MC_Thread_Data;

void* AI_MC_Thread(void* args);
void AI_MC(const Reversi* r, int* x, int* y);

#endif
