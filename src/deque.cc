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
#include "deque.h"

#include <iostream>

template<typename T>
class Deque
{
  ring_buffer_t *__buffer = nullptr;

public:
  Deque(const size_t capacity)
  {
    __buffer = ring_buffer_create(capacity);
  }

  ~Deque()
  {
    ring_buffer_destroy(__buffer);
  }

  int enqueue(const T& data)
  {
    return ring_buffer_enqueue(__buffer, &data, sizeof(T));
  }

  void *dequeue(void)
  {
    return ring_buffer_dequeue(__buffer, sizeof(T));
  }
};

void cleanup(void *pointer)
{
  free(pointer);
  pointer = NULL;
}

int main(void)
{
  Deque<int> queue(16);

  if (0 > queue.enqueue(1))
  {
    std::cerr << __func__ << "(): Could not enqueue item into queue" << std::endl;
  }
  if (0 > queue.enqueue(4))
  {
    std::cerr << __func__ << "(): Could not enqueue item into queue" << std::endl;
  }
  if (0 > queue.enqueue(9))
  {
    std::cerr << __func__ << "(): Could not enqueue item into queue" << std::endl;
  }
  if (0 > queue.enqueue(16))
  {
    std::cerr << __func__ << "(): Could not enqueue item into queue" << std::endl;
  }
  if (0 > queue.enqueue(25))
  {
    std::cerr << __func__ << "(): Could not enqueue item into queue" << std::endl;
  }

  int *data = nullptr;
  while (NULL != (data = (int *)queue.dequeue()))
  {
    std::cout << *(int *)data << std::endl;
    cleanup(data);
  }

  return 0;
}
