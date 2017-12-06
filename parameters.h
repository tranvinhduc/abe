#ifndef parameters_h
#define parameters_h

#include <pbc.h>
#include "dnf.h"
#include "fix_parameters.h"

// Global parameters
struct global_parameters
{
    element_t g;
    element_t ga;
    element_t eg;
    element_t h[N];
};

typedef struct global_parameters Global_parameters;

void free_global_parameters (Global_parameters *g);



// Secret keys
struct secret_key_user
{
    int size;   //size of secret_key_user
    Set subset;
    int *key;
    element_t *d;
    element_t du0;
    element_t du0_;
    
};
typedef struct secret_key_user Secret_key_user;

void init_secret_key_user(Secret_key_user *su, Set *subset);
void free_secret_key_user(Secret_key_user *su);
void insert_key(Secret_key_user *su, int key);
int MAP_d(Secret_key_user *su, int key);


// Cipher text
struct cipher_text
{
    int m; // size of beta
    element_t C;
    element_t C0;
    /* element_t Ci[M_Bi];  // m <= N */
    element_t *Ci;
};

typedef struct cipher_text Cipher_text;
void init_cipher_text(Cipher_text *ct, int m);
void free_cipher_text (Cipher_text *ct);


#endif /* parameters_h */
