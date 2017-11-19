#ifndef GRAPH_H_J2HT0K9X
#define GRAPH_H_J2HT0K9X

#include <stdbool.h>

typedef struct AdjListNode_t {
    void* dest;
    struct AdjListNode_t* next;
} AdjListNode;

typedef struct AdjList_t {
    // pointer to head of list
    int num_of_members;
    // index value
    void* key;
    // point to next index
    struct AdjList_t* next;
    // adjlist head node
    AdjListNode* head;
} AdjList;

typedef int (*Graph_compare)(const void* a, const void* b);
typedef struct Graph_t {
    int num_of_vertices;
    // int num_of_edges;
    Graph_compare compare;
    struct AdjList_t* adjlist;
} Graph;

Graph* Graph_create(Graph_compare compare);
void Graph_add_edge(Graph* graph, void* source, void* dest);
void Graph_destory(Graph* graph);
_Bool Graph_has_edge(Graph* graph, void* source, void* dest);
void Graph_add_vertex_not_exists(Graph* graph, void* source);

typedef void (*Graph_traverse_cb)(Graph* graph, void* source, void* data);
void Graph_foreach(Graph* graph, Graph_traverse_cb func, void* source, void* data);

#define Graph_vertex_count(GRAPH) (GRAPH->num_of_vertices)
#define Graph_edge_count(GRAPH) (GRAPH->num_of_edges)

/* Graph algorithms */
void Graph_DFS_traverse(Graph* graph, void* source);
void Graph_BFS_traverse(Graph* graph, void* source);

#endif /* end of include guard: GRAPH_H_J2HT0K9X */
