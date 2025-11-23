# Where Am I? - Trilateration in Scheme

A functional programming implementation of a trilateration algorithm in Scheme to determine location based on distances from multiple reference points (e.g., cell towers, WiFi access points, GPS satellites).

## Overview

This project demonstrates:

- Functional programming paradigm in Scheme
- Geometric algorithms for location estimation
- List processing and higher-order functions
- Computational geometry concepts
- Pure functional approach without side effects

## Project Structure

```
.
├── where-am-i.scm       # Main trilateration implementation
├── scheme-examples.scm  # Helper examples and utilities
└── README.md
```

## Features

The implementation includes several key functions:

- **`intersection-points`**: Finds intersection points of two circles
- **`distance-product`**: Calculates weighted distance from candidate point to reference points
- **`rate-points`**: Scores candidate locations based on fit to distance measurements
- **`sort-points`**: Orders points by their quality scores
- **`clumped-points`**: Identifies clusters of candidate points
- **`average-point`**: Computes centroid of a point cluster
- **`best-estimate`**: Returns the most likely location estimate
- **`where-am-i`**: Main function that estimates location from tower distances

## Algorithm

The trilateration process:

1. **Circle Intersection**: For each pair of reference points (towers), compute intersection points of circles centered at those points with radii equal to measured distances
2. **Candidate Generation**: Generate all possible intersection points as location candidates
3. **Scoring**: Rate each candidate based on how well it fits all distance measurements
4. **Clustering**: Group nearby candidates to find consensus regions
5. **Estimation**: Return the centroid of the best cluster as the final location estimate

## Requirements

- Kawa Scheme interpreter

## Testing

### Run All Tests

```sh
kawa

(load "where-am-i.scm")
(test-all)
```

### Run Individual Tests

```sh
kawa

(load "where-am-i.scm")
(test-intersection-points)
(test-distance-product)
(test-rate-points)
(test-sort-points)
(test-clumped-points)
(test-average-point)
(test-best-estimate)
(test-where-am-i)
```

## Example Usage

```scheme
; Define tower locations and distances
(define towers '((0 0) (10 0) (5 8)))
(define distances '(5 7 3))

; Estimate location
(where-am-i towers distances)
; Returns estimated (x y) coordinates
```

## Implementation Approach

The solution follows functional programming principles:

- **Immutability**: No mutation of data structures
- **Pure Functions**: No side effects, output depends only on inputs
- **Higher-Order Functions**: Heavy use of `map`, `filter`, `fold`
- **Recursion**: Iteration through recursive function calls
- **List Processing**: Scheme lists as primary data structure

## Key Concepts

### Trilateration vs. Triangulation

- **Trilateration**: Uses distances to determine position (this project)
- **Triangulation**: Uses angles to determine position

### Geometric Primitives

- Circle-circle intersection
- Distance calculations
- Point clustering
- Centroid computation

### Functional Patterns

- List comprehensions via `map`
- Filtering with predicates
- Reduction operations with `fold`
- Function composition

## Learning Outcomes

This project demonstrates:

- Functional programming in a Lisp dialect
- Solving real-world geometric problems functionally
- Working with recursive data structures
- Algorithm design without mutable state
- Mathematical computations in Scheme
- Test-driven development in functional languages

## Applications

Trilateration is used in:

- GPS navigation systems
- Cell phone location services
- WiFi positioning systems
- Indoor navigation
- Robot localization
