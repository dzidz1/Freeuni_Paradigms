# Six Degrees of Separation - IMDB Edition

A C++ implementation of the "Six Degrees of Kevin Bacon" game using IMDB data. This program finds the shortest path between any two actors through their shared movie appearances.

## Overview

This project implements a breadth-first search algorithm to find connections between actors in the IMDB database. It demonstrates efficient graph traversal, binary data file parsing, and memory-efficient data structure usage.

## Project Structure

```
.
├── data/                    # IMDB database files
│   ├── actresses-only/      # Female actors subset
│   ├── big-endian/          # Big-endian binary data
│   ├── little-endian/       # Little-endian binary data
│   └── updated/             # Updated dataset
├── imdb.cc/h                # IMDB database interface
├── imdb-utils.h             # Utility functions
├── path.cc/h                # Path finding implementation
├── six-degrees.cc           # Main program
├── imdb-test.cc             # Testing suite
└── Makefile                 # Build configuration
```

## Features

- Finds shortest connection path between any two actors
- Handles both big-endian and little-endian binary data formats
- Includes multiple datasets (full IMDB, actresses-only, updated)
- Memory-efficient implementation validated with valgrind

## Building

```sh
make
```

The first build will automatically download and set up the required data files.

### Rebuilding Data Files

If data files become corrupted, rebuild them with:

```sh
rm -rf data/
make data
```

## Usage

Run the program interactively:

```sh
./six-degrees
```

The program will prompt you to enter two actor names and will find the shortest connection path between them.

## Testing

**Run test suite:**

```sh
./imdb-test
```

**Memory leak check:**

```sh
./six-degrees-checker64 ./six-degrees -m
```

**Full validation:**

```sh
./six-degrees-checker64 ./six-degrees
```

## How It Works

1. Parses binary IMDB data files containing actor and movie information
2. Uses breadth-first search (BFS) to find the shortest path between actors
3. Each edge in the graph represents a shared movie appearance
4. Returns the chain of actors and movies connecting the two input actors

## Data Format

The project includes three dataset variations:

- **Full dataset**: Complete IMDB data
- **Actresses-only**: Subset containing only female actors
- **Updated**: Most recent dataset version

Each dataset is available in both big-endian and little-endian formats for cross-platform compatibility.
