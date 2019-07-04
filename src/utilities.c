/**
 * NAME: Patrick Di Salvo
 * 0629465
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "utilities.h"




/****STATIC FUNCTIONS***/

static int partition(void *a, int l, int r, CompareFunc compare)
{
    int i = -1;
    int j = 0;
    for (j = 0; j < r; j++)
    {
        if (compare(((void**)(a))[j], ((void**)a)[r]) <= 0)
        {
            i++;
            void *temp = ((void**)a)[i];
            ((void**)a)[i] = ((void**)a)[j];
            ((void**)a)[j] = temp;
        }
    }
    void *temp = ((void**)a)[i + 1];
    ((void**)a)[i + 1] = ((void**)a)[r];
    ((void**)a)[r] = temp;
    return (i + 1);
}

void swap(void* a, void* b, SwapFunc s)
{
     s(a, b);
}

void swapInts(void* a, void* b)
{
    int temp = 0;
    int* first = (int*)(a);
    int* second = (int*)(b);
    temp = (*first);
    (*first) = (*second);
    (*second) = temp;

}



int compare(void* a, void* b, CompareFunc c)
{
  return c(a, b);
}


bool sameobject(void* a, void* b)
{
    if(a == b)return true;
    return false;
}




int* test_clone(void* a)
{
    int* f = (int*)a;
   int* clone = (int*)calloc(1, sizeof(int));
    *clone = *f;
   return clone;
}



                        /******MEMORY ALLOCATERS*****/

void resize_real(AnyData* a, int size)
{
     size += 1000;
    *a = realloc(*a, size);
}

                        /******DESTROYERS****/

void clear_real(AnyData d)
{
    assert(d != NULL);
    void** delete = (void**)d;
    *delete = *((void**)(d));
    if(*delete == NULL){
        return;
    }
    free(*delete);
    *((void**)d) = NULL;
}


/****SORTERS***/

void quick_sort(void *array, int left, int right, CompareFunc compare)
{
    if (left < right)
    {
        int p = partition(array, left, right, compare);
        quick_sort(array, left, (p - 1), compare);
        quick_sort(array, (p + 1), right, compare);
    }
}

  /*********Dummy Functions****/

char* dummy_print(AnyData p){
     char* printer = calloc(100, sizeof(char));
     int* temp = (int*)p;
     sprintf(printer, "%d", *temp);
     printer = strcat(printer, "\n");
     return printer;
}

int dummy_compare(const void* a, const void* b)
{
    if(a == NULL)return -2;
    if(b == NULL)return -2;
    int* first = (int*)a;
    int* second = (int*)b;
    if(*first == *second)return 0;
    else if(*first < *second)return -1;
    else return 1;
}

void dummy_delete(AnyData d){
    clear(d);
}


/*****CLONER FUNCTIONS****/

Cloner new_cloner(CloneFunc clonerFunction)
{
    Cloner cloner;
    cloner.cloneData = clonerFunction;
    return cloner;
}


/****PRINTERS***/

void print(void* p)
{
    if(p == NULL){
        printf("\nNULL ARGUMENT\n");
        return;
    }
    char* pr = (char*)p;
    if(strlen(pr)==0)printf("\nEMPTY ARGUMENT\n");
    printf("\n%s\n", pr);
}