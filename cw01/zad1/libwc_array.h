#ifndef WC_ARRAY_H
#define WC_ARRAY_H

#include <stdio.h>

typedef struct wc_array wc_array_t;

wc_array_t* wc_array_create(size_t max_size);
void wc_array_free(wc_array_t* arr);

int wc_array_count(wc_array_t* arr, const char* filename);
char* wc_array_get(wc_array_t* arr, size_t index);
int wc_array_remove(wc_array_t* arr, size_t index);

#endif /* WC_ARRAY_H */

