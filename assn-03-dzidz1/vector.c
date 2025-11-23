#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    assert(elemSize > 0);
    v->elem_size = elemSize;
    v->log_len = 0;
    if(initialAllocation == 0) {
        v->alloc_len = 4;
    } else {
        v->alloc_len = initialAllocation;
    }
    v->free_fn = freeFn;
    v->elems = malloc(elemSize * v->alloc_len);
    assert(v->elems != NULL);
}

void VectorDispose(vector *v)
{   
    if(v->free_fn != NULL) {
        for(int i = 0; i < v->log_len; i++) {
            void *elem_ptr = (char *)v->elems + v->elem_size * i;
            v->free_fn(elem_ptr);
        }
    }
    free(v->elems);
}

int VectorLength(const vector *v)
{
    return v->log_len;
}

void *VectorNth(const vector *v, int position)
{
    assert(position >= 0 && position < v->log_len);
    void *elem_ptr = (char *)v->elems + v->elem_size * position;
    return elem_ptr;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0 && position < v->log_len);
    void *elem_ptr = (char *)v->elems + v->elem_size * position;
    if(v->free_fn != NULL) v->free_fn(elem_ptr);
    memcpy(elem_ptr, elemAddr, v->elem_size);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0 && position <= v->log_len);
    if(v->log_len == v->alloc_len) {
        v->alloc_len *= 2;
        void *new_elems = realloc(v->elems, v->alloc_len * v->elem_size);
        assert(new_elems != NULL);
        v->elems = new_elems;
    }
    void *old_ptr = (char *)v->elems + v->elem_size * position;
    int delta = v->log_len - position;
    if(delta > 0) {
        memmove((char *)old_ptr + v->elem_size, old_ptr, v->elem_size * delta);
    } 
    memcpy(old_ptr, elemAddr, v->elem_size);
    v->log_len++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
    VectorInsert(v, elemAddr, v->log_len);
}

void VectorDelete(vector *v, int position)
{
    assert(position >= 0 && position < v->log_len);
    void *elem_ptr = (char *)v->elems + v->elem_size * position;
    if(v->free_fn != NULL) v->free_fn(elem_ptr);
    int delta = v->log_len - position - 1;
    if(delta > 0) {
        memmove(elem_ptr, (char *)elem_ptr + v->elem_size, v->elem_size * delta);
    }
    v->log_len--;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
    assert(compare != NULL);
    qsort(v->elems, v->log_len, v->elem_size, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    assert(mapFn != NULL);
    for(int i = 0; i < v->log_len; i++) {
        void *elem_ptr = (char *)v->elems + v->elem_size * i;
        mapFn(elem_ptr, auxData);
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
    assert(startIndex >= 0 && startIndex <= v->log_len);
    assert(searchFn != NULL);
    void *start = (char *)v->elems + v->elem_size * startIndex;
    size_t num_elements = v->log_len - startIndex;
    if(isSorted) {
        void *found = bsearch(key, start, num_elements, v->elem_size, searchFn);
        if(found == NULL) return kNotFound;
        return ((char *)found - (char *)v->elems) / v->elem_size;
    } else {
        for (int i = startIndex; i < v->log_len; i++) {
            void *elem_ptr = (char *)v->elems + i * v->elem_size;
            if(searchFn(key, elem_ptr) == 0) { 
                return i;
            }
        }
    }
    return kNotFound;
} 
