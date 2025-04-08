#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include <stddef.h>
#include <stdint.h>

typedef struct ring_buffer_writer ring_buffer_writer_t;

uint64_t ring_buffer_writer_get_head(const ring_buffer_writer_t *self);

uint64_t ring_buffer_writer_get_tail(const ring_buffer_writer_t *self);

typedef struct ring_buffer_reader ring_buffer_reader_t;

uint64_t ring_buffer_reader_get_head(const ring_buffer_reader_t *self);

uint64_t ring_buffer_reader_get_tail(const ring_buffer_reader_t *self);

typedef struct ring_buffer ring_buffer_t;

ring_buffer_t *ring_buffer_create(const size_t cap);

void ring_buffer_destroy(ring_buffer_t *self);

int ring_buffer_enqueue(ring_buffer_t *self, const void *data, size_t size);

void *ring_buffer_dequeue(ring_buffer_t *self, const size_t size);

size_t ring_buffer_size(const ring_buffer_t *self);

size_t ring_buffer_get_cap(const ring_buffer_t *self);

ring_buffer_writer_t *ring_buffer_get_writer(const ring_buffer_t *self);

ring_buffer_reader_t *ring_buffer_get_reader(const ring_buffer_t *self);

uint8_t *ring_buffer_get_data(const ring_buffer_t *self);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*RING_BUFFER_H*/
