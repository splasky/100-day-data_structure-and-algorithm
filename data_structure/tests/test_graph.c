#include "../include/dbg.h"
#include "../include/graph.h"
#include "unit.h"

Graph* graph = NULL;

static int int_compare(const void* a, const void* b)
{
    const int x = *((int*)a);
    const int y = *((int*)b);
    return (x > y) - (y > x);
}

TEST(test_create)
{
    int edge[12][2] = { { 1, 2 }, { 1, 3 }, { 2, 1 }, { 3, 1 }, { 2, 3 }, { 3, 2 },
        { 2, 4 }, { 4, 2 }, { 3, 5 }, { 5, 3 }, { 4, 5 }, { 5, 4 } };

    graph = Graph_create(int_compare);
    for (int i = 0; i < 12; ++i) {
        int* source = malloc(sizeof(int));
        *source = edge[i][0];
        int* dest = malloc(sizeof(int));
        *dest = edge[i][1];
        Graph_add_edge(graph, source, dest, 0);
    }

    return NULL;
}

void graph_print_cb(void* source) { printf("%d\t", *(int*)source); }

TEST(test_print_graph)
{
    Graph_print_graph(graph, graph_print_cb);
    return NULL;
}

TEST(test_destory)
{
    Graph_destory(graph);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_create);
    unit_run_test(test_print_graph);
    unit_run_test(test_destory);
    return NULL;
}

RUN_TESTS(all_tests);
