# Custom Vector and HashSet Implementation in C

A C implementation of generic dynamic array (vector) and hash table (hashset) data structures with type-agnostic interfaces using void pointers and function pointers.

## Overview

This project implements fundamental data structures in C without relying on C++ templates or standard library containers. It demonstrates:

- Generic programming in C using void pointers
- Custom memory management
- Hash table collision resolution
- Function pointers for type-specific operations

## Project Structure

```
.
├── vector.c/h              # Dynamic array implementation
├── hashset.c/h             # Hash table implementation
├── vectortest.c            # Vector test suite
├── hashsettest.c           # HashSet test suite
├── thesaurus-lookup.c      # Example application using both structures
├── streamtokenizer.c/h     # String tokenization utilities
├── data/
│   └── thesaurus.txt       # Thesaurus data file
└── Makefile                # Build configuration
```

## Features

### Vector

- Generic dynamic array that can store any data type
- Automatic resizing when capacity is reached
- Support for custom comparison and free functions
- Type-safe operations through function pointers

### HashSet

- Generic hash table implementation
- Configurable hash function and comparison function
- Collision handling
- Dynamic resizing based on load factor

### Thesaurus Lookup

A practical application demonstrating both data structures:

- Loads a thesaurus into a hashset
- Stores synonym sets in vectors
- Provides fast lookup of word synonyms

## Building

```sh
make
```

The first build will automatically download the required data files.

### Rebuilding Data Files

If data files become corrupted:

```sh
rm -rf data/
make data
```

## Testing

**Run vector tests:**

```sh
./vector-test
```

**Run hashset tests:**

```sh
./hashset-test
```

**Compare output with expected results:**

```sh
./vector-test | diff sample-output-vector.txt -
./hashset-test | diff sample-output-hashset.txt -
```

If the `diff` command produces no output, your implementation matches the expected behavior.

**Run thesaurus application:**

```sh
./thesaurus-lookup
```

## Implementation Details

### Generic Programming in C

Both data structures use void pointers to achieve type genericity:

```c
typedef struct {
  void *elems;
  int elemSize;
  int logicalLength;
  int allocatedLength;
  VectorFreeFunction freeFn;
} vector;
```

Users provide:

- Element size (in bytes)
- Comparison function for searching
- Free function for cleanup (if elements contain dynamically allocated memory)
- Hash function (for hashset)

### Memory Management

The implementations handle:

- Dynamic memory allocation and reallocation
- Deep copying of elements
- Proper cleanup of dynamically allocated elements
- Prevention of memory leaks

## Example Usage

### Vector

```c
vector v;
VectorNew(&v, sizeof(int), NULL, 4);
int value = 42;
VectorAppend(&v, &value);
VectorDispose(&v);
```

### HashSet

```c
hashset h;
HashSetNew(&h, sizeof(int), 10, IntHash, IntCompare, NULL);
int value = 42;
HashSetEnter(&h, &value);
HashSetDispose(&h);
```

## Learning Outcomes

This project demonstrates:

- Low-level memory management in C
- Generic programming without language-level generics
- Hash table implementation and collision resolution
- Function pointers for polymorphism
- Proper resource cleanup and memory leak prevention
