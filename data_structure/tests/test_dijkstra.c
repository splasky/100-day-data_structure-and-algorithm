#include "../include/dbg.h"
#include "../include/graph.h"
#include "unit.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int int_compare(const void* a, const void* b)
{
    const int x = *((int*)a);
    const int y = *((int*)b);
    return (x > y) - (y > x);
}

static void graph_print_cb(const void* a) { printf("%d\t", *(int*)a); }

static inline void print_result(int dist[], int num_of_vertices)
{
    /* print dist array */
    printf("print result:\n");
    for (int i = 0; i < num_of_vertices; ++i) {
        printf("%d\t", i + 1);
    }
    printf("\n");

    for (int i = 0; i < num_of_vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("∞\t");
        } else {
            printf("%d\t", dist[i]);
        }
    }
    printf("\n");
}

void Graph_dijkstra(Graph* graph, const int src)
{
    const int num_of_vertices = graph->num_of_vertices + 1;

    int selected[num_of_vertices + 1];
    int dist[num_of_vertices + 1];

    for (int i = 0; i < num_of_vertices + 1; ++i) {
        selected[i] = 0;
        /* init distance */
        dist[i] = INT_MAX;
    }

    /* mark source as i */
    selected[src] = 1;
    dist[src] = 0;
    int prev_min_len = 0;

    AdjList* curr_list = graph->adjlist;
    while (curr_list) {
        prev_min_len = dist[*(int*)curr_list->key];
        AdjListNode* curr_node = curr_list->head;
        while (curr_node) {
            int curr_weight = curr_node->weight;

            if (dist[*(int*)curr_node->dest] > curr_weight + prev_min_len) {
                dist[*(int*)curr_node->dest] = curr_weight + prev_min_len;
            }

            curr_node = curr_node->next;
        }

        selected[*(int*)curr_list->key] = 1;
        curr_list = curr_list->next;
    }

    print_result(dist, graph->num_of_vertices + 1);

    for (int i = 0; i < num_of_vertices + 1; i++) {
        printf("%d\t", selected[i]);
    }
    printf("\n");
}

TEST(test_dijkstra)
{
    int edge[7][3] = { { 1, 2, 35 }, { 1, 3, 90 }, { 2, 3, 45 }, { 2, 4, 30 },
        { 3, 5, 25 }, { 4, 5, 45 }, { 5, 6, 200 } };

    Graph* graph = Graph_create(int_compare);

    for (int i = 0; i < 7; ++i) {
        int* source = malloc(sizeof(int));
        *source = edge[i][0];
        int* dest = malloc(sizeof(int));
        *dest = edge[i][1];
        Graph_add_edge(graph, source, dest, edge[i][2]);
    }

    printf("Print graph:\n");
    Graph_print_graph(graph, graph_print_cb);
    printf("count point 1 to other points distance:\n");
    Graph_dijkstra(graph, 1);
    Graph_destory(graph);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_dijkstra);
    return NULL;
}

RUN_TESTS(all_tests);
