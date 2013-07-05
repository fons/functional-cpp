
# Functional Programming in C++

## Overview

This repository contains my attempt to explore basic functional programming in C++. 
It builds on the availability of λ's and variadic templates (among other things).

One self-imposed limitation was to avoid `weird`syntax or to overload operators away from their semantics.


### Why
It's a great way to explore functional programming and C++, neither of which are well served by this approach.

I primarily use it as a test bench and don't intend to turn this into some sort of library.


###Build

1. Use the latest version of gcc.I've build this with gcc 4.8 on mac osx 10.8.4.
2. Clone the repo.
3. Run make in the src directory.
4. run ../bin/main or ../bin/main all
5. ../bin/main is a very primitive 'repl' which allows you to run the test functions.

### What

#### ./src

The main file in the source directory contains a 'repl' and a set of registered test functions.

The .cpp files contain simple example code for the functional concepts in the header.
So bracket.cpp contains example code for bracket.hpp and bracket_helper.hpp

#### ./docs

This directory contains .tex files. These document provide more background for the code. 
pp-fp-1.tex discusses basic functional programming available in C++. pp-fp-2.tex discusses more advanced concepts.

pp-fp-1.tex was submitted to pragpub for publication.

### Related Work

## Basic Functional Programming

#### Functors
#### Applicative Functors

#### Monads

### Helpful Hints


