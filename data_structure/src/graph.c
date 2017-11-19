#include "../include/graph.h"
#include "../include/dbg.h"
#include "../include/list.h"
#include "../include/queue.h"
#include "../include/stack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static AdjListNode* New_AdjListNode(void* dest)
{
    AdjListNode* node = malloc(sizeof(AdjListNode));
    node->dest = dest;
    node->next = NULL;
    return node;
}

static AdjList* New_AdjList(void* source)
{
    AdjList* list = malloc(sizeof(AdjList));
    check_mem(list);
    list->key = source;
    list->next = NULL;
    list->head = NULL;
    list->num_of_members = 0;

    return list;
error:
    return NULL;
}

static inline _Bool Graph_is_vertex_in_graph(Graph* graph, void* key)
{
    check(graph, "graph is empty");
    AdjList* curr = graph->adjlist;
    while (curr) {
        if (graph->compare(curr->key, key) == 0) {
            return true;
        }
        curr = curr->next;
    }
    return false;

error:
    return false;
}

static inline void AdjList_destory(AdjList* adjlist)
{
    if (!adjlist->head) {
        goto finally;
    }

    AdjListNode* curr = adjlist->head;
    while (curr->next) {
        AdjListNode* prev = curr;
        curr = curr->next;
        if (prev) {
            if (prev->dest) {
                free(prev->dest);
                prev->dest = NULL;
            }
            free(prev);
            prev = NULL;
        }
    }

    free(curr);
    curr = NULL;
finally:
    free(adjlist);
    adjlist = NULL;
}

static int int_compare(const void* a, const void* b)
{
    const int x = *((int*)(a));
    const int y = *((int*)(b));
    return (x > y) - (y > x);
}

Graph* Graph_create(Graph_compare compare)
{
    Graph* graph = malloc(sizeof(graph));
    check_mem(graph);

    graph->num_of_vertices = 0;
    graph->compare = (compare != NULL) ? compare : int_compare;
    graph->adjlist = malloc(sizeof(AdjList));
    check_mem(graph->adjlist);

    return graph;
error:
    log_err("Create graph failed.");
    return NULL;
}

void Graph_add_vertex_not_exists(Graph* graph, void* source)
{
    if (!Graph_is_vertex_in_graph(graph, source)) {
        AdjList* adjlist = New_AdjList(source);
        check_mem(adjlist);
        /* add adjlist to graph */
        if (!graph->adjlist) {
            graph->adjlist = adjlist;
            return;
        }

        AdjList* curr = graph->adjlist;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = adjlist;
        graph->num_of_vertices++;
    }
error:
    log_err("Add vertex not exists failed");
}

static inline _Bool AdjList_is_node_in_list(Graph* graph, AdjList* adjlist, void* dest)
{
    check(adjlist, "adjlist is empty");
    AdjListNode* curr = adjlist->head;
    while (curr->next) {
        if (graph->compare(curr->dest, dest) == 0) {
            return true;
        }
        curr = curr->next;
    }
    return false;

error:
    return false;
}

static void AdjList_add_node_not_exists(Graph* graph, AdjList* adjlist, void* dest)
{
    if (!AdjList_is_node_in_list(graph, adjlist, dest)) {
        AdjListNode* new_node = New_AdjListNode(dest);
        check_mem(new_node);
        /* add adjlist to graph */
        if (!adjlist->head) {
            adjlist->head = new_node;
            adjlist->num_of_members++;
            return;
        }
        /* iterate adjlist nodes */
        AdjListNode* curr_node = adjlist->head;
        /* find the last AdjListnode*/
        while (curr_node->next) {
            curr_node = curr_node->next;
        }
        curr_node->next = new_node;
        adjlist->num_of_members++;
    }
error:
    log_err("Add vertex not exists failed");
}

