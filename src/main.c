#include <stdio.h>
#include "dataset.h"

#define DEFAULT_CAPACITY 10

int main() {
    const Field schema[] = {
        {"id", TYPE_INT, NULL},
        {"name", TYPE_STRING, NULL},
        {"score", TYPE_FLOAT, NULL}
    };
    const size_t num_fields = sizeof(schema) / sizeof(Field);
    DatasetV2 *ds = create_dataset(schema, num_fields, DEFAULT_CAPACITY);

    int id1 = 1;
    char *name1 = "Alice";
    float score1 = 85.5f;
    void *values1[] = {&id1, name1, &score1};
    add_row(ds, values1);

    int id2 = 2;
    char *name2 = "Bob";
    float score2 = 92.0f;
    void *values2[] = {&id2, name2, &score2};
    add_row(ds, values2);

    for (size_t i = 0; i < ds->num_rows; ++i) {
        const Row *row = &ds->rows[i];
        for (size_t j = 0; j < ds->num_fields; ++j) {
            const Field *field = &row->fields[j];
            printf("%s: ", field->name);
            switch (field->type) {
                case TYPE_INT:
                    printf("%d\t", *(int *) field->data);
                    break;
                case TYPE_FLOAT:
                    printf("%.2f\t", *(float *) field->data);
                    break;
                case TYPE_STRING:
                    printf("%s\t", (char *) field->data);
                    break;
                default:
                    printf("Unknown\t");
                    break;
            }
        }
        printf("\n");
    }

    free_dataset(ds);
    return 0;
}
