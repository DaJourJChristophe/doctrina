template<typename T>
class Deque
{
  ring_buffer_t *__buffer = nullptr;

public:
  Deque()
  {
    __buffer = ring_buffer_create(32UL);
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

int main(void)
{
  Deque<int> queue;

  queue.enqueue(1);
  queue.enqueue(4);
  queue.enqueue(9);

  int *data = nullptr;
  while (NULL != (data = queue.dequeue()))
  {
    std::cout << *(int *)data << std:endl;
  }

  return 0;
}
