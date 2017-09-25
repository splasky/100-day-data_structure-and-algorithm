#ifndef LIST_H
#define LIST_H

typedef struct Node_ {
    void* data;
    struct _Node* prev;
    struct _Node* next;
} Node;

typedef struct List_ {
    int size;
    Node* tail;
    Node* head;
    void (*destory)(struct List_** list);

} List;

List* init_link_list(List** list);
List* insert_node(List** list, void* data);
void destory(List** list);

#endif /* ifndef LIST_H */
