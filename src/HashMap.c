#include "HashMap.h"
#include "LinkedListAPI.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>




HashMap* newMap(unsigned long tableSize, char* (*printFunc)(void *), void (*delFunc)(void *), int (*compareFunc)(const void*, const void*))
{
    HashMap* h = calloc(1, sizeof(HashMap));
    h->numEntries = 0;
    h->table = calloc(tableSize, sizeof(List*));
    h->tableSize = tableSize;
    for(int i = 0; i < tableSize; i++)
    {
        h->table[i] = initializeList(printFunc, delFunc, compareFunc);
    }
    return h;

}
//hash the key
unsigned long hash(HashMap* h, char* key)
{
    int hash = 401;

	while (*key != '\0') {
		hash = ((hash << 4) + (int)(*key)) % h->tableSize;
		key++;
	}

	return hash % h->tableSize;
}

void insertToMap(HashMap* h, void* data)
{

    if(containsData(h, data)==true)return;
    unsigned long idx = hash(h, data);
    insertBack(h->table[idx], data);
}
void removeFromMap(HashMap* h, void* data)
{
    if(containsData(h, data)==false)return;
    unsigned long idx = hash(h, data);
    List* l = h->table[idx];
    l->deleteData(data);
    return;
}
bool containsData(HashMap* h, void* data)
{
    unsigned long idx = hash(h, data);
    List* l = h->table[idx];
    for(Node* n = l->head; n != NULL; n = n->next)
    {
        if(l->compare(n->data, data)==0)return true;
    }
    return false;

}

void deleteHashMap(HashMap* h)
{
    for(int i = 0; i < h->tableSize; i++)
    {
        freeList(h->table[i]);
    }
    free(h->table);
    free(h);
}


