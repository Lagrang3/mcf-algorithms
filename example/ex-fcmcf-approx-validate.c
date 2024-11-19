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

s64 compute_cost(const struct graph *graph, const s64 *res_capacity,
		 const s64 *cost, const s64 *fixedcost)
{
	const size_t max_num_arcs = graph_max_num_arcs(graph);
	s64 fix_cost = 0;

	for (u32 i = 0; i < max_num_arcs; i++) {
		struct arc arc = {.idx = i};
		struct arc dual = arc_dual(graph, arc);

		if (!arc_enabled(graph, arc) || arc_is_dual(graph, arc))
			continue;

		if (res_capacity[dual.idx] > 0)
			fix_cost += fixedcost[i];
	}

	return fix_cost + flow_cost(graph, res_capacity, cost);
}

double compute_mean_over_arcs(const struct graph *graph, const s64 *A)
{
	const size_t max_num_arcs = graph_max_num_arcs(graph);
	int count = 0;
	double sum = 0;
	for (struct arc arc = {.idx = 0}; arc.idx < max_num_arcs; arc.idx++) {
		if (!arc_enabled(graph, arc) || arc_is_dual(graph, arc))
			continue;
		count++;
		sum += A[arc.idx];
	}
	return sum / count;
}

s64 solve_mcf(const tal_t *ctx, const struct graph *graph, const s64 *excess, 
const s64 *capacity, const s64 *cost, const s64 *fixedcost){
	tal_t *this_ctx = tal(ctx, tal_t);
	const size_t max_num_nodes = graph_max_num_nodes(graph);
	const size_t max_num_arcs = graph_max_num_arcs(graph);

	s64 *tmp_potential = tal_arrz(this_ctx, s64, max_num_nodes);
	s64 *tmp_excess = tal_arrz(this_ctx, s64, max_num_nodes);
	s64 *tmp_capacity = tal_arrz(this_ctx, s64, max_num_arcs);
	s64 *tmp_cost = tal_arrz(this_ctx, s64, max_num_arcs);
	
	for (u32 i = 0; i < max_num_arcs; i++) {
		struct arc arc = {.idx = i};
		struct arc dual = arc_dual(graph, arc);

		if (!arc_enabled(graph, arc) || arc_is_dual(graph, arc))
			continue;

		tmp_cost[arc.idx] =
		    cost[arc.idx] + fixedcost[arc.idx] / capacity[arc.idx];
		tmp_cost[dual.idx] = -tmp_cost[arc.idx];
	}

	memcpy(tmp_capacity, capacity, sizeof(s64)*max_num_arcs); 
	memcpy(tmp_excess, excess, sizeof(s64)*max_num_nodes);
	
	bool result = mcf_refinement(ctx, graph, tmp_excess, tmp_capacity, cost, tmp_potential);
	assert(result);
	
	const s64 total_cost = compute_cost(graph, tmp_capacity, cost, fixedcost);
	
	tal_free(this_ctx);
	return total_cost;
}

static bool solve_case(const tal_t *ctx) {
	static int c = 0;
	c++;
	tal_t *this_ctx = tal(ctx, tal_t);

	int N_nodes, N_arcs;
	scanf("%d %d\n", &N_nodes, &N_arcs);
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
	
	/* ignoring fixed charge, what do we obtain? */
	const s64 mcf_solution = solve_mcf(this_ctx, graph, excess, capacity, cost, fixedcost);

	bool result = solve_fcnfp_approximate(this_ctx, graph,
			     excess, capacity, cost,
			     fixedcost,
			     100);
	assert(result);
	
	assert(node_balance(graph, src, capacity) == -amount);
	assert(node_balance(graph, dst, capacity) == amount);

	for (u32 i = 2; i < N_nodes; i++)
		assert(node_balance(graph, node_obj(i), capacity) == 0);

	for (u32 i = 0; i < N_nodes; i++)
		assert(excess[i] == 0);

	const double mean_fixedcost = compute_mean_over_arcs(graph, fixedcost);
	const double mean_cost = compute_mean_over_arcs(graph, cost);
	const s64 dynslope_solution =
	    compute_cost(graph, capacity, cost, fixedcost);
	printf("%lld %lld %lld %lf %lf %lld\n",
	       best_cost,
	       mcf_solution,
	       dynslope_solution,
	       mean_fixedcost,
	       mean_cost,
	       amount);

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

