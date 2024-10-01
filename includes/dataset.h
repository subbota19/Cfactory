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


DatasetV2 *create_dataset(const Field *schema, size_t num_fields, size_t capacity);

void add_row(DatasetV2 *ds, void **values);

void free_dataset(DatasetV2 *ds);

void shrink_capacity(DatasetV2 *ds);

int remove_row_by_index(DatasetV2 *ds, size_t index);

Row *get_row_by_index(const DatasetV2 *ds, size_t index);

size_t get_num_rows(const DatasetV2 *ds);

void print_row(const DatasetV2 *ds, size_t index);


#endif
