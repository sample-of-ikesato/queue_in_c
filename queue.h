#ifndef _queue_h_
#define _queue_h_

typedef struct Queue_t {
  unsigned char *head;
  unsigned char *last;
  unsigned char *buffer;
  int buffer_size;
} Queue;

// initilize Queue object
void queue_init(Queue *queue, void *buffer, int max_size);

// get current queue size
int queue_size(Queue *queue);

// enqueu buffer
// return 0:success -1:buffer over flow
int queue_enqueue(Queue *queue, void *buffer, int size);

// dequeue buffer
// return buffer (same as argubment's buffer)
void *queue_dequeue(Queue *queue, void *buffer, int size);

#endif//_queue_h_
