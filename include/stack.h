/*
 * Copyright (C) 2025 Da'Jour J. Christophe. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
