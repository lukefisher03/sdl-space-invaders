#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

struct CircularQueue {
    void **queue; // create void pointer array to hold items
    int head;
    int tail;
    int size;
};

struct CircularQueue *CQ_initialize(int size);

int CQ_empty(struct CircularQueue *q);

int CQ_full(struct CircularQueue *q);

void CQ_clear(struct CircularQueue *q);

void CQ_destroy(struct CircularQueue *q);

int CQ_enqueue(void *data, struct CircularQueue *q);

void *CQ_dequeue(struct CircularQueue *q);

void display_queue(struct CircularQueue *q);

#endif