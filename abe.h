#ifndef ABE_H
#define ABE_H

#include <pbc.h>
#include <stdio.h>
#include <stdlib.h>
#include "parameters.h"
#include "lsss.h"


extern pairing_t pairing;
extern Global_parameters param;
extern element_t MSK;

void  ABE (void);

void Setup (void);
	

//Takes as input a user u and his set of attributes B(u), 
//as well as the public parameters param and the master key MSK. 
//It outputs the user’s private key du
int Extract (Secret_key_user *du, Set *subset);

	
// Takes as input a message M, an access policy A over the universe of attributes and 
//the public parameters param. 
//It outputs the ciphertext ct along with a description of the access policy A.
int Encrypt (Cipher_text *ct, element_t M, DNF *beta);

int EncryptLSSS (Cipher_text *ct, element_t M, DNF *beta);

int Decrypt(element_t M, Cipher_text *ct, DNF *beta,
		    Secret_key_user *du);

int DecryptLSSS(element_t M, Cipher_text *ct, DNF *beta,
                        Secret_key_user *du);

void product (element_t prod, element_t *v, int *vec, int size);

#endif
