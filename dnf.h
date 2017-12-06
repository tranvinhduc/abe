//
//  DNF_param.h
//  ABE
//
//  Created by Tran Vinh Duc on 11/23/17.
//  Copyright © 2017 Tran Vinh Duc. All rights reserved.
//

#ifndef DNF_param_h
#define DNF_param_h

#include <stdlib.h>
#include <stdio.h>
#include "fix_parameters.h"

typedef struct
{
    int size;
    int elements[MAX_SET];
} Set;

void set_set (Set *b, Set x);
int isSubset(Set s1, Set s2); //test if s1 is a subset of s2
void print_set(Set *t);

// Access boolean formula
struct DNF
{
    int MaX;
    int m;
    Set *clauses;
};
typedef  struct DNF DNF;
void init_abf (DNF *B, int MAX); //Max of number of  B_i
void insert_abf (DNF *B, Set x); //Insert an element
void free_abf (DNF *B);
void print_abf (DNF *B);







#endif /* DNF_param_h */
