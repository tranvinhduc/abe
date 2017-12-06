#include "abe.h"


pairing_t pairing;
Global_parameters param;
element_t MSK;

void ABE ()
{
    char param[1024];
    size_t  count = fread(param, 1, 1024,  stdin);
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");
}

void Setup ()
{
    element_init_G1(param.g, pairing);
    element_random (param.g);
    
    element_t alpha;
    
    element_init_Zr(alpha,pairing);
    element_random(alpha);
    element_init_G1(MSK, pairing);
    element_pow_zn(MSK, param.g, alpha);  //ga = g^a
    
    
    element_t a;
    element_init_Zr(a,pairing);
    element_random(a);
    
    
    element_init_G1(param.ga, pairing);
    element_pow_zn(param.ga, param.g, a);  //ga = g^a
    
    element_t eg;
    element_init_GT (eg, pairing);
    element_pairing(eg, param.g, param.g);
    
    element_init_GT (param.eg, pairing);
    element_pow_zn(param.eg, eg, alpha);
    
    
    for (int i = 0; i < N; i++)
    {
        element_init_G1(param.h[i], pairing);
        element_random(param.h[i]);
    }
    element_clear(alpha);
    element_clear(a);
    element_clear(eg);
}


//Takes as input a user u and his set of attributes B(u), 
//as well as the public parameters param and the master key MSK. 
//It outputs the user’s private key du

int Extract (Secret_key_user *du, Set *subset)
{
    init_secret_key_user(du, subset);

    element_t su;
    element_init_Zr(su, pairing);
    element_random (su);
    
    
    element_t tmp;
    element_init_G1(tmp, pairing);
    element_pow_zn(tmp, param.ga, su); //tmp = (g^a)^su
    
    element_init_G1(du->du0, pairing);
    element_mul(du->du0, MSK, tmp);   //du0 = MSK * (g^a)^su
    
    element_init_G1(du->du0_, pairing);
    element_pow_zn(du->du0_, param.g, su);  //du0_ = g^su
    
    int index = -1;
    for (int i = 0; i< subset->size; i++)
    {
        index = MAP_d(du, subset->elements[i]);             //Misunderstanding 
        element_init_G1(du->d[index], pairing);
        element_pow_zn(du->d[index], param.h[subset->elements[i]], su);
    }
    
    element_clear(su);
    element_clear(tmp);
    
    return 0;
}


// Takes as input a message M, an access policy A over the universe of attributes and 
//the public parameters param. 
//It outputs the ciphertext ct along with a description of the access policy A.
int Encrypt (Cipher_text * ct, element_t M, DNF *beta)
{
    
    init_cipher_text(ct, beta->m);
    
    
    element_t s;
    element_init_Zr(s, pairing);
    element_random (s);
    
    element_t tmp;
    element_init_GT(tmp, pairing);
    
    
    element_pow_zn(tmp, param.eg, s); //tmp = (e(g,g)^alpha)^s
    
    element_init_GT(ct->C, pairing);
    element_mul(ct->C, M, tmp);        // C = M*tmp
    
    element_init_G1(ct->C0, pairing);
    element_pow_zn(ct->C0, param.g, s);  //C0 = g^s
    
    /* ct->m = beta.m; */
    int i = 0;
    for (i = 0; i < beta->m; i++)
    {
        element_init_G1(ct->Ci[i], pairing);
        element_set(ct->Ci[i], param.ga);
        
        for (int j = 0; j < beta->clauses[i].size; j++)
            element_mul(ct->Ci[i], ct->Ci[i], param.h[beta->clauses[i].elements[j]]);
        
        element_pow_zn(ct->Ci[i], ct->Ci[i], s);
    }
    
    element_clear(s);
    element_clear(tmp);
    
    return 0;
}



int Decrypt(element_t M, Cipher_text *ct, DNF *beta,
            Secret_key_user *du)
{
    element_init_GT(M, pairing);
    
    int j = 0;
    
    for (j=0; j< beta->m; j++)
        if (isSubset(beta->clauses[j], du->subset)) break;
    
    if (j == beta->m) return 1; //Error
    
    
    element_t tgt1;
    element_t tgt2;
    element_t K;
    
    
    element_init_GT (tgt1, pairing);
    element_init_GT (tgt2, pairing);
    element_init_GT (K, pairing);
    
    element_t tg1;
    element_init_G1(tg1, pairing);
    element_set (tg1, du->du0);
    
    int index = -1;
    for (int i = 0; i < beta->clauses[j].size; i++ ){
        index = MAP_d(du, beta->clauses[j].elements[i]);
        element_mul (tg1, tg1, du->d[index]);
    }
    
    
    element_pairing(tgt1, ct->C0, tg1);
    element_pairing(tgt2, du->du0_, ct->Ci[j]);
//    element_div (K, tgt1, tgt2);
//    element_div (M, ct->C, K );
    element_div (K, tgt2, tgt1);   // K = 1/K
    element_mul (M, ct->C, K );

    element_clear(tgt1);
    element_clear(tgt2);
    element_clear(K);
    element_clear(tg1);
    
    return 0;
    
}




