#ifndef CIRCULAR_QUEUE_H_YQ0ROWOP
#define CIRCULAR_QUEUE_H_YQ0ROWOP

#include "queue.h"
#include <stdbool.h>

typedef struct __circular_Queue Circular_Queue;
struct __circular_Queue {
    Queue* base_queue;
    int max;
};

#define is_Circular_Queue_Full(cqueue) ((cqueue->max == Queue_count(cqueue->base_queue)))
#define is_Circular_Queue_Empty(cqueue)                                                  \
    ((cqueue->base_queue->head == NULL) ? true : false)
#define Circular_Queue_count(cqueue) Queue_count(cqueue->base_queue)
#define Circular_Queue_destory_and_clean(cqueue)                                         \
    (Circular_Queue_clear(cqueue));                                                      \
    (Circular_Queue_destory(cqueue));

Circular_Queue* New_Circular_Queue(const int max);
int Circular_Queue_enqueue(Circular_Queue* cqueue, void* value);
void* Circular_Queue_dequeue(Circular_Queue* cqueue);
void Circular_Queue_destory(Circular_Queue* cqueue);
void Circular_Queue_clear(const Circular_Queue* cqueue);

#endif /* end of include guard: CIRCULAR_QUEUE_H_YQ0ROWOP */
