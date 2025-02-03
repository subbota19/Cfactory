# Cfactory

As part of my C journey (Modern C book), I have developed a simple Dataset structure. This project provides fundamental dataset operations in C and enables interoperability with Python through Cython compilation.

## Features

Dynamic Memory Management: Uses malloc, realloc, calloc, and free for efficient memory allocation.

Schema-Based Design: Supports a predefined schema for structured data handling.

Row Operations: Add, remove, and retrieve rows dynamically.

Automatic Memory Expansion & Shrinking: Expands storage when full and shrinks when utilization is low.

Cython Compatibility: Can be accessed and utilized within Python via Cython

## Usage

For compilation please take a look at the Makefile. 
For Python usage, use test_dataset.py 
