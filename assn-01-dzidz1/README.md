# Random Sentence Generator (RSG)

A C++ implementation of a random sentence generator that uses context-free grammar files to generate random sentences based on defined production rules.

## Overview

This project parses grammar definition files and generates random sentences by recursively expanding production rules. It's designed with proper memory management and validated using memory leak detection tools.

## Project Structure

```
.
├── data/               # Grammar definition files
│   ├── bionic.g
│   ├── bond.g
│   ├── haiku.g
│   ├── insult.g
│   └── ...
├── definition.cc/h     # Grammar definition handling
├── production.cc/h     # Production rule implementation
├── random.cc/h         # Random number generation utilities
├── rsg.cc              # Main program
└── Makefile            # Build configuration
```

## Build Requirements

- C++ compiler (g++)
- valgrind (for memory checking)

### Installing valgrind

**Ubuntu:**

```sh
sudo apt-get install valgrind
```

**Arch Linux:**

```sh
yay -S valgrind
```

## Building

```sh
make
```

## Usage

Run the generator with a grammar file:

```sh
./rsg data/bionic.g
```

## Testing

**Single test:**

```sh
./rsg data/bionic.g
```

**Memory leak check:**

```sh
./rsgChecker32 ./rsg data/bionic.g
./rsgChecker64 ./rsg data/bionic.g
```

**All tests:**

```sh
make test_all
```

## Grammar Files

The `data/` directory contains various grammar files that define different sentence generation patterns, including haikus, insults, Star Trek dialogue, and more.
