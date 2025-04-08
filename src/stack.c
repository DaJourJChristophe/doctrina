#include "stack.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

stack_t *stack_create(const size_t cap)
{
  const size_t memory = offsetof(stack_t, data[cap]);
  stack_t *self = NULL;

  self = (stack_t *)malloc(memory);
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate stack to the heap");
    exit(EXIT_FAILURE);
  }

  self->cap = cap;

  return self;
}

void stack_destroy(stack_t *self)
{
  if (self != NULL)
  {
    free(self);
    self = NULL;
  }
}

int stack_push(stack_t *self, const void *data, const size_t size)
{
  if ((size + self->top) >= self->cap)
  {
    return (-1);
  }

  memcpy((self->data + self->top), data, size);

  self->top += size;

  return 0;
}

void *stack_peek(stack_t *self, const size_t size)
{
  if (size > self->top)
  {
    return NULL;
  }

  void *data = NULL;

  data = calloc(size, sizeof(*self->data));
  if (data == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate stack to the heap");
    exit(EXIT_FAILURE);
  }

  memcpy(data, (self->data + (self->top - size)), size);

  return data;
}

void *stack_pop(stack_t *self, const size_t size)
{
  if (size > self->top)
  {
    return NULL;
  }

  void *data = NULL;

  data = calloc(size, sizeof(*self->data));
  if (data == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate stack to the heap");
    exit(EXIT_FAILURE);
  }

  self->top -= size;

  memcpy(data, (self->data + self->top), size);

  return data;
}