int EncryptLSSS (Cipher_text * ct, element_t M, DNF *beta)
{
    LSSS lsss;
    dnf2lsss(&lsss, *beta);
    
    int n = lsss.ncol;
    int l = lsss.nrow;
    
    init_cipher_text(ct, l);
    
    element_t s;
    element_init_Zr(s, pairing);
    element_random (s);
    
    element_t tmp;
    element_init_GT(tmp, pairing);
    
    
    element_pow_zn(tmp, param.eg, s); //tmp = (e(g,g)^alpha)^s
    
    element_init_GT(ct->C, pairing);
    element_mul(ct->C, M, tmp);        // C = M*tmp
    
    element_init_G1(ct->C0, pairing);
    element_pow_zn(ct->C0, param.g, s);  //C0 = g^s
    
    
    //Chooses a random vector v = (s,y2,...,yn)
    element_t *v = (element_t*)malloc(sizeof(element_t) * n);
    element_init_Zr(v[0], pairing);
    element_set(v[0], s);
    for (int i=1; i < n; i++) {
        element_init_Zr(v[i], pairing);
        element_random(v[i]);
    }
    
    // Computes lambda_i = v * M_i
    // where M_i is vector correponding to ith-row of M
    element_t *lambda = malloc(sizeof(*lambda) * l);
    for (int i = 0; i<l; i++) {
        element_init_Zr(lambda[i], pairing);
        product(lambda[i], v, lsss.M[i], n);
    }
    
    element_t tmp2;
    element_init_G1(tmp2, pairing);
    
    
    for (int i = 0; i < l; i++)
    {
        element_init_G1(ct->Ci[i], pairing);                   // Ci = g^a
        element_set(ct->Ci[i], param.ga);
        element_pow_zn(ct->Ci[i], ct->Ci[i], lambda[i]);       // Ci = g^a^lambda_i
        element_pow_zn(tmp2, param.h[lsss.index[i].index], s); //tmp2 =   h_index^s
        element_div(ct->Ci[i], ct->Ci[i], tmp2);               //Ci = g^a^lambda_i / h_index^s
    }
    
    
    //Clear everything
    for (int i=0; i < n; i++) {
        element_clear(v[i]);
    }
    free(v);

    for (int i=0; i < l; i++) {
        element_clear(lambda[i]);
    }
    free(lambda);

    element_clear(s);
    element_clear(tmp);
    element_clear(tmp2);
    freelsss(&lsss);

    return 0;
    
}

int DecryptLSSS(element_t M, Cipher_text *ct, DNF *beta,
                Secret_key_user *du)
{
    element_init_GT(M, pairing);
    int j = 0;
    
    for (j=0; j< beta->m; j++)
        if (isSubset(beta->clauses[j], du->subset)) break;
    
    if (j == beta->m) return 1; //Error
    // j is set_index
    
    element_t tgt1;
    element_t tgt2;
    element_t K;
    
    element_init_GT (tgt1, pairing);
    element_init_GT (tgt2, pairing);
    element_init_GT (K, pairing);
    
    element_t tg1;
    element_init_G1(tg1, pairing);
    element_set1(tg1);
    
    
    element_t tg2;
    element_init_G1(tg2, pairing);
    element_set1(tg2);
    
    
    LSSS lsss;
    dnf2lsss(&lsss, *beta);
    
    if (DEBUG) printlsss(lsss);
    
    int index = -1;
    //int lenomega = lsss.nrow;      //
    //long *omega = (long*) malloc (sizeof(*omega) * lenomega);
    for (int i = 0; i < lsss.nrow ; i++) {
        if (lsss.index[i].set_index == j)
        {  // omega[i] = 1;
            element_mul(tg1, tg1, ct->Ci[i]);         // * Ci^wi
            index = MAP_d(du, lsss.index[i].index);
            element_mul(tg2, tg2, du->d[index]); // du^wi
        }
    }
    
    element_invert(tg1, tg1);
    element_div(tg2, du->du0, tg2);
    
    element_pairing(tgt1, tg1, du->du0_);
    element_pairing(tgt2, ct->C0,tg2);
    element_mul (K, tgt1, tgt2);
    element_div (M, ct->C, K );
    
    element_clear(tgt1);
    element_clear(tgt2);
    element_clear(K);
    element_clear(tg1);
    element_clear(tg2);
    freelsss(&lsss);
    return 0;
    
}

void product (element_t prod, element_t *v, int *vec, int size)
{
    element_set_si(prod, 0 );
    
    element_t tmp;
    element_init_Zr(tmp, pairing);
    
    for (int i = 0; i < size; i++) {
        element_mul_si(tmp, v[i], vec[i]);
        element_add(prod, prod, tmp);
    }
    
    element_clear(tmp);
}


