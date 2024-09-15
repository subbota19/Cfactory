#include <stdio.h>
#include "dataset.h"

int main() {
    Dataset *ds = dataset_init(2);

    dataset_append(ds, 1.1);
    dataset_append(ds, 2.2);
    dataset_append(ds, 3.3);

    printf("Dataset elements:\n");
    for (size_t i = 0; i < ds->len; i++) {
        printf("%.2f ", dataset_return_element(ds, i));
    }

    dataset_delete(ds, 1);

    printf("After deletion:\n");
    for (size_t i = 0; i < ds->len; i++) {
        printf("%.2f ", dataset_return_element(ds, i));
    }

    dataset_resize(ds, 10);
    dataset_append(ds, 4.4);

    printf("After resize and append:\n");
    for (size_t i = 0; i < ds->len; i++) {
        printf("%.2f ", dataset_return_element(ds, i));
    }

    dataset_destroy(ds);

    return 0;
}
