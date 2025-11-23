#include "counting_array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void CountingArrayInit(CountingArray* a, int elem_size, CmpFn cmp_fn, FreeFn free_fn) {
	assert(elem_size != NULL);
	assert(cmp_fn != NULL);
	a->elem_size = elem_size;
	a->log_len = 0;
	a->all_together = 0;
	a->alloc_len = 4;
	a->root_uniques = malloc(a->root_uniques, a->elem_size * a->alloc_len);
	a->root_all = malloc(a->root_all, a->elem_size * a->alloc_len);
	assert(a != NULL);
	a->CmpFn = cmp_fn;
	a->FreeFn = free_fn;
}

void CountingArrayDestroy(CountingArray* a) {
	if (a->FreeFn != NULL) {
		for (int i = 0; i < a->log_len; i++) {
			void* remove = (char*)a->root_uniques + i * a->elem_size;
			a->FreeFn(remove);
		}

		for (int i = 0; i < a->all_together; i++) {
			void* remove = (char*)a->root_all + i * a->elem_size;
			a->FreeFn(remove);
		}
	}
	free(a->root_all);
	free(a->root_uniques);
}

int CountingArraySize(CountingArray* a) {
  return a->log_len;
}

void* CountingArrayGet(CountingArray* a, int index) {
	assert(index >= 0 && index < a->log_len);
	void* at_index = (char*)a->root_uniques + index * a->elem_size;
	return at_index;
}

bool CountingArrayInsert(CountingArray* a, void* elem) {
	if (a->log_len == a->alloc_len) {
		a->alloc_len *= 2;
		a->root_uniques = realloc(a->root_uniques, a->alloc_len * a->elem_size);
		assert(a->root_uniques != NULL);
	// }

	// if (a->all_together == a->alloc_len) {
	// 	a->alloc_len *= 2;
		a->root_all = realloc(a->root_all, a->alloc_len * a->elem_size);
		assert(a->root_all != NULL);
	}

	bool contains = false;
	for (int i = 0; i < a->log_len; i++) {
		void* curr_elem = (char*)a->root_uniques + i * a->elem_size;
		if (a->CmpFn(curr_elem, elem) == 0) {
			contains = true;
			break;
		}
	}
	if (contains == false) {
		int i;
		for (i = 0; a->CmpFn((void*)((char*)a->root_uniques + i * a->elem_size), elem) < 0 && i < a->log_len; i++) {};
		void* old_ptr = (char*)a->root_uniques + i * a->elem_size;
		int delta = a->log_len - i;
		if (delta > 0) {
			memmove(old_ptr + a->elem_size, old_ptr, a->elem_size * delta);
			memcpy(old_ptr, elem, a->elem_size);
		}
		else {
			memcpy((void*)((char*)a->root_uniques + a->elem_size * a->log_len), elem, a->elem_size);
		}
		a->log_len++;
	}
	else {
		int i;
		for (i = 0; a->CmpFn((void*)((char*)a->root_all + i * a->elem_size), elem) < 0 && i < a->all_together; i++) {};
		void* old_ptr = (char*)a->root_all + i * a->elem_size;
		int delta = a->log_len - i;
		if (delta > 0) {
			memmove((void*)((char*)old_ptr + a->elem_size), old_ptr, a->elem_size * delta);
			memcpy(old_ptr, elem, a->elem_size);
		}
		else {
			memcpy((void*)((char*)a->root_uniques + a->elem_size * a->all_together), elem, a->elem_size);
		}
	}
	a->all_together++;

}

void CountingArrayRemove(CountingArray* a, void* elem) {
	bool contains = false;
	for (int i = 0; i < a->log_len; i++) {
		void* curr = (char*)a->root_uniques + a->elem_size * i;
		if (a->CmpFn(curr, elem) == 0) {
			contains = true;
			break;
		}
	}
	if (contains) { int i;
		for (i = 0; a->CmpFn((void*)((char*)a->root_uniques + i * a->elem_size), elem) < 0 && i < a->log_len; i++) {};
		void* should_remove = (char*)a->root_uniques + i * a->elem_size;
		if (a->FreeFn != NULL) a->FreeFn(should_remove);
		int delta = a->log_len - i - 1;
		if (delta > 0) {
			memmove((char*)a->root_uniques + i * a->elem_size, (void*)((char*)should_remove + a->elem_size), delta * a->elem_size);
		}
		else {
			memmove(a->root_uniques, (void*)((char*)a->root_uniques + a->elem_size), delta * a->elem_size);
		}
		a->log_len--;


		for (i = 0; a->CmpFn((void*)((char*)a->root_all + i * a->elem_size), elem) < 0 && i < a->all_together; i++) {};
		void* should_remove = (char*)a->root_all + i * a->elem_size;
		if (a->FreeFn != NULL) a->FreeFn(should_remove);
		int delta = a->all_together - i - 1;
		if (delta > 0) {
			memmove((void*)((char*)a->root_all + i * a->elem_size), (void*)((char*)should_remove + a->elem_size), delta * a->elem_size);
		}
		else {
			memmove(a->root_all, (void*)((char*)a->root_all + a->elem_size), delta * a->elem_size);
		}
		a->all_together--;
	}


}

void CountingArrayMerge(CountingArray* a, CountingArray* o) {
  // IMPLEMENT
}