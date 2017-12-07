#include "parameters.h"


void init_secret_key_user(Secret_key_user *su, Set *subset)
{
    su->size = subset->size;
    su->key = malloc(sizeof(int) * su->size);
    su->d = malloc (sizeof(element_t) * su->size);
    su->subset.size = subset->size;
    
    for(int i = 0; i < su->size; i ++)   // Init for hash tables !!!
        su->key[i] = -1;
    
    for (int i = 0; i< subset->size; i++) // Create hash table for search
    {
        su->subset.elements[i] =  subset->elements[i];
        insert_key(su, subset->elements[i]);
    }
}

void free_secret_key_user(Secret_key_user *su)
{
    free(su->key);
    free(su->d);
}

void insert_key(Secret_key_user *su, int key)
{
    int x = key % su->size;
    while (su->key[x] != -1) x = (x+1) % su->size;
    
    su->key[x] = key;
}

int MAP_d(Secret_key_user *su, int key)
{
    int x = key % su->size;
    while (su->key[x] != key) x = (x+1)%su->size;
    
    return x;
}


//For cipher text
void init_cipher_text(Cipher_text *ct, int m)
{
    ct->m = m;
    ct->Ci = (element_t*) malloc(sizeof(element_t) * ct->m);
    if (ct->Ci  == 0)
    {
        perror("Cannot allocation!");
        exit(-1);
    }
}

void free_cipher_text(Cipher_text *ct)
{
    ct->m = 0;
    free(ct->Ci);
}

// Free global parameters
void free_global_parameters (Global_parameters *g)
{
  element_clear(g->g);
  element_clear(g->ga);

  for (int i = 0; i < N; i++)
    element_clear(g->h[i]);
}

