#include <stdio.h>
#include "queue.h"

void queue_init(Queue *queue, void *buffer, int max_size)
{
  Queue *q = queue;
  q->head = buffer;
  q->last = buffer;
  q->buffer = buffer;
  q->buffer_size = max_size;
}

int queue_size(Queue *queue)
{
  Queue *q = queue;
  if (q->last >= q->head) {
    return q->last - q->head;
  } else {
    return q->buffer_size - (q->head - q->last);
  }
}

int queue_enqueue(Queue *queue, void *buffer, int size)
{
  Queue *q = queue;
  unsigned char *bp = buffer;
  int i;
  for (i=0; i<size; i++) {
    if (q->last + 1 == q->head) {
      return -1;
    }
    *q->last++ = *bp++;
    if (q->last - q->buffer >= q->buffer_size) {
      q->last = q->buffer;
    }
  }
  return 0;
}

void *queue_dequeue(Queue *queue, void *buffer, int size)
{
  Queue *q = queue;
  if (buffer != NULL) {
    unsigned char *bp = buffer;
    unsigned char *ptr = q->head;
    int i;
    for (i=0; i<size; i++) {
      if (ptr == q->last)
        break;
      bp[i] = *ptr++;
      if (ptr - q->buffer >= q->buffer_size) {
        ptr = q->buffer;
      }
    }
  }
  if (size > q->buffer_size) {
    return NULL; // error
  }

  int cursize = queue_size(q);
  if (size > cursize)
    size = cursize;

  if (q->head + size >= q->buffer + q->buffer_size) {
    q->head = q->head + size - q->buffer_size;
  } else {
    q->head += size;
  }
  return buffer;
}
