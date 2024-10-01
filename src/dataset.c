#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"

// malloc creates a storage instance for us on the fly
// free can annihilates storage
// calloc (clear allocate) sets all bits of the new storage to 0
// realloc grows or shrinks storage
// aligned_alloc ensures nondefault alignment.

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

void free_row(const DatasetV2 *ds, const size_t index) {
    const Row *row = &ds->rows[index];
    for (size_t i = 0; i < ds->num_fields; ++i) {
        free(row->fields[i].data);
    }
    free(row->fields);
}

void shrink_capacity(DatasetV2 *ds) {
    if (ds->num_rows < ds->capacity / 2) {
        ds->capacity /= 2; // Shrink capacity by half
        ds->rows = realloc(ds->rows, sizeof(Row) * ds->capacity);
        if (!ds->rows) {
            perror("Failed to shrink memory");
            exit(EXIT_FAILURE);
        }
        printf("Capacity shrunk to %zu\n", ds->capacity);
    }
}

int remove_row_by_index(DatasetV2 *ds, const size_t index) {
    if (index >= ds->num_rows) {
        return -1;
    }

    free_row(ds, index);

    for (size_t i = index; i < ds->num_rows - 1; ++i) {
        ds->rows[i] = ds->rows[i + 1];
    }

    ds->num_rows--;

    shrink_capacity(ds);

    return 0;
}

void print_row(const DatasetV2 *ds, const size_t index) {
    const Row *row = get_row_by_index(ds, index);

    if (row != NULL) {
        for (size_t i = 0; i < ds->num_fields; ++i) {
            const Field *schema = &ds->schema[i];

            printf("%s: ", schema->name);

            switch (schema->type) {
                case TYPE_INT:
                    printf("%d\t", *(int *) row->fields[i].data);
                    break;
                case TYPE_FLOAT:
                    printf("%f\t", *(float *) row->fields[i].data);
                    break;
                case TYPE_STRING:
                    printf("%s\t", (char *) row->fields[i].data);
                    break;
                default:
                    printf("Unknown Type\t");
                    break;
            }

        }
        printf("\n");
    }
}

Row *get_row_by_index(const DatasetV2 *ds, const size_t index) {
    if (index < ds->num_rows) {
        return &ds->rows[index];
    }
    return NULL;
}

size_t get_num_rows(const DatasetV2 *ds) {
    return ds->num_rows;
}
