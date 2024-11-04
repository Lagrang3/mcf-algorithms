#include <assert.h>
#include <ccan/tal/tal.h>
#include <inttypes.h>
#include <mcf/algorithm.h>
#include <mcf/graph.h>
#include <stdio.h>

int next_bit(s64 x) {
	int b;
	for (b = 0; (1LL << b) <= x; b++)
		;
	return b;
}

static bool solve_case(const tal_t *ctx) {
	static int c = 0;
	c++;
	tal_t *this_ctx = tal(ctx, tal_t);

	int N_nodes, N_arcs;
	scanf("%d %d\n", &N_nodes, &N_arcs);
	printf("solving case nodes=%d arcs=%d\n", N_nodes, N_arcs);
	if (N_nodes == 0 && N_arcs == 0) goto fail;

	const int MAX_NODES = N_nodes;
	const int DUAL_BIT = next_bit(N_arcs);
	const int MAX_ARCS = (1LL << DUAL_BIT) | N_arcs;

	struct graph *graph = graph_new(ctx, MAX_NODES, MAX_ARCS, DUAL_BIT);
	assert(graph);

	s64 *capacity = tal_arrz(ctx, s64, MAX_ARCS);
	s64 *cost = tal_arrz(ctx, s64, MAX_ARCS);
	s64 *fixedcost = tal_arrz(ctx, s64, MAX_ARCS);
	s64 *excess = tal_arrz(ctx, s64, MAX_NODES);

	for (u32 i = 0; i < N_arcs; i++) {
		u32 from, to;
		scanf("%" PRIu32 " %" PRIu32 " %" PRIi64 " %" PRIi64
		      " %" PRIi64,
		      &from, &to, &capacity[i], &cost[i], &fixedcost[i]);

		struct arc arc = {.idx = i};
		graph_add_arc(graph, arc, node_obj(from), node_obj(to));

		struct arc dual = arc_dual(graph, arc);
		cost[dual.idx] = -cost[i];
	}
	struct node src = {.idx = 0};
	struct node dst = {.idx = 1};

	s64 amount, best_cost;
	scanf("%" PRIi64 " %" PRIi64, &amount, &best_cost);
	excess[src.idx] = amount;
	excess[dst.idx] = -amount;
	
	bool result = solve_fcnfp_approximate(ctx, graph, excess, capacity, cost, fixedcost);
	assert(result);

	assert(node_balance(graph, src, capacity) == -amount);
	assert(node_balance(graph, dst, capacity) == amount);

	for (u32 i = 2; i < N_nodes; i++)
		assert(node_balance(graph, node_obj(i), capacity) == 0);

	for (u32 i = 0; i < N_nodes; i++)
		assert(excess[i] == 0);

	s64 fix_cost = 0;
	for (u32 i = 0; i < N_arcs; i++) {
		struct arc arc = {.idx = i};
		struct arc dual = arc_dual(graph, arc);

		if (capacity[dual.idx] > 0)
			fix_cost += fixedcost[i];
	}

	const s64 total_cost = fix_cost + flow_cost(graph, capacity, cost);
	printf("Found solution total_cost=%lld vs official solution=%lld\n", total_cost, best_cost);
	// assert(total_cost == best_cost);

	tal_free(this_ctx);
	return true;

fail:
	tal_free(this_ctx);
	return false;
}

int main() {
	tal_t *ctx = tal(NULL, tal_t);
	assert(ctx);

	/* One test case after another. The last test case has N number of nodes
	 * and arcs equal to 0 and must be ignored. */
	while (solve_case(ctx))
		;

	ctx = tal_free(ctx);
	return 0;
}

