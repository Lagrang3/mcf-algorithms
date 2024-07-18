#include <assert.h>
#include <ccan/tal/tal.h>
#include <inttypes.h>
#include <mcf/algorithm.h>
#include <mcf/network.h>
#include <stdio.h>

#define MAX_NODES 256
#define MAX_ARCS 256
#define DUAL_BIT 7

int main() {
	printf("Allocating a memory context\n");
	tal_t *ctx = tal(NULL, tal_t);
	assert(ctx);

	printf("Allocating a graph\n");
	struct graph *graph = graph_new(ctx, MAX_NODES, MAX_ARCS, DUAL_BIT);
	assert(graph);

	s64 *capacity = tal_arrz(ctx, s64, MAX_ARCS);

	graph_add_arc(graph, arc_obj(0), node_obj(1), node_obj(2));
	capacity[0] = 1;
	graph_add_arc(graph, arc_obj(1), node_obj(1), node_obj(3));
	capacity[1] = 4;
	graph_add_arc(graph, arc_obj(2), node_obj(2), node_obj(4));
	capacity[2] = 1;
	graph_add_arc(graph, arc_obj(3), node_obj(2), node_obj(5));
	capacity[3] = 1;
	graph_add_arc(graph, arc_obj(4), node_obj(3), node_obj(5));
	capacity[4] = 4;
	graph_add_arc(graph, arc_obj(5), node_obj(4), node_obj(6));
	capacity[5] = 1;
	graph_add_arc(graph, arc_obj(6), node_obj(6), node_obj(10));
	capacity[6] = 1;
	graph_add_arc(graph, arc_obj(7), node_obj(5), node_obj(10));
	capacity[7] = 4;

	struct node src = {.idx = 1};
	struct node dst = {.idx = 10};

	bool result = simple_feasibleflow(ctx, graph, src, dst, capacity, 5);
	assert(result);

	assert(node_balance(graph, src, capacity) == -5);
	assert(node_balance(graph, dst, capacity) == 5);

	for (u32 i = 2; i < 10; i++)
		assert(node_balance(graph, node_obj(i), capacity) == 0);

	printf("Freeing memory\n");
	ctx = tal_free(ctx);
	return 0;
}

