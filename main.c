#include <stdio.h>
#include "abe.h"
#include <pbc.h>
#include "bintree.h"
#include "readFile.h"

#include <pbc_test.h>

#define NUMTEST 1

int main(int argc, char const *argv[])
{
    double time1, time2;

    DNF beta;
    Set subset;

    if (argc < 1)
    {
        perror("Usage: abe datafile");
        exit(-1);
    }
    
    readFiles(argv[1], &subset, &beta);
    printf ("Data file: %s\n", argv[1]);
    ABE();
    
    time1 = pbc_get_time();
    
    Setup();
    
    time2 = pbc_get_time();
    
    printf ("Setup: %fs\n", time2 - time1);
    
    Secret_key_user du;
    
    time1 = pbc_get_time();
    Extract (&du, &subset);
    time2 = pbc_get_time();
    printf ("Extract: %fs\n", time2 - time1);
    
    
    element_t M1;
    element_init_GT(M1, pairing);
    element_random(M1);

    Cipher_text ct;
    element_t M2;

    double everage = 0;
    for (int i=0; i < NUMTEST; i++) {
        time1 = pbc_get_time();
        EncryptLSSS(&ct, M1, &beta);
        time2 = pbc_get_time();
        everage += (time2 - time1);
    }
 //   printf ("&%4f ms ", everage*1000.0/NUMTEST);
    printf ("EncryptLSSS: %f s\n", everage/NUMTEST);
    
    everage = 0;
    for(int i = 0; i < NUMTEST; i++){
        time1 = pbc_get_time();
        DecryptLSSS(M2, &ct, &beta, &du);
        time2 = pbc_get_time();
        everage += (time2 - time1);
    }
//    printf ("&%4f ms ", everage * 1000.0/NUMTEST);
    printf ("DecryptLSSS: %f s\n", everage/NUMTEST);
    if (!element_cmp(M1, M2))
        printf ("LSSS Decrypt: OK!\n");
    else printf ("LSSS Decrypt: Error!\n");

    free_cipher_text(&ct);

    everage = 0;
    
    for (int i = 0; i < NUMTEST; i++)
    {
        time1 = pbc_get_time();
        Encrypt(&ct, M1, &beta);
        time2 = pbc_get_time();
        everage += (time2 - time1);
    }
  //  printf ("&%4f ms ", everage*1000.0/NUMTEST);
    printf ("Encrypt: %f s", everage/NUMTEST);
    
    everage = 0;

    for (int i = 0; i < NUMTEST; i++) {
        time1 = pbc_get_time();
        Decrypt(M2, &ct, &beta, &du);
        time2 = pbc_get_time();
        everage += (time2 - time1);
    }

    //printf ("&%4f ms\\\\ \n", everage*1000.0/NUMTEST);
    printf ("Decrypt: %f s\n", everage/NUMTEST);
    
    if (!element_cmp(M1, M2))
        printf ("Decrypt: OK!\n");
    else printf ("Decrypt: Error!\n");

    free_cipher_text(&ct);

    
    free_secret_key_user(&du);

    element_clear(M1);
    element_clear(M2);
    element_clear(MSK);
    free_global_parameters(&param);
    pairing_clear(pairing);
    
    return 0;
}
