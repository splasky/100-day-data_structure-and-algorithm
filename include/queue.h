#ifndef QUEUE_H_HTFODBQ3
#define QUEUE_H_HTFODBQ3

#include "list.h"
typedef LinkedList Queue;

#define New_Queue New_LinkedList
#define Queue_destory LinkedList_destory
#define Queue_clear LinkedList_clear
#define Queue_clear_destory LinkedList_clear_destory

#define Queue_send LinkedList_push
#define Queue_recv LinkedList_shift

#define Queue_peek LinkedList_first
#define Queue_count LinkedList_count

#endif /* end of include guard: QUEUE_H_HTFODBQ3 */
