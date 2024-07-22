#include <assert.h>
#include <ccan/tal/tal.h>
#include <inttypes.h>
#include <mcf/algorithm.h>
#include <mcf/network.h>
#include <stdio.h>

int next_bit(s64 x) {
	int b;
	for (b = 0; (1LL << b) < x; b++)
		;
	return b;
}

int main() {
	tal_t *ctx = tal(NULL, tal_t);
	assert(ctx);

	int N_nodes, N_arcs;
	scanf("%d %d\n", &N_nodes, &N_arcs);

	const int MAX_NODES = N_nodes;
	const int DUAL_BIT = next_bit(N_arcs);
	const int MAX_ARCS = (1LL << DUAL_BIT) | N_arcs;

	struct graph *graph = graph_new(ctx, MAX_NODES, MAX_ARCS, DUAL_BIT);
	assert(graph);

	s64 *capacity = tal_arrz(ctx, s64, MAX_ARCS);
	s64 *cost = tal_arrz(ctx, s64, MAX_ARCS);

	for (u32 i = 0; i < N_arcs; i++) {
		u32 from, to;
		scanf("%" PRIu32 " %" PRIu32 " %" PRIi64 " %" PRIi64, &from,
		      &to, &capacity[i], &cost[i]);

		struct arc arc = {.idx = i};
		graph_add_arc(graph, arc, node_obj(from), node_obj(to));

		struct arc dual = arc_dual(graph, arc);
		cost[dual.idx] = -cost[i];
	}
	struct node src = {.idx = 0};
	struct node dst = {.idx = 1};

	s64 amount, best_cost;
	scanf("%" PRIi64 " %" PRIi64, &amount, &best_cost);

	bool result = simple_mcf(ctx, graph, src, dst, capacity, amount, cost);
	assert(result);

	assert(node_balance(graph, src, capacity) == -amount);
	assert(node_balance(graph, dst, capacity) == amount);

	for (u32 i = 2; i < N_nodes; i++)
		assert(node_balance(graph, node_obj(i), capacity) == 0);

	const s64 total_cost = flow_cost(graph, capacity, cost);
	assert(total_cost == best_cost);

	ctx = tal_free(ctx);
	return 0;
}

