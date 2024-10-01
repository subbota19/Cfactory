#include <stdio.h>
#include "dataset.h"

#define DEFAULT_CAPACITY 2

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

    int id2 = 2;
    char *name2 = "Bob";
    float score2 = 92.0f;
    void *values2[] = {&id2, name2, &score2};

    add_row(ds, values1);
    add_row(ds, values2);
    add_row(ds, values1);
    add_row(ds, values1);


    printf("%lu\n", get_num_rows(ds));
    printf("%lu\n", ds->num_rows);
    printf("%lu\n", ds->capacity);

    print_row(ds,3);

    free_dataset(ds);
    return 0;
}
