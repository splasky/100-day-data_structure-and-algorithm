#ifndef GRAPH_H_J2HT0K9X
#define GRAPH_H_J2HT0K9X

#include <stdbool.h>

typedef struct AdjListNode_t {
    void* dest;
    int weight;
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
void Graph_add_edge(Graph* graph, void* source, void* dest, int weight);
void Graph_destory(Graph* graph);
_Bool Graph_has_edge(Graph* graph, void* source, void* dest);
void Graph_add_vertex_not_exists(Graph* graph, void* source);

typedef void (*Graph_print_cb)(const void* value);
void Graph_print_graph(const Graph* graph, Graph_print_cb graph_print);

#define Graph_vertex_count(GRAPH) (GRAPH->num_of_vertices)
#define Graph_edge_count(GRAPH) (GRAPH->num_of_edges)

AdjList* Graph_find_AdjList(Graph* graph, void* key);
AdjListNode* Graph_find_AdjNode(Graph* graph, void* key, void* dest);

/* TODO */
void Graph_remove_edge(Graph* graph, void* source, void* dest);
void Graph_remove_vertex(Graph* graph, void* vertex);

/* Graph algorithms */
void Graph_DFS_traverse(Graph* graph, void* source);
void Graph_BFS_traverse(Graph* graph, void* source);

#endif /* end of include guard: GRAPH_H_J2HT0K9X */
