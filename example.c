// This example works on both a C or C++ compiler

#include <stdlib.h>
#include <stdio.h>
#define DEFER_IMPL
#include "defer.h"

struct my_struct {
  int* a;
  int* b;
};

struct my_struct* create_my_struct() {
  struct my_struct* s = (struct my_struct*) malloc(sizeof(struct my_struct));
  s->a = (int*) malloc(sizeof(int));
  s->b = (int*) malloc(sizeof(int));
  return s;
}

void destroy_my_struct(struct my_struct* s) {
  free(s->a);
  free(s->b);
  free(s);
}

int main() {
  autofree int* i = (int*) malloc(sizeof(int));
  struct my_struct* s = create_my_struct();
  defer({
    destroy_my_struct(s);
  });

  printf("i = %d\n", *i);
  printf("s.a = %d, s.b = %d\n", *s->a, *s->b);

  return 0;
}

