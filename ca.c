#include <stdio.h>
#include <string.h>

typedef struct {
      void *elements;
      size_t element_size;
      size_t size;
      size_t capacity;
} Array;

Array new_array(void *elements, size_t element_size, size_t size) {
      Array arr = (Array){
          arr.elements = elements,
          arr.element_size = element_size,
          arr.size = size,
          arr.capacity = size,

      };

      return arr;
}

void foreach (Array *arr, void (*callback)(void *element)) {
      for (int i = 0; i < arr->size; i++) {
            void *element = (char *)arr->elements + (i * arr->element_size);
            callback(element);
      }
}

void map(Array *arr, void *(*callback)(void *element)) {
      for (int i = 0; i < arr->size; i++) {
            void *element = (char *)arr->elements + (i * arr->element_size);
            void *new_element = callback(element);
            memcpy(element, new_element, arr->element_size);
      }
}

// clang-format off

#define PRINT_CASE(type, fmt, element)      \
      case sizeof(type):                     \
            printf(fmt, *(type *)(element)); \
            break;

#define TYPE_CASE(ELEMENT) \
    PRINT_CASE(char, "%c, ", ELEMENT)    \
    PRINT_CASE(short, "%hd, ", ELEMENT)  \
    PRINT_CASE(int, "%d, ", ELEMENT)     \
    PRINT_CASE(double, "%lf, ", ELEMENT)
// PRINT_CASE(long, "%ld", ELEMENT)   \  TODO: FIX THIS
// PRINT_CASE(float, "%f", ELEMENT)   \  TODO: FIX THIS

// clang-format on

// void DEBUG_LOG(fmt, )

// You are expected to supply your own formatting. ',', '[', and ']' are not
// provided. Last = 1 if the element is the last element
typedef void (*PrintCallback)(void *element, size_t element_idx, size_t last);

void print_arr(Array *arr, PrintCallback print_callback) {
#ifdef PRINT_ARRAY_METADATA

      printf("Array Metadata: \n");
      printf("    Element Size = %zu\n", arr->element_size);
      printf("    Array Size = %zu\n", arr->size);
      printf("    Array Capacity = %zu\n", arr->capacity);
      printf("    Elements = ");

#endif

      if (print_callback != NULL) {
            for (size_t i = 0; i < arr->size; i++) {
                  void *element =
                      (char *)arr->elements + (i * arr->element_size);
                  print_callback(element, i, i == arr->element_size - 1);
            }
      } else {

#ifndef NO_WARN_NULL_CALLBACK
            printf("Warning: print_callback not passed, values might not be "
                   "printed properly. To ignore this message, define "
                   "NO_WARN_NULL_CALLBACK!\n");
#endif

            for (size_t i = 0; i < arr->size; i++) {
                  void *element =
                      (char *)arr->elements + (i * arr->element_size);
                  if (i == 0) {
                        printf("[%zu", arr->element_size);
                  } else {
                        printf("[%zu", arr->element_size);
                  }
                  switch (arr->element_size) {
                        TYPE_CASE(element);
                  default:
                        printf("Invalid element type. Please pass in "
                               "`print_callback` to print custom types.\n");
                  };
            }

            printf("\b\b]\n");
      }
}
