//
//  lsss.c
//  ABE
//
//  Created by Tran Vinh Duc on 11/23/17.
//  Copyright © 2017 Tran Vinh Duc. All rights reserved.
//

#include "lsss.h"
#include <stdlib.h>

void copyrow (int numcol, int *des, int *src)
{
  for (int i =0; i < numcol; i++) {
    des[i] = src[i];
  }
}

void initlsss(LSSS *lsss)
{
  for (int i = 0; i < lsss->nrow; i++) {
    for (int j = 0; j < lsss->ncol; j++) {
      lsss->M[i][j] = 0;
    }
  }
}

void updatechild (Node *root, int *c, LSSS *lsss, int *t)
{

  if (root->d.type == 0)
    {
      copyrow(root->d.c, lsss->M[*t], root->d.vector);
      lsss->index[*t].index = root->d.label;
      lsss->index[*t].set_index = root->d.index;
      (*t)++;
    }

  else if (root->d.type == 1) // AND nodes
    { 
      root->r->d.vector = (int *)calloc((*c)+1, sizeof(int) );
      root->r->d.c = (*c)+1;
      root->l->d.vector = (int *)calloc((*c)+1, sizeof(int));
      root->l->d.c = (*c)+1;

      for (int i = 0; i < *c; i++)
	{
	  root->r->d.vector[i] = 0;
	}
      root->r->d.vector[*c] = -1;
        
      copyrow(root->d.c, root->l->d.vector, root->d.vector);
      root->l->d.vector[*c] = 1;
      (*c)++;
    }

  else if (root ->d.type == 2)  //OR nodes
    {
      root->r->d.vector = (int *)calloc(*c, sizeof(int) );
      root->r->d.c = (*c);
      root->l->d.vector = (int *)calloc(*c, sizeof(int));
      root->l->d.c = (*c);
      copyrow(root->d.c, root->l->d.vector, root->d.vector);
      copyrow(root->d.c, root->r->d.vector, root->d.vector);
    }
}

/**
   Computes matrix LSSS from DNF formula
   But it is easy to make it work with general formula

   @param lsss output lsss matrix
   @param dnf dnf formula
*/
void dnf2lsss(LSSS *lsss, DNF dnf)
{
  int sum = 0;
    
  for (int i = 0; i < dnf.m; ++i)
    {
      sum = sum+ dnf.clauses[i].size;
    }
    
  lsss->nrow = sum;
  lsss->ncol = sum - 1; // In DNF form, colLSSS = rowLSSS -1
  // TODO: for general circuit, we need change it
  lsss->M = (int**) malloc(sizeof(int*) * lsss->nrow);
  for (int i=0; i<sum; i++)
    lsss->M[i] = (int *)malloc(lsss->ncol*  sizeof(int));
    
  lsss->index = (Att_index*) malloc(sizeof(Att_index)*sum);
    
  initlsss(lsss);

  // Tree construction
  Node * root = dnf2tree(dnf);

  //LSSS construction
  Node * r = root;
  queueinit();
  qpush(root);

  int c = 1;
  int t = 0;    //current row of LSSS matrix

  // BFS on Tree
  root->d.vector = (int*) malloc(sizeof(int) * c);
  root->d.c = c;
  root->d.vector[c-1] = 1;
    
  while (!queueempty())
    {
      root = qpop();               
      updatechild(root, &c, lsss, &t);
      if (root->l != 0) qpush(root->l);
      if (root->r != 0) qpush(root->r);
        
    }

   freeBinTree(r);
}


/**
   Clear LSSS matrix

   @param lsss structure
*/
void freelsss(LSSS *lsss)
{
    
  for (int i=0; i<lsss->nrow; i++)
    free(lsss->M[i]);
  free(lsss->M);
}

/**
   Print out a matrix LSSS

   @param lsss matrix LSSS
*/
void printlsss(LSSS lsss)
{
  printf ("[ \n");
  for (int i = 0; i < lsss.nrow; i++) {
    printf("%2d%3d: " ,lsss.index[i].set_index, lsss.index[i].index);
    printf ("[");
    for (int j = 0; j < lsss.ncol-1; j++) {
      printf("%3d,",  lsss.M[i][j]);
    }
    printf("%3d",  lsss.M[i][lsss.ncol-1]);
        
    printf ("], ");
    printf("\n");
  }
  printf (" ]");
}





