#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"

// malloc creates a storage instance for us on the fly
// free can annihilates storage
// calloc (clear allocate) sets all bits of the new storage to 0
// realloc grows or shrinks storage
// aligned_alloc ensures nondefault alignment.

// Function to initialize the dataset
Dataset *dataset_init(size_t initial_size) {
    Dataset *ds = (Dataset *) malloc(sizeof(Dataset));
    if (ds == NULL) {
        perror("Failed to allocate memory for dataset");
        exit(EXIT_FAILURE);
    }

    ds->start = 0;
    ds->len = 0;
    ds->max_len = initial_size;
    ds->tab = (double *) malloc(initial_size * sizeof(double));

    if (ds->tab == NULL) {
        perror("Failed to allocate memory for tab");
        free(ds);
        exit(EXIT_FAILURE);
    }

    return ds;
}

// Function to destroy the dataset and free memory
void dataset_destroy(Dataset *ds) {
    if (ds) {
        free(ds->tab);
        free(ds);
    }
}

// Function to append a new element to the dataset
void dataset_append(Dataset *ds, double value) {
    if (ds->len >= ds->max_len) {
        ds->max_len *= 2;
        ds->tab = (double *) realloc(ds->tab, ds->max_len * sizeof(double));
        if (ds->tab == NULL) {
            perror("Failed to reallocate memory for tab");
            exit(EXIT_FAILURE);
        }
    }
    ds->tab[ds->len++] = value;
}

// Function to return an element by index
double dataset_return_element(Dataset *ds, size_t index) {
    if (index < ds->len) {
        return ds->tab[index];
    } else {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
}

// Function to resize the dataset manually
void dataset_resize(Dataset *ds, size_t new_size) {
    if (new_size < ds->len) {
        ds->len = new_size;
    }
    ds->tab = (double *) realloc(ds->tab, new_size * sizeof(double));
    if (ds->tab == NULL) {
        perror("Failed to reallocate memory for resizing");
        exit(EXIT_FAILURE);
    }
    ds->max_len = new_size;
}

// Function to delete an element by index (shifting the remaining elements)
void dataset_delete(Dataset *ds, size_t index) {
    if (index < ds->len) {
        memmove(&ds->tab[index], &ds->tab[index + 1], (ds->len - index - 1) * sizeof(double));
        ds->len--;
    } else {
        fprintf(stderr, "Index out of bounds for deletion\n");
        exit(EXIT_FAILURE);
    }
}
