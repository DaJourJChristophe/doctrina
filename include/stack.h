#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include <stddef.h>
#include <stdint.h>

struct stack
{
  size_t cap;
  uint64_t top;
  uint8_t data[];
};

typedef struct stack stack_t;

stack_t *stack_create(const size_t cap);

void stack_destroy(stack_t *self);

int stack_push(stack_t *self, const void *data, const size_t size);

void *stack_peek(stack_t *self, const size_t size);

void *stack_pop(stack_t *self, const size_t size);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*STACK_H*/
