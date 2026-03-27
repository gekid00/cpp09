# CPP Module 09

42 school project focusing on the practical use of STL containers in C++98.
Each exercise requires a specific container chosen for its properties, reinforcing
the importance of selecting the right data structure for each problem.

## Technologies

- C++98
- STL containers (`std::map`, `std::stack`, `std::vector`, `std::deque`)
- Makefile

## Exercises

### ex00 -- Bitcoin Exchange

Reads a CSV database of historical Bitcoin prices and an input file of dates and values.
For each entry, it finds the closest earlier exchange rate using `std::map` and computes the result.

### ex01 -- Reverse Polish Notation

Evaluates mathematical expressions written in RPN using `std::stack`.
Supports `+`, `-`, `*`, `/` operators on single-digit operands.

### ex02 -- PmergeMe

Implements the Ford-Johnson merge-insert sort algorithm on both `std::vector` and `std::deque`,
then compares their performance. Uses the Jacobsthal sequence to optimize insertion order.

## Build

Each exercise is compiled independently:

```bash
cd ex00 && make      # builds ./btc
cd ex01 && make      # builds ./RPN
cd ex02 && make      # builds ./PmergeMe
```

`make clean`, `make fclean`, and `make re` are available in each directory.

## Usage

```bash
# ex00 -- Bitcoin Exchange
./btc input.txt

# ex01 -- Reverse Polish Notation
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"

# ex02 -- PmergeMe
./PmergeMe 3 5 9 7 4
```

## Key Concepts

- STL container selection based on access patterns and performance constraints
- Ford-Johnson (merge-insert) sorting algorithm with Jacobsthal sequence optimization
- Binary search insertion (`std::lower_bound`) for efficient sorted insertion
- Orthodox Canonical Form for all classes
