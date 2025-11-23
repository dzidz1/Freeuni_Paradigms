#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
	HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
	assert(elemSize > 0);
	assert(numBuckets > 0);
	assert(hashfn != NULL);
	assert(comparefn != NULL);
	h->num_buckets = numBuckets;
	h->hash_fn = hashfn;
	h->compare_fn = comparefn;
	h->free_fn = freefn;
	h->elem_size = elemSize;
	h->elems = malloc(numBuckets * sizeof(vector));
	assert(h->elems != NULL);
	for(int i = 0; i < numBuckets; i++) {
		vector *vec_ptr = (vector *)((char *)h->elems + i * sizeof(vector));
		VectorNew(vec_ptr, elemSize, freefn, 4);
	}
}

void HashSetDispose(hashset *h)
{
	for(int i = 0; i < h->num_buckets; i++) {
		vector *vector_ptr = (vector *)((char *)h->elems + i * sizeof(vector));
		VectorDispose(vector_ptr);
	}
	free(h->elems);
	h->elems = NULL;
}

int HashSetCount(const hashset *h)
{
	int cnt = 0;
	for(int i = 0; i < h->num_buckets; i++) {
		vector *vector_ptr = (vector *)((char *)h->elems + i * sizeof(vector));
		cnt += VectorLength(vector_ptr);
	}
	return cnt;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
	assert(mapfn != NULL);
	for(int i = 0; i < h->num_buckets; i++) {
		vector *vector_ptr = (vector *)((char *)h->elems + i * sizeof(vector));
		VectorMap(vector_ptr, mapfn, auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
	assert(elemAddr != NULL);
	int bucket_num = h->hash_fn(elemAddr, h->num_buckets);
	assert(bucket_num >= 0 && bucket_num < h->num_buckets);
	vector *vector_ptr = (vector *)((char *)h->elems + bucket_num * sizeof(vector));
	int idx = VectorSearch(vector_ptr, elemAddr, h->compare_fn, 0, false);
	if (idx != -1) {
		VectorReplace(vector_ptr, elemAddr, idx);
	} else {
		VectorAppend(vector_ptr, elemAddr);
	}
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{
	assert(elemAddr != NULL);
	int bucket_num = h->hash_fn(elemAddr, h->num_buckets);
	assert(bucket_num >= 0 && bucket_num < h->num_buckets);
	vector *vector_ptr = (vector *)((char *)h->elems + bucket_num * sizeof(vector));
	int idx = VectorSearch(vector_ptr, elemAddr, h->compare_fn, 0, false);
	if(idx == -1) return NULL;
	return VectorNth(vector_ptr, idx);
}
