//
//  bintree.h
//  ABE
//
//  Created by Tran Vinh Duc on 11/25/17.
//  Copyright © 2017 Tran Vinh Duc. All rights reserved.
//

#ifndef bintree_h
#define bintree_h

#include <stdio.h>
#include "dnf.h"

#define LSSS_MAX_COL 1000

typedef struct
{
  
  int type; // 0: leaf, 1: And, 2: Or node
  int label;
  int index; // index set
  int c;   //sizeof vector
  int *vector; //[LSSS_MAX_COL];
} NodeInfo;



typedef struct node
{
  NodeInfo d;
  struct node *l, *r;     //left and right
} Node;


Node *createNode(int type, int label, int index);  // type = 0: leaf, 1: And, 2: Or node
Node * ands2tree (Set b, int);
Node * dnf2tree (DNF dnf);
void freeBinTree (Node *root);

#endif /* bintree_h */
