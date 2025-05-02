#include <stdio.h>
#include <stdlib.h>

#include "circular_queue.h"

struct CircularQueue *CQ_initialize(int size) {
    struct CircularQueue *q = malloc(sizeof(struct CircularQueue));
    q->head = -1;
    q->tail = -1;
    q->size = size;
    q->queue = calloc(size, sizeof(void *));
    return q;
}

int CQ_empty(struct CircularQueue *q) { return q->head == -1; }

int CQ_full(struct CircularQueue *q) {
    return q->head == (q->tail + 1) % q->size;
}

void CQ_clear(struct CircularQueue *q) {
    q->head = q->tail = -1;
    for (size_t i = 0; i < q->size; i++) {
        if (q->queue[i]) {
            free(q->queue[i]);
            q->queue[i] = NULL;
        }
    }
}

void CQ_destroy(struct CircularQueue *q) {
    CQ_clear(q);
    free(q->queue);
    free(q);
}

int CQ_enqueue(void *data, struct CircularQueue *q) {
    printf("Calling enqueue\n");
    if (CQ_full(q)) {
        printf("Queue is full\n");
        return 0;
    }

    if (CQ_empty(q)) {
        q->head = 0;
    }

    q->tail = (q->tail + 1) % q->size;
    q->queue[q->tail] = data;
    return 1;
}

void *CQ_dequeue(struct CircularQueue *q) {
    printf("Calling dequeue\n");
    if (CQ_empty(q)) {
        return NULL;
    }

    void *head_item = q->queue[q->head];
    q->queue[q->head] = NULL;
    if (q->head == q->tail) {
        q->head = q->tail = -1;
    } else {
        q->head = (q->head + 1) % q->size;
    }
    return head_item;
}

void display_queue(struct CircularQueue *q) {
    if (CQ_empty(q)) {
        return;
    }

    printf("front: %d, back: %d\n", q->head, q->tail);

    for (size_t i = 0; i < q->size; i++) {
        if (!q->queue[i]) {
            printf("NULL ");
        } else {
            printf("%d ", *(int *)q->queue[i]);
        }
    }

    printf("\n");
}