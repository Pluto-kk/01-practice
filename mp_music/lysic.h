#ifndef LYSIC_H
#define LYSIC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct time
{
    int h;
    int m;
    int s;
}TIME;
typedef struct kk
{
    struct kk *prev;
    TIME *t;
    char src[128];
    int num;
    struct kk *next;
}KK;


void KK_CREATE(KK **);
void SRC_HANDLE(char *,KK *);
void SRC_SORT(KK **);
void GET_SRC(FILE *,KK *);
void DIS_SRC(TIME *,KK *,KK *);
void DISPLAY(TIME *,KK *);
void END_OF_SRC(KK *);
int lysic_catch();

#endif // LYSIC_H
