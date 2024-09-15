#ifndef DATASET_H
#define DATASET_H

#include <stddef.h>

typedef struct {
    size_t start;
    size_t len;
    size_t max_len;
    double *tab;
} Dataset;

// Function prototypes
Dataset *dataset_init(size_t initial_size);

void dataset_destroy(Dataset *ds);

void dataset_append(Dataset *ds, double value);

double dataset_return_element(Dataset *ds, size_t index);

void dataset_resize(Dataset *ds, size_t new_size);

void dataset_delete(Dataset *ds, size_t index);

#endif
