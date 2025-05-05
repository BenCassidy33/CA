#include <stddef.h>
#include <stdlib.h>

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

typedef struct {
      size_t length;
      size_t capacity;
      void *elements;
      size_t element_size;
} Array;

Array *new_array(void *elements, size_t element_size);

void foreach (Array *arr, void (*pred)(void *element));
void map(Array *arr, void *(*pred)(void *element));
// void array_contains(Array *arr, void *element);
void print_arr(Array *arr, void (*print_callback)(void *element));
void *index_arr(Array *arr, size_t index);
size_t reserve(Array *arr);
void apppend(Array *arr, void *element);
void prepend(Array *arr, void *element);
void foreach (Array *arr, void (*callback)(void *element));
void map(Array *arr, void *(*callback)(void *element));
typedef void (*PrintCallback)(void *element, size_t element_idx, bool is_last);

void print_arr(Array *arr, PrintCallback print_callback);
size_t contains(Array *haystack, void *needle);
Array *find_contains(Array *haystack, void *needle);

void *pop_element(Array *arr, size_t idx);
void compress(Array *arr);
