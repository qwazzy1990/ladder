/**
 * NAME: Patrick Di Salvo
 * 0629465
 * */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>
#include "Numbers.h"
#include "DynamicString.h"

/******STATIC FUNCTIONS****/

static int _____compare_ints(const void *a, const void *b)
{
    long long *first = (long long *)a;
    long long *second = (long long *)b;
    if (*first < *second)
    {
        return -1;
    }
    else if (*first == *second)
    {
        return 0;
    }
    else
        return 1;
}

static int _____compare_doubles(const void *a, const void *b)
{
    long double *first = (long double *)a;
    long double *second = (long double *)b;
    if (*first < *second)
        return -1;
    else if (*first == *second)
        return 0;
    else
        return 1;
}

/****FUNCTIONS****/

/***CONSTRUCTORS***/

Int new_int(void)
{
    new_object(Int, n, 1);
    n->ints = NULL;
    n->size = 0;

    n->concat = concat_int;
    n->add = add_int;
    n->remove = remove_int;
    n->remove_at = remove_int_at;
    n->remove_smallest = remove_smallest_int;
    n->remove_largest = remove_largest_int;
    n->remove_all = remove_all_ints;
    n->destroy = destroy_int;

    n->print = print_int;
    n->sort = sort_int;

    return n;
}

void concat_int(Int dest, Int source)
{
    if (source == NULL)
        return;
    if (source->size == 0)
        return;
    if (dest == NULL)
    {
        return;
    }
    if (dest->size == 0)
    {
        forall(source->size)
        {
            dest->add(dest,  source->ints[x]);
        }
        return;
    }
    int index = dest->size;
    int memSize = dest->size + source->size;
    dest->ints = realloc(dest->ints, memSize * sizeof(long long));
    forall(source->size)
    {
        dest->ints[x] = source->ints[x];
        index++;
        dest->size++;
    }
    return;
}

Double new_double(void)
{
    new_object(Double, n, 1);
    n->doubles = NULL;
    n->size = 0;

    n->add = add_double;
    n->concat = concat_double;
    n->remove = remove_double;
    n->remove_at = remove_double_at;
    n->remove_smallest = remove_smallest_double;
    n->remove_largest = remove_largest_double;
    n->remove_all = remove_all_doubles;
    n->destroy = destroy_double;

    n->print = print_double;
    n->sort = sort_double;
    return n;
}

void concat_double(Double dest, Double source)
{
    if (source == NULL)
        return;
    if (source->size == 0)
        return;
    if (dest == NULL)
    {
        return;
    }
    if (dest->size == 0)
    {
        forall(source->size)
        {
            dest->add(dest, source->doubles[x]);
        }
        return;
    }
    int index = dest->size;
    int memSize = dest->size + source->size;
    dest->doubles = realloc(dest->doubles, memSize * sizeof(long double));
    forall(source->size)
    {
        dest->doubles[x] = source->doubles[x];
        index++;
        dest->size++;
    }
    return;
}
/***END CONSTRUCTORS***/

void add_int(Int m, int n)
{
    if (m == NULL)
        return;
   

    int index = m->size;
    if (m->size == 0)
    {
        m->ints = calloc(1, sizeof(long long));
    }
    else
    {
        int memSize = 1 + m->size;
        m->ints = realloc(m->ints, memSize * sizeof(long long));
    }
    m->ints[index] = n;
    m->size++;
}

void add_double(Double d, double n)
{
    if (d == NULL)
        return;
    if (n <= 0)
        return;
    int index = d->size;
    if (d->size == 0)
    {
        d->doubles = calloc(n, sizeof(long double));
        d->doubles[index] = (long double)n;
        d->size = 1;
    }
    else
    {
        int memSize = d->size + 1;
        d->doubles = realloc(d->doubles, memSize * sizeof(long double));
        d->doubles[index] = (long double)n;
        d->size++;
    }
}
/***END SETTERS***/

/***DESTROYERS***/

void remove_int(Int m, long long n)
{
    if (m == NULL)
        return;
    if (m->size == 0)
        return;

    Int newInts = new_int();
    forall(m->size)
    {
        if (m->ints[x] != n)
        {
            newInts->add(newInts, m->ints[x]);
        }
    }
    clear(m->ints);
    m->size = 0;
    m->ints = newInts->ints;
    m->size = newInts->size;
    clear(newInts);
}

void remove_int_at(Int m, long long n)
{
    if (m == NULL)
        return;
    if (n < 0)
        return;
    if (m->size == 0)
        return;
    if (n >= m->size)
    {
        n = m->size - 1;
    }
    Int newInts = new_int();
    forall(m->size)
    {
        if (x != n)
        {
            newInts->add(newInts, m->ints[x]);
        }
    }

    clear(m->ints);
    m->size = newInts->size;
    m->ints = newInts->ints;
    clear(newInts);
}
void remove_smallest_int(Int m)
{
    if (m == NULL)
        return;
    if (m->size == 0)
        return;
    int index = -1;
    long long temp = m->ints[0];
    forall(m->size)
    {
        if (m->ints[x] < temp)
        {
            temp = m->ints[x];
            index = x;
        }
    }
    Int newInt = new_int();
    forall(m->size)
    {
        if (x != index)
        {
            newInt->add(newInt, m->ints[x]);
        }
    }
    clear(m->ints);
    m->ints = newInt->ints;
    m->size = newInt->size;
    clear(newInt);
    return;
}

