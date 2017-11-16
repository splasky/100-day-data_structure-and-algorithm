#include "../include/graph.h"
#include "../include/dbg.h"
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

static _Bool Graph_is_vertex_in_graph(Graph* graph, void* key)
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

static void AdjList_destory(AdjList* adjlist)
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

void Graph_add_Vertex_not_exists(Graph* graph, void* source)
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

static _Bool AdjList_is_node_in_list(Graph* graph, AdjList* adjlist, void* dest)
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
    Graph_add_Vertex_not_exists(graph, source);
    Graph_add_Vertex_not_exists(graph, dest);

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

void Graph_foreach(Graph* graph, Graph_traverse_cb func, void* source, void* data) {}

/* Graph search algorithms */
void Graph_Dfs(Graph* graph, void* data) {}
void Graph_Bfs(Graph* graph, void* data);
