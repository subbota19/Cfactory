# dataset.pyx

# distutils: language = c

cdef extern from "dataset.h" nogil:
    # Enumerations
    ctypedef enum FieldType:
        TYPE_INT
        TYPE_FLOAT
        TYPE_STRING

    # Structures
    ctypedef struct Field:
        char *name
        FieldType type
        void *data

    ctypedef struct Row:
        Field *fields

    ctypedef struct DatasetV2:
        Field *schema
        size_t num_fields
        size_t num_rows
        size_t capacity
        Row *rows

    # Function declarations
    DatasetV2 *create_dataset(const Field *schema, const size_t num_fields, const size_t capacity)
    void add_row(DatasetV2 *ds, void **values)
    void free_dataset(DatasetV2 *ds)
    int remove_row_by_index(DatasetV2 *ds, const size_t index)
    void print_row(const DatasetV2 *ds, const size_t index)
    Row *get_row_by_index(const DatasetV2 *ds, const size_t index)
    size_t get_num_rows(const DatasetV2 *ds)

from libc.stdlib cimport malloc, free, realloc, exit
from libc.string cimport strdup


# Python-side wrapper classes
cdef class PyDataset:
    cdef DatasetV2 *c_ds

    def __cinit__(self, schema, capacity=10):
        """
        Initialize the Dataset.

        :param schema: List of tuples [(name: str, type: str), ...]
        :param capacity: Initial capacity of the dataset
        """
        cdef size_t num_fields = len(schema)
        cdef Field *c_schema = <Field *> malloc(sizeof(Field) * num_fields)
        if not c_schema:
            raise MemoryError("Failed to allocate memory for schema")

        for i in range(num_fields):
            name, type_str = schema[i]
            c_schema[i].name = strdup(name.encode('utf-8'))
            if type_str == 'int':
                c_schema[i].type = TYPE_INT
            elif type_str == 'float':
                c_schema[i].type = TYPE_FLOAT
            elif type_str == 'string':
                c_schema[i].type = TYPE_STRING
            else:
                c_schema[i].type = TYPE_STRING  # Default to string
            c_schema[i].data = NULL

        self.c_ds = create_dataset(c_schema, num_fields, capacity)
        free(c_schema)  # Assuming create_dataset copies the schema

    def __dealloc__(self):
        if self.c_ds != NULL:
            free_dataset(self.c_ds)

    def print_row(self, index):
        """
        Print a row to the console.

        :param index: Index of the row
        """
        print_row(self.c_ds, index)
