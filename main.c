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
  assert(memcmp(tmp, "345", 2) == 0);
  assert(memcmp(buffer, "453", 3) == 0);
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "567", 3) == 0);
  assert(memcmp(buffer, "675", 3) == 0);
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
  assert(memcmp(buffer, "675", 3) == 0);
  assert(queue_size(&q) == 3);
  assert(queue_dequeue(&q, NULL, 1) == 1);
  assert(queue_dequeue(&q, NULL, 3) == 2);
  assert(queue_size(&q) == 0);
}


void test_queue_clear() {
  Queue q;
  unsigned char buffer[3];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_dequeue(&q, NULL, 1) == 1);
  assert(queue_enqueue(&q, "4", 1) == 0);
  queue_clear(&q);
  assert(queue_size(&q) == 0);
  assert(memcmp(buffer, "423", 3) == 0);
}

void test_enqueue_from_queue() {
  Queue q1;
  Queue q2;
  unsigned char buffer1[3];
  unsigned char buffer2[3];
  queue_init(&q1, buffer1, sizeof(buffer1));
  queue_init(&q2, buffer2, sizeof(buffer2));

  assert(queue_size(&q1) == 0);
  assert(queue_enqueue(&q1, "123", 3) == 0);
  assert(queue_size(&q1) == 3);

  assert(queue_size(&q2) == 0);
  assert(queue_enqueue_from_queue(&q2, &q1) == 0);
  assert(queue_size(&q2) == 3);
  assert(queue_size(&q1) == 3);
  assert(memcmp(buffer1, "123", 3) == 0);
  assert(memcmp(buffer2, "123", 3) == 0);

  assert(queue_dequeue(&q2, NULL, 1) == 1);
  assert(memcmp(buffer2, "123", 3) == 0);
  assert(queue_size(&q2) == 2);
  queue_clear(&q1);
  assert(queue_enqueue(&q1, "4", 1) == 0);
  assert(queue_size(&q1) == 1);
  assert(queue_enqueue_from_queue(&q2, &q1) == 0);
  assert(memcmp(buffer2, "423", 3) == 0);
  assert(queue_dequeue(&q2, NULL, 3) == 3);
  assert(queue_size(&q2) == 0);
  queue_clear(&q1);
  assert(queue_enqueue(&q1, "567", 3) == 0);
  assert(queue_enqueue_from_queue(&q2, &q1) == 0);
  assert(memcmp(buffer2, "756", 3) == 0);
  assert(queue_size(&q2) == 3);
  assert(queue_dequeue(&q2, NULL, 1) == 1);
  assert(queue_dequeue(&q2, NULL, 3) == 2);
  assert(queue_size(&q2) == 0);
}

void test_queue_peek() {
  Queue q;
  unsigned char buffer[3];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_size(&q) == 3);
  assert(queue_peek(&q, 0) == '1');
  assert(queue_peek(&q, 1) == '2');
  assert(queue_peek(&q, 2) == '3');

  assert(queue_dequeue(&q, NULL, 1) == 1);
  assert(queue_size(&q) == 2);
  assert(queue_enqueue(&q, "4", 1) == 0);
  assert(queue_peek(&q, 0) == '2');
  assert(queue_peek(&q, 1) == '3');
  assert(queue_peek(&q, 2) == '4');

  assert(queue_enqueue(&q, "5", 1) == -1);
  assert(queue_dequeue(&q, NULL, 3) == 3);
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "567", 3) == 0);
  assert(queue_peek(&q, 0) == '5');
  assert(queue_peek(&q, 1) == '6');
  assert(queue_peek(&q, 2) == '7');
  assert(memcmp(buffer, "675", 3) == 0);
  assert(queue_size(&q) == 3);
}

void test_queue_peek_for_ringbuffer() {
  Queue q;
  unsigned char buffer[3];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_size(&q) == 0);
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_size(&q) == 3);
  assert(queue_peek(&q, 0) == '1');
  assert(queue_peek(&q, 1) == '2');
  assert(queue_peek(&q, 2) == '3');
  assert(queue_dequeue(&q, NULL, 1) == 1);
  assert(queue_peek(&q, 0) == '2');
  assert(queue_enqueue(&q, "4", 1) == 0);
  assert(queue_peek(&q, 0) == '2');
}


void test_enqueue_from_queue_bug_for_success_pattern() {
  Queue q;
  unsigned char buffer[3];
  queue_init(&q, buffer, sizeof(buffer));
  assert(queue_enqueue(&q, "123", 3) == 0);
  assert(queue_dequeue(&q, NULL, 2) == 2);
  assert(queue_enqueue(&q, "45", 2) == 0);
  assert(memcmp(buffer, "453", 3) == 0);
  assert(queue_enqueue(&q, "6", 1) == -1);
  assert(memcmp(buffer, "456", 3) == 0);
}

void test_enqueue_from_queue_bug() {
  Queue q1,q2;
  unsigned char buffer1[3];
  unsigned char buffer2[3];
  queue_init(&q1, buffer1, sizeof(buffer1));
  queue_init(&q2, buffer2, sizeof(buffer2));
  queue_enqueue(&q2, "123", 3);
  assert(queue_enqueue_from_queue(&q1, &q2) == 0);
  assert(queue_dequeue(&q1, NULL, 2) == 2);
  queue_clear(&q2); queue_enqueue(&q2, "45", 2);
  assert(queue_enqueue_from_queue(&q1, &q2) == 0);
  assert(memcmp(buffer1, "453", 3) == 0);
  queue_clear(&q2); queue_enqueue(&q2, "6", 1);
  assert(queue_enqueue_from_queue(&q1, &q2) == -1);
  assert(memcmp(buffer1, "456", 3) == 0);
}

int main(int argc, char **argv)
{
  test_init();
  test_normal_usage();
  test_ring_buffer();
  test_ring_buffer_without_output();
  test_queue_clear();
  test_enqueue_from_queue();
  test_queue_peek();
  test_queue_peek_for_ringbuffer();
  test_enqueue_from_queue_bug_for_success_pattern();
  test_enqueue_from_queue_bug();
  printf("success!\n");
  return 0;
}