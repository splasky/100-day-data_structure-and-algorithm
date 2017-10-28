#include "../include/circular_queue.h"
#include "../include/dbg.h"

Circular_Queue* New_Circular_Queue(int max)
{
    Circular_Queue* cqueue = calloc(1, sizeof(Circular_Queue));
    check_mem(cqueue);
    cqueue->base_queue = New_Queue();
    check_mem(cqueue->base_queue);
    cqueue->max = max;
    return cqueue;
error:
    return NULL;
}

int Circular_Queue_enqueue(Circular_Queue* cqueue, void* value)
{
    if (is_Circular_Queue_Full(cqueue)) {
        goto error;
    }

    Queue_send(cqueue->base_queue, value);
    Queue* base_queue = cqueue->base_queue;

    base_queue->head->prev = base_queue->tail;
    base_queue->tail->next = base_queue->head;

    return 0;
error:
    log_info("Circular_Queue is full");
    return -1;
}

void* Circular_Queue_dequeue(Circular_Queue* cqueue)
{
    void* value = Queue_recv(cqueue->base_queue);

    Queue* base_queue = cqueue->base_queue;

    base_queue->head->prev = base_queue->tail;
    base_queue->tail->next = base_queue->head;
    return value;
}

void Circular_Queue_destory(Circular_Queue* cqueue)
{

    ListNode* cur = Queue_peek(cqueue->base_queue);
    check(cur, "haed node is not exists");

    do {
        cur = cur->next;
        if (cur->prev) {
            free(cur->prev);
            cur->prev = NULL;
        }
    } while (cur != cqueue->base_queue->tail);

    free(cqueue->base_queue->tail);
    cqueue->base_queue->tail = NULL;
    free(cqueue->base_queue);
    cqueue->base_queue = NULL;
    free(cqueue);
    cqueue = NULL;
error:
    return;
}

void Circular_Queue_clear(const Circular_Queue* cqueue)
{
    ListNode* head = Queue_peek(cqueue->base_queue);
    check(head, "haed no is not exists");
    ListNode* cur = head;

    do {
        if (cur->value) {
            free(cur->value);
            cur->value = NULL;
        }
        cur = cur->next;
    } while (cur != head);
error:
    return;
}
