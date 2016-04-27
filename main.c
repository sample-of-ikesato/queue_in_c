#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "queue.h"

void debug_buffer(unsigned char *buffer, int size) {
  int i;
  for (i=0; i<size; i++) {
    printf(" %02x", buffer[i]);
    if (i%16 == 15)
      printf("\n");
  }
  printf("\n");
}

void test_init() {
  Queue q;
  unsigned char buffer[10];
  queue_init(&q, buffer, sizeof(buffer));
  assert(q.head == buffer);
  assert(q.size == 0);
  assert(q.buffer == buffer);
  assert(q.buffer_size == 10);
}

void test_normal_usage() {
  Queue q;
  unsigned char buffer[10];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_size(&q) == 3);

  unsigned char tmp[10];
  assert(queue_dequeue(&q, tmp, 3) == 3);
  assert(memcmp(tmp, "123", 3) == 0);
  assert(queue_size(&q) == 0);
}

void test_ring_buffer() {
  Queue q;
  unsigned char buffer[3];
  unsigned char tmp[3];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_size(&q) == 3);

  assert(memcmp(buffer, "123", 3) == 0);
  assert(queue_dequeue(&q, tmp, 1) == 1);
  assert(memcmp(tmp, "1", 1) == 0);
  assert(memcmp(buffer, "123", 3) == 0);
  assert(queue_size(&q) == 2);
  assert(queue_enqueue(&q, "4", 1) == 0);
  assert(queue_size(&q) == 3);
  assert(memcmp(buffer, "423", 3) == 0);
  assert(queue_enqueue(&q, "5", 1) == -1);
  assert(queue_dequeue(&q, tmp, 3) == 3);
  assert(memcmp(tmp, "234", 2) == 0);
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "567", 3) == 0);
  assert(memcmp(buffer, "756", 3) == 0);
  assert(queue_size(&q) == 3);
  assert(queue_dequeue(&q, tmp, 1) == 1);
  assert(queue_dequeue(&q, tmp, 3) == 2);
  assert(queue_size(&q) == 0);
}

void test_ring_buffer_without_output() {
  Queue q;
  unsigned char buffer[3];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_size(&q) == 3);

  assert(memcmp(buffer, "123", 3) == 0);
  assert(queue_dequeue(&q, NULL, 1) == 1);
  assert(memcmp(buffer, "123", 3) == 0);
  assert(queue_size(&q) == 2);
  assert(queue_enqueue(&q, "4", 1) == 0);
  assert(queue_size(&q) == 3);
  assert(memcmp(buffer, "423", 3) == 0);
  assert(queue_enqueue(&q, "5", 1) == -1);
  assert(queue_dequeue(&q, NULL, 3) == 3);
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "567", 3) == 0);
  assert(memcmp(buffer, "756", 3) == 0);
  assert(queue_size(&q) == 3);
  assert(queue_dequeue(&q, NULL, 1) == 1);
  assert(queue_dequeue(&q, NULL, 3) == 2);
  assert(queue_size(&q) == 0);
}



int main(int argc, char **argv)
{
  test_init();
  test_normal_usage();
  test_ring_buffer();
  test_ring_buffer_without_output();
  return 0;
}