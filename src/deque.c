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
#include "common.h"
#include "deque.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ring_buffer_writer {
  uint64_t head;
  uint64_t tail;
};

uint64_t ring_buffer_writer_get_head(const ring_buffer_writer_t *self)
{
  return self->head;
}

uint64_t ring_buffer_writer_get_tail(const ring_buffer_writer_t *self)
{
  return self->tail;
}

struct ring_buffer_reader {
  uint64_t head;
  uint64_t tail;
};

uint64_t ring_buffer_reader_get_head(const ring_buffer_reader_t *self)
{
  return self->head;
}

uint64_t ring_buffer_reader_get_tail(const ring_buffer_reader_t *self)
{
  return self->tail;
}

struct ring_buffer
{
  size_t cap;
  ring_buffer_writer_t writer;
  ring_buffer_reader_t reader;
  uint8_t data[];
};

ring_buffer_t *ring_buffer_create(const size_t cap)
{
  const size_t memory = offsetof(ring_buffer_t, data[cap]);
  ring_buffer_t *self = NULL;

  self = (ring_buffer_t *)calloc(memory, sizeof(uint8_t));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate ring buffer to the heap");
    exit(EXIT_FAILURE);
  }

  self->cap = cap;
  self->writer.tail = cap;

  return self;
}

void ring_buffer_destroy(ring_buffer_t *self)
{
  if (self != NULL)
  {
    free(self);
    self = NULL;
  }
}

static inline uint64_t always_inline ring_buffer_mask(ring_buffer_t *self, const uint64_t index)
{
  return index & (self->cap - 1UL);
}

static inline uint64_t always_inline ring_buffer_reader_capacity(ring_buffer_t *self)
{
  return self->reader.tail - self->reader.head;
}

static inline uint64_t always_inline ring_buffer_writer_capacity(ring_buffer_t *self)
{
  return self->writer.tail - self->writer.head;
}

static inline void always_inline ring_buffer_try_resize(ring_buffer_t *self)
{
  if (self->writer.head == self->writer.tail)
  {
    self->writer.tail = self->reader.head;
  }
}

static inline void always_inline ring_buffer_increment(ring_buffer_t *self, const size_t size)
{
  self->writer.head += size;
  self->reader.tail += size;
}

static inline void always_inline ring_buffer_try_reset(ring_buffer_t *self)
{
  if (self->reader.head == self->reader.tail)
  {
    self->reader.head = self->writer.head;
    self->reader.tail = self->reader.head;
  }
}

static inline void always_inline ring_buffer_decrement(ring_buffer_t *self, const size_t size)
{
  self->reader.head += size;
}

int ring_buffer_enqueue(ring_buffer_t *self, const void *data, size_t size)
{
  if (size > ring_buffer_writer_capacity(self))
  {
    return (-1);
  }

  if (size > (self->writer.head % self->cap))
  {
    self->writer.head = 0UL;
  }

  if (size > ring_buffer_writer_capacity(self))
  {
    return (-1);
  }

  const void *pointer = self->data + ring_buffer_mask(self, self->writer.head);
  memcpy((void *)pointer, data, size);

  ring_buffer_increment(self, size);
  ring_buffer_try_resize(self);

  return 0;
}

void *ring_buffer_dequeue(ring_buffer_t *self, const size_t size)
{
  if (size > ring_buffer_reader_capacity(self))
  {
    return NULL;
  }

  void *data = NULL;
  data = calloc(size, sizeof(*self->data));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate data buffer to the heap");
    exit(EXIT_FAILURE);
  }

  const void *pointer = self->data + ring_buffer_mask(self, self->reader.head);
  memcpy(data, pointer, size);

  ring_buffer_decrement(self, size);
  ring_buffer_try_reset(self);

  return data;
}

size_t ring_buffer_size(const ring_buffer_t *self)
{
  return self->writer.head - self->reader.head;
}

size_t ring_buffer_get_cap(const ring_buffer_t *self)
{
  return self->cap;
}

ring_buffer_writer_t *ring_buffer_get_writer(const ring_buffer_t *self)
{
  return (ring_buffer_writer_t *)&self->writer;
}

ring_buffer_reader_t *ring_buffer_get_reader(const ring_buffer_t *self)
{
  return (ring_buffer_reader_t *)&self->reader;
}

uint8_t *ring_buffer_get_data(const ring_buffer_t *self)
{
  return (uint8_t *)self->data;
}
