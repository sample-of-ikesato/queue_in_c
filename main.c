#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "queue.h"

void test_init() {
  Queue q;
  unsigned char buffer[10];
  queue_init(&q, buffer, sizeof(buffer));
  assert(q.head == buffer);
  assert(q.last == buffer);
  assert(q.buffer == buffer);
  assert(q.buffer_size == 10);
  assert(q.buffer[0] == '\0');
}

void test_normal_usage() {
  Queue q;
  unsigned char buffer[10];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_size(&q) == 3);

  unsigned char *ptr;
  unsigned char tmp[10];
  ptr = queue_dequeue(&q, tmp, 3);
  assert(memcmp(ptr, "123", 3) == 0);
  assert(ptr == tmp);
  assert(queue_size(&q) == 0);
}

void test_ring_buffer() {
  Queue q;
  unsigned char buffer[3];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_size(&q) == 3);

  unsigned char *ptr;
  unsigned char tmp[10];
  ptr = queue_dequeue(&q, tmp, 3);
  assert(memcmp(ptr, "123", 3) == 0);
  assert(ptr == tmp);
  assert(queue_size(&q) == 0);
}




int main(int argc, char **argv)
{
  //test_init();
  //test_normal_usage();
  test_ring_buffer();
  return 0;
}