void Graph_addEdge(Graph* graph, void* source, void* dest)
{
    Graph_add_vertex_not_exists(graph, source);
    Graph_add_vertex_not_exists(graph, dest);

    /* iterate adjlist */
    AdjList* curr = graph->adjlist;
    while (curr) {
        if (graph->compare(curr->key, source) == 0) {
            /* iterate adjlist nodes */
            AdjList_add_node_not_exists(graph, curr, dest);
            break;
        }
        curr = curr->next;
    }
}

void Graph_destory(Graph* graph)
{
    if (!graph) {
        goto finally;
    }

    const int num_of_vertices = graph->num_of_vertices;
    for (int i = 0; i < num_of_vertices; ++i) {
        AdjList_destory(graph->adjlist);
    }

finally:
    free(graph);
    graph = NULL;
}

_Bool Graph_has_edge(Graph* graph, void* source, void* dest)
{
    AdjList* curr = graph->adjlist;
    while (curr) {
        if (graph->compare(curr->key, source) == 0) {
            /* iterate adjlist nodes */
            AdjListNode* curr_node = curr->head;
            /* find the last AdjListnode*/
            while (curr_node) {
                if (graph->compare(curr_node->dest, dest)) {
                    return true;
                }
                curr_node = curr_node->next;
            }
        }
        curr = curr->next;
    }

    return false;
}

void Graph_print_graph(const Graph* graph, Graph_print_cb graph_print)
{
    check(graph, "Graph is empty");
    for (AdjList* head = graph->adjlist; head != NULL; head = head->next) {
        graph_print(head->key);
        for (AdjListNode* node = head->head; node != NULL; node = node->next) {
            graph_print(node->dest);
        }
        printf("\n");
    }

error:
    return;
}

static inline AdjList* Graph_find_AdjList(Graph* graph, void* key)
{
    check(graph, "Graph is empty");
    check(key, "Key is empty");
    AdjList* curr = graph->adjlist;
    while (curr) {
        if (graph->compare(curr->key, key) == 0) {
            return curr;
        }
        curr = curr->next;
    }

error:
    return NULL;
}
/* Graph search algorithms */
void Graph_DFS_traverse(Graph* graph, void* source)
{
    /* store visited AdjListNOde inside visited */
    LinkedList* visited = New_LinkedList();
    Stack* stack = New_Stack();

    /* find source */
    AdjList* adjlist = Graph_find_AdjList(graph, source);
    if (!adjlist) {
        goto source_not_found;
    }

    /* from source work to destination */
    Stack_push(stack, adjlist->head);
    LinkedList_push(visited, adjlist->key);

    while (Stack_count(stack) != 0) {
        AdjListNode* next_dest = Stack_pop(stack);
        while (next_dest != NULL) {
            /* if next_dest's dest not inside visited */
            if (!LinkedList_item_exists(visited, next_dest->dest, graph->compare)) {
                Stack_push(stack, next_dest->dest);
                LinkedList_push(visited, next_dest->dest);
                /* TODO: do something */
            }
            next_dest = next_dest->next;
        }
    }

source_not_found:
    log_err("Source not found in graph");
}

void Graph_BFS_traverse(Graph* graph, void* source)
{

    /* store visited AdjListNOde inside visited */
    LinkedList* visited = New_LinkedList();
    Queue* queue = New_Queue();

    /* find source */
    AdjList* adjlist = Graph_find_AdjList(graph, source);
    if (!adjlist) {
        goto source_not_found;
    }

    /* from source work to destination */
    Queue_send(queue, adjlist->head);
    LinkedList_push(visited, adjlist->key);

    while (Queue_count(queue) != 0) {
        AdjListNode* next_dest = Queue_recv(queue);
        while (next_dest != NULL) {
            /* if next_dest's dest not inside visited */
            if (!LinkedList_item_exists(visited, next_dest->dest, graph->compare)) {
                Queue_send(queue, next_dest->dest);
                LinkedList_push(visited, next_dest->dest);
                /* TODO: do something */
            }
            next_dest = next_dest->next;
        }
    }

source_not_found:
    log_err("Source not found in graph");
}
