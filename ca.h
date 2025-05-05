#include <stddef.h>
#include <stdlib.h>

typedef struct {
      size_t size;
      size_t capacity;
      void *elements;
      size_t element_size;
} Array;

Array *new_array(void *elements, size_t element_size);

void foreach (Array *arr, void (*pred)(void *element));
void map(Array *arr, void *(*pred)(void *element));
void print_arr(Array *arr, void (*print_callback)(void *element));