void remove_largest_int(Int m)
{
    if (m == NULL)
        return;
    if (m->size == 0)
        return;
    int index = -1;
    long long temp = m->ints[0];
    forall(m->size)
    {
        if (m->ints[x] > temp)
        {
            temp = m->ints[x];
            index = x;
        }
    }
    Int newInt = new_int();
    forall(m->size)
    {
        if (x != index)
        {
            newInt->add(newInt,  m->ints[x]);
        }
    }
    if (m->ints != NULL)
    {
        clear(m->ints);
    }
    m->ints = newInt->ints;
    m->size = newInt->size;
    clear(newInt);
    return;
}
void remove_all_ints(Int m)
{
    if (m == NULL)
        return;
    if (m->size == 0)
        return;
    clear(m->ints);
    m->size = 0;
}

void destroy_int(void *m)
{
    if (m == NULL)
        return;
    Int mm = (Int)m;
    if (mm->ints != NULL)
    {
        clear(mm->ints);
    }
    clear(mm);
}

void remove_double(Double d, long double n)
{
    if (d == NULL)
        return;
    if (d->size == 0)
        return;

    Double newDouble = new_double();
    forall(d->size)
    {
        if (d->doubles[x] != n)
        {
            newDouble->add(newDouble, d->doubles[x]);
        }
    }
    clear(d->doubles);
    d->size = 0;
    d->doubles = newDouble->doubles;
    d->size = newDouble->size;
    clear(newDouble);
}

void remove_double_at(Double d, long long n)
{
    if (d == NULL)
        return;
    if (n < 0)
        return;
    if (d->size == 0)
        return;
    if (n >= d->size)
    {
        d->size = d->size - 1;
    }
    Double newDouble = new_double();
    forall(d->size)
    {
        if (x != n)
        {
            newDouble->add(newDouble, d->doubles[x]);
        }
    }

    clear(d->doubles);
    d->size = newDouble->size;
    d->doubles = newDouble->doubles;
    clear(newDouble);
}
void remove_smallest_double(Double d)
{

    if (d == NULL)
        return;
    if (d->size == 0)
        return;
    int index = -1;
    long double temp = d->doubles[0];
    forall(d->size)
    {
        if (d->doubles[x] < temp)
        {
            temp = d->doubles[x];
            index = x;
        }
    }
    Double newDouble = new_double();
    forall(d->size)
    {
        if (x != index)
        {
            newDouble->add(newDouble, d->doubles[x]);
        }
    }
    clear(d->doubles);
    d->doubles = newDouble->doubles;
    d->size = newDouble->size;
    clear(newDouble);
    return;
}

void remove_largest_double(Double d)
{
    if (d == NULL)
        return;
    if (d->size == 0)
        return;
    int index = -1;
    long double temp = d->doubles[0];
    forall(d->size)
    {
        if (d->doubles[x] > temp)
        {
            temp = d->doubles[x];
            index = x;
        }
    }
    Double newDouble = new_double();
    forall(d->size)
    {
        if (x != index)
        {
            newDouble->add(newDouble, d->doubles[x]);
        }
    }
    clear(d->doubles);
    d->doubles = newDouble->doubles;
    d->size = newDouble->size;
    clear(newDouble);
    return;
}

void remove_all_doubles(Double d)
{
    if (d == NULL)
        return;
    if (d->size == 0)
        return;
    clear(d->doubles);
    d->size = 0;
}

void destroy_double(void *
                        d)
{
    if (d == NULL)
        return;
    Double dd = (Double)d;
    clear(dd->doubles);
    clear(dd);
}

/***END DESTROYERS***/

/****PRINTERS FIX ME****/
char *print_int(void *m)
{
    if (m == NULL)
        return NULL;
    Int mm = (Int)m;

    int memSize = mm->size + 100;
    char *printer = calloc(memSize, sizeof(char));
    forall(mm->size)
    {
        new_object(char *, temp, 1000);

        sprintf(temp, "%lld", mm->ints[x]);
        strcat(printer, temp);
        strcat(printer, " ");
        clear(temp);
    }
    //FIX ME
    return printer;
}

char *print_double(void *d)
{
    if (d == NULL)
        return NULL;
    Double mm = (Double)d;

    if (mm->size == 0)
        return NULL;
    new_object(char *, printer, (mm->size + 100));
    forall(mm->size)
    {
        new_object(char *, temp, 1000);

        sprintf(temp, "%Lf", mm->doubles[x]);
        strcat(printer, temp);
        clear(temp);
    }
    //FIX ME
    return printer;
}
/****END PRINTERS****/

/****SORTERS FIX ME***/

void sort_int(Int m)
{
    CompareFunc compareInts = _____compare_ints;
    qsort(m->ints, m->size, sizeof(long long *), compareInts);
}

void sort_double(Double d)
{
    CompareFunc compareDoubles = _____compare_doubles;
    qsort(d->doubles, d->size, sizeof(long double *), compareDoubles);
}
/***END SORTERS***/
