#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack
{
  size_t cap;
  uint64_t top;
  uint8_t data[];
};

typedef struct stack stack_t;

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

#include <assert.h>

int main(void)
{
  stack_t *stack = NULL;
  stack = stack_create(16UL);
  assert(stack != NULL && stack->cap == 16UL && stack->top == 0UL);
  assert(0 == stack_push(stack, &(uint32_t){5U}, sizeof(uint32_t)));
  assert(sizeof(uint32_t) == stack->top);
  assert(5U == *(uint32_t *)stack->data);
  assert(0 == stack_push(stack, &(uint32_t){6U}, sizeof(uint32_t)));
  assert((2 * sizeof(uint32_t)) == stack->top);
  assert(6U == *(uint32_t *)(stack->data + sizeof(uint32_t)));
  assert(0 == stack_push(stack, &(uint32_t){7U}, sizeof(uint32_t)));
  assert((3 * sizeof(uint32_t)) == stack->top);
  assert(7U == *(uint32_t *)(stack->data + (2 * sizeof(uint32_t))));
  const uint64_t old_top = stack->top;
  stack->top = 16UL;
  assert(0 > stack_push(stack, &(uint32_t){9U}, sizeof(uint32_t)));
  stack->top = old_top;
  void *data = NULL;
  assert(stack->top == (3 * sizeof(uint32_t)));
  data = stack_peek(stack, sizeof(uint32_t));
  assert(data != NULL && *(uint32_t *)data == 7U);
  if (data != NULL) { free(data); data = NULL; }
  assert(stack->top == (3 * sizeof(uint32_t)));
  data = stack_pop(stack, sizeof(uint32_t));
  assert(data != NULL && *(uint32_t *)data == 7U);
  if (data != NULL) { free(data); data = NULL; }
  assert(stack->top == (2 * sizeof(uint32_t)));
  data = stack_pop(stack, sizeof(uint32_t));
  assert(data != NULL && *(uint32_t *)data == 6U);
  if (data != NULL) { free(data); data = NULL; }
  assert(stack->top == (1 * sizeof(uint32_t)));
  data = stack_pop(stack, sizeof(uint32_t));
  assert(data != NULL && *(uint32_t *)data == 5U);
  if (data != NULL) { free(data); data = NULL; }
  assert(stack->top == 0UL);
  data = stack_pop(stack, sizeof(uint32_t));
  assert(data == NULL);
  stack_destroy(stack);
  return EXIT_SUCCESS;
}



