#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list
{
  size_t cap;
  size_t len;
  uint8_t *data;
};

typedef struct list list_t;

list_t *list_create(const size_t cap)
{
  list_t *self = NULL;

  self = calloc(1UL, sizeof(*self));
  if (self == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate list to the heap");
    exit(EXIT_FAILURE);
  }

  self->cap = cap;

  return self;
}

void list_destroy(list_t *self)
{
  if (self != NULL)
  {
    if (self->data != NULL)
    {
      free(self->data);
      self->data = NULL;
    }

    free(self);
    self = NULL;
  }
}

int list_append(list_t *self, const void *data, const size_t size)
{
  if (self->cap > 0UL && (size + self->len) > self->cap)
  {
    return (-1);
  }

  self->len += size;

  if (self->data == NULL)
  {
    self->data = (uint8_t *)malloc(size);
  }
  else
  {
    void *old = NULL;
    old = self->data;
    self->data = NULL;
    self->data = (uint8_t *)realloc(old, self->len);
  }

  if (self->data == NULL)
  {
    fprintf(stderr, "%s(): %s\n", __func__, "Could not allocate list data buffer to the heap");
    exit(EXIT_FAILURE);
  }

  memcpy((self->data + self->len), data, size);

  return 0;
}

#include <assert.h>

int main(void)
{
  list_t *list = NULL;
  list = list_create(16UL);
  assert(list != NULL && list->cap == 16UL && list->len == 0UL && list->data == NULL);
  assert(0 == list_append(list, &(uint32_t){1U}, sizeof(uint32_t)));
  assert(list->len == sizeof(uint32_t) && list->data != NULL);
  list_destroy(list);
  return EXIT_SUCCESS;
}
