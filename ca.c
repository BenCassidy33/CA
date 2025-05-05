#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1

typedef struct {
      void *elements;
      size_t element_size;
      size_t length;
      size_t capacity;
} Array;

Array new_array(void *elements, size_t element_size, size_t size) {
      Array arr = (Array){
          arr.elements = elements,
          arr.element_size = element_size,
          arr.length = size,
          arr.capacity = size,
      };

      return arr;
}

void *index_arr(Array *arr, size_t index) {
      if (index < arr->length) {
            printf("Out of bounds index: Array Length: %zu, Index: %zu",
                   arr->length, index);
            exit(1);
      };
      return (char *)arr->elements + (index * arr->element_size);
}

size_t reserve(Array *arr) {
      arr->capacity = ceil(arr->length * 1.5f);
      void *old = arr->elements;
      void *new = malloc(arr->capacity * arr->element_size);

      if (!new) {
            perror("Could not allocate memory for array.");
            return (size_t)NULL;
      }

      void *dest = memmove(new, arr->elements, arr->length * arr->element_size);
      if (!dest) {
            perror("Could not move elements.");
            return (size_t)NULL;
      }

      arr->elements = dest;
      free(old);
      return OK;
}

// copies element to start of array
void append(Array *arr, void *element) {
      arr->length++;
      if (arr->element_size == 0) {
            arr->element_size = sizeof(element);
      }

      if (arr->length - 1 == arr->capacity) {
            if (!reserve(arr)) {
                  return;
            };
      }

      void *e = (char *)arr->elements + (arr->length - 1);
      memcpy(e, element, arr->element_size);
}

void prepend(Array *arr, void *element) {
      if (arr->capacity == arr->length) {
            reserve(arr);
      }

      // get a pointer to the second element of the array
      void *dest = index_arr(arr, 1);
      memmove(dest, arr->elements, arr->element_size * arr->length);
      memcpy(arr->elements, element, arr->element_size);
      arr->length++;
}

void foreach (Array *arr, void (*callback)(void *element)) {
      for (int i = 0; i < arr->length; i++) {
            void *element = (char *)arr->elements + (i * arr->element_size);
            callback(element);
      }
}

void map(Array *arr, void *(*callback)(void *element)) {
      for (int i = 0; i < arr->length; i++) {
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
typedef void (*PrintCallback)(void *element, size_t element_idx, bool is_last);

void print_arr(Array *arr, PrintCallback print_callback) {
#ifdef PRINT_ARRAY_METADATA

      printf("Array Metadata: \n");
      printf("    Element Size = %zu\n", arr->element_size);
      printf("    Array Size = %zu\n", arr->length);
      printf("    Array Capacity = %zu\n", arr->capacity);
      printf("    Elements = ");

#endif

      if (print_callback != NULL) {
            for (size_t i = 0; i < arr->length; i++) {
                  void *element =
                      (char *)arr->elements + (i * arr->element_size);
                  print_callback(element, i,
                                 (bool)(i == arr->element_size - 1));
            }
      } else {

#ifndef NO_WARN_NULL_CALLBACK
            printf("Warning: print_callback not passed, values might not be "
                   "printed properly. To ignore this message, define "
                   "NO_WARN_NULL_CALLBACK!\n");
#endif

            for (size_t i = 0; i < arr->length; i++) {
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

Array empty_array() {
      return (Array){
          .elements = NULL,
          .element_size = 0,
          .length = 0,
          .capacity = 0,
      };
}

/// returns the amount of elements within the array
size_t contains(Array *haystack, void *needle) {
      size_t n = 0;

      for (int i = 0; i < haystack->length; i++) {
            if (memcmp(index_arr(haystack, i), needle,
                       haystack->element_size)) {
                  n++;
            }
      }

      return n;
}

Array *find_contains(Array *haystack, void *needle) {
      Array *arr = (Array *)malloc(sizeof(Array));
      *arr = empty_array();

      void *e;
      for (int i = 0; i < haystack->length; i++) {
            if (memcmp((e = index_arr(haystack, i)), needle,
                       haystack->element_size)) {
                  append(arr, e);
            }
      }

      return arr;
};

void *pop_element(Array *arr, size_t idx) {
      if (idx >= arr->length) {
            return NULL;
      }

      void *elem_ptr = index_arr(arr, idx);
      void *return_value = malloc(arr->element_size);
      memcpy(return_value, elem_ptr, arr->element_size);

      if (idx < arr->length - 1) {
            void *next_elem_ptr = index_arr(arr, idx + 1);
            size_t move_size = arr->length - idx - 1;
            // need to check bounds to ensure memmove does not move parts of
            // memory that are past the vector
            memmove(elem_ptr, next_elem_ptr, move_size * arr->element_size);
      }

      arr->length--;
      return return_value;
}

void *pop_end(Array *arr) {
      if (arr->length == 0) {
            return NULL;
      }

      arr->length--;

      void *last_elem = index_arr(arr, arr->length);
      void *return_value = malloc(arr->element_size);
      memcpy(return_value, last_elem, arr->element_size);

      return return_value;
}

/// removes ending parts of the element buffer where there are no elements
void compress(Array *arr) {
      arr->elements = realloc(arr->elements, arr->length * arr->element_size);
      arr->capacity = arr->length;
}
