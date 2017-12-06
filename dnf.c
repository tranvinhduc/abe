#include "dnf.h"
void set_set (Set *b, Set x)
{
    b->size = x.size;
    for(int i = 0; i < b->size; i++)
        b->elements[i] = x.elements[i];
}

void print_set(Set *t)
{
  printf("%5d:[ ", t->size ); 
  for(int i = 0; i < t->size; i++)
    printf("%d, ", t->elements[i]);
  printf("]\n");
}


int isSubset(Set s1, Set s2) //s1 is a subset of s2
{
    if (s1.size > s2.size)
        return 0;

    int j= 0;
    for (int i = 0; i < s1.size; i++)
    {
        j = 0;
        for (; j < s2.size; j++)
            if (s1.elements[i] == s2.elements[j]) break;

        if (j == s2.size) return 0;

    }

    return 1;
}

void init_abf (DNF *B, int MAX) //Max of number of  B_i
{
    B->MaX = MAX;
    B->m = 0;
    B->clauses = (Set *) malloc (sizeof(Set) * B->MaX);
    if (B->clauses == 0)
    {
        perror ("Cannot allocation!");
        exit (-1);
    }
}

void free_abs (DNF *B)
{
    free(B->clauses);
}

void insert_abf (DNF *B, Set x)
{
    if ((B->m) < B->MaX)
        (B->m)++;

    set_set(&B->clauses[(B->m)-1], x);
}

void print_abf (DNF *B)
{
  printf ("%2d:[\n", B->m);

  for (int i = 0; i < B->m; i++)
    {
      print_set(&(B->clauses[i]));
    }
  printf("   ]\n");

}
