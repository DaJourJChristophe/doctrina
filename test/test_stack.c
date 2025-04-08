#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>

#include "cmocka.h"

#include "common.h"
#include "stack.h"

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
