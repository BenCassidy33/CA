#define PRINT_ARRAY_METADATA
#include "ca.c"
#include <stdio.h>

typedef struct {
      float x;
      float y;
} Point;

void print_callback(void *element, size_t k, size_t last) {
      Point *point = (Point *)element;
      if (k == 0) {
            printf("[\n(x, y) = (%f, %f),\n", point->x, point->y);
      } else if (last == 1) {
            printf("(%f, %f)]\n", point->x, point->y);
      } else {
            printf("(%f, %f),\n", point->x, point->y);
      }
}

int main() {
      Point p1 = (Point){.x = 5, .y = 2};
      Point p2 = (Point){.x = 10, .y = 5};

      Point tmp_points[] = {p1, p2};
      printf("%zu\n", sizeof(Point));
      Array points = new_array(tmp_points, sizeof(Point), 2);
      // Array arr = new_array(name, sizeof(char), strlen(name));
      print_arr(&points, print_callback);
}
