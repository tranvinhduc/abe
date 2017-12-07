#ifndef lsss_h
#define lsss_h

#include "queue.h"
#include "bintree.h"
#include "parameters.h"

typedef struct {
    int set_index, index;
} Att_index;

typedef struct
{
    int nrow;               //Number of rows
    int ncol;               //Number of cols
    int **M;           //
    Att_index *index;
} LSSS;


void initlsss(LSSS *lsss);
void updatechild (Node *root, int *c, LSSS *lsss, int *t);
void dnf2lsss(LSSS *lsss, DNF dnf);
void freelsss(LSSS *lsss);
void printlsss(LSSS lsss);

#endif /* lsss_h */
