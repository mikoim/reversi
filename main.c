#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "reversi.h"
#include "ai_stub.h"
#include "ai_mc.h"
#include "cui.h"

typedef void (*pAi)(const Reversi* r, int* x, int* y);

int main()
{
    Reversi r;
    pAi AI;
    Menu m;
    int x, y;
    
    srand((unsigned int)time(NULL));
    
    cuiTop(&m);
    
    switch (m.AI) {
        case 0:
            AI = &AI_MC;
            break;
            
        default:
            AI = &AI_Stub;
            break;
    }
    
    r_init(&r);
    
    while (r.current_ != None) {
        r_dump(&r);
        
        switch (r.current_) {
            case Black:
                if (m.isBlackPlayer) {
                    do {
                        cuiPlaying(&x, &y);
                    } while (r_set(&r, Black, x, y) == -1);
                } else {
                    (AI)(&r, &x, &y);
                    printf("[x,y] > %d,%d\n", x, y);
                    if (r_set(&r, Black, x, y) == -1) {
                        printf("AI Error.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
                
            case White:
                if (m.isWhitePlayer) {
                    do {
                        cuiPlaying(&x, &y);
                    } while (r_set(&r, White, x, y) == -1);
                } else {
                    (AI)(&r, &x, &y);
                    printf("[x,y] > %d,%d\n", x, y);
                    if (r_set(&r, White, x, y) == -1) {
                        printf("AI Error.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
                
            case None:
                break;
        }
    }
    
    r_dump(&r);
    
    if (r.countBlack == r.countWhite) {
        printf("It's a draw.\n");
    } else if (r.countBlack > r.countWhite) {
        printf("Black won by %d.\n", r.countBlack - r.countWhite);
    } else {
        printf("White won by %d.\n", r.countWhite - r.countBlack);
    }
    
    return 0;
}