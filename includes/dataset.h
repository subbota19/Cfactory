#ifndef DATASET_H
#define DATASET_H

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
} DataType;

typedef struct {
    char *name;
    DataType type;
    void *data;
} Field;

typedef struct {
    Field *fields;
} Row;

typedef struct {
    size_t start;
    size_t len;
    size_t max_len;
    double *tab;
} Dataset;

typedef struct {
    Field *schema;
    size_t num_fields;
    Row *rows;
    size_t num_rows;
    size_t capacity;
} DatasetV2;


Dataset *dataset_init(size_t initial_size);

DatasetV2 *create_dataset(const Field *schema, size_t num_fields, size_t capacity);

void add_row(DatasetV2 *ds, void **values);

void free_dataset(DatasetV2 *ds);

void dataset_destroy(Dataset *ds);

void dataset_append(Dataset *ds, double value);

double dataset_return_element(Dataset *ds, size_t index);

void dataset_resize(Dataset *ds, size_t new_size);

void dataset_delete(Dataset *ds, size_t index);

#endif
