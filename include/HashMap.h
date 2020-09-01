#ifndef _HASH_MAP_
#define _HASH_MAP_
#include "LinkedListAPI.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct htable
{
    List** table;
    int numEntries;
    unsigned long tableSize;

}; typedef struct htable HashMap;

HashMap* newMap(unsigned long tableSize, char* (*printFunc)(void *), void (*delFunc)(void *), int (*compareFunc)(const void*, const void*));

//returns an index hashed from the key
unsigned long hash(HashMap* h, char* key);

void insertToMap(HashMap* h, void* data);

void removeFromMap(HashMap* h, void* data);

void deleteHashMap(HashMap* h);

bool containsData(HashMap* h, void* data);
#endif
