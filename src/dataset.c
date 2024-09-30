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
    Dataset *ds = malloc(sizeof(Dataset));
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

DatasetV2 *create_dataset(const Field *schema, const size_t num_fields, const size_t capacity) {
    DatasetV2 *ds = malloc(sizeof(DatasetV2));
    if (!ds) {
        perror("Failed to allocate memory for Dataset");
        exit(EXIT_FAILURE);
    }

    ds->schema = malloc(sizeof(Field) * num_fields);
    if (!ds->schema) {
        perror("Failed to allocate memory for Schema");
        free(ds);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < num_fields; ++i) {
        // allocates memory for the new string dynamically
        ds->schema[i].name = strdup(schema[i].name);
        ds->schema[i].type = schema[i].type;
        ds->schema[i].data = NULL;
    }

    ds->num_fields = num_fields;
    ds->num_rows = 0;
    ds->capacity = capacity;
    ds->rows = malloc(sizeof(Row) * ds->capacity);

    return ds;
}

void add_row(DatasetV2 *ds, void **values) {
    if (ds->num_rows >= ds->capacity) {
        ds->capacity *= 2;
        ds->rows = realloc(ds->rows, sizeof(Row) * ds->capacity);
        if (!ds->rows) {
            perror("Failed to reallocate memory for Row");
            exit(EXIT_FAILURE);
        }
    }

    Row *row = &ds->rows[ds->num_rows];
    row->fields = malloc(sizeof(Field) * ds->num_fields);
    if (!row->fields) {
        perror("Failed to allocate memory for Row");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < ds->num_fields; ++i) {
        row->fields[i].name = ds->schema[i].name;
        row->fields[i].type = ds->schema[i].type;
        switch (row->fields[i].type) {
            case TYPE_INT:
                row->fields[i].data = malloc(sizeof(int));
                if (!row->fields[i].data) {
                    perror("Failed to allocate memory for Field");
                    exit(EXIT_FAILURE);
                }
                *(int *) row->fields[i].data = *(int *) values[i];
                break;
            case TYPE_FLOAT:
                row->fields[i].data = malloc(sizeof(float));
                if (!row->fields[i].data) {
                    perror("Failed to allocate memory for Field");
                    exit(EXIT_FAILURE);
                }
                *(float *) row->fields[i].data = *(float *) values[i];
                break;
            case TYPE_STRING:
                row->fields[i].data = strdup(values[i]);
                if (!row->fields[i].data) {
                    perror("Failed to allocate memory for Field");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                row->fields[i].data = NULL;
                break;
        }
    }

    ds->num_rows++;
}

void free_dataset(DatasetV2 *ds) {
    for (size_t i = 0; i < ds->num_fields; ++i) {
        free(ds->schema[i].name);
    }
    free(ds->schema);

    for (size_t i = 0; i < ds->num_rows; ++i) {
        const Row *row = &ds->rows[i];
        for (size_t j = 0; j < ds->num_fields; ++j) {
            free(row->fields[j].data);
        }
        free(row->fields);
    }
    free(ds->rows);
    free(ds);
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
