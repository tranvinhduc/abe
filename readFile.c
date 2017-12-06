#include "readFile.h"
void readFiles(const char *filename, Set *userAttrs, DNF *B)
{
    FILE * fp;
    
    fp = fopen (filename, "r+");
    if (fp == 0)
    {
        perror("Cannot open files!\n");
        exit (-1);
    }
    
    fscanf(fp, "%d", &(userAttrs->size));
    for(int i = 0; i < userAttrs->size ; i++)
    {
        fscanf(fp, "%d", &(userAttrs->elements[i]));
    }
    
    int num;
    fscanf(fp,  "%d",&num);
    init_abf(B, num);         //Max = M_Bi

    Set t;
    for(int i = 0; i < num ; i++)
    {
        fscanf(fp, "%d", &(t.size));
        for(int j = 0; j < t.size; j++)
        {
            fscanf(fp, "%d", &(t.elements[j]));
        }
        insert_abf(B, t);
    }
    fclose(fp);
}
