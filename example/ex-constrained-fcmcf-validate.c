#include <assert.h>
#include <ccan/tal/tal.h>
#include <inttypes.h>
#include <mcf/algorithm.h>
#include <mcf/graph.h>
#include <stdio.h>

static int next_bit(s64 x) {
	int b;
	for (b = 0; (1LL << b) <= x; b++)
		;
	return b;
}

static bool solve_case(const tal_t *ctx) {
	static int c = 0;
	c++;
	tal_t *this_ctx = tal(ctx, tal_t);

	int N_nodes, N_arcs, N_constraints;
	scanf("%d %d %d\n", &N_nodes, &N_arcs, &N_constraints);
	// printf("solving test case Nnodes = %d Narcs = %d Nconstraints = %d\n",
	//	N_nodes, N_arcs, N_constraints);

	if (N_nodes == 0 && N_arcs == 0 && N_constraints==0) goto fail;
	
	
	const int MAX_NODES = N_nodes;
	const int DUAL_BIT = next_bit(N_arcs);
	const int MAX_ARCS = (1LL << DUAL_BIT) | N_arcs;

	struct graph *graph = graph_new(ctx, MAX_NODES, MAX_ARCS, DUAL_BIT);
	assert(graph);

	s64 *capacity = tal_arrz(ctx, s64, MAX_ARCS);
	s64 *excess = tal_arrz(ctx, s64, MAX_NODES);
	
	s64 **cost = tal_arrz(ctx, s64*, N_constraints);
	s64 **fixedcost = tal_arrz(ctx, s64*, N_constraints);
	s64 *bound = tal_arrz(ctx, s64, N_constraints);
	
	for(u32 i=0;i<N_constraints;i++){
		cost[i] = tal_arrz(ctx, s64, MAX_ARCS);
		fixedcost[i] = tal_arrz(ctx, s64, MAX_ARCS);
	}

	for (u32 i = 0; i < N_arcs; i++) {
		u32 from, to;
		scanf("%" PRIu32 " %" PRIu32 " %" PRIi64,
		      &from, &to, &capacity[i]);
		
		struct arc arc = {.idx = i};
		graph_add_arc(graph, arc, node_obj(from), node_obj(to));
		struct arc dual = arc_dual(graph, arc);

		for (u32 k = 0; k < N_constraints; k++) {
			scanf("%" PRIi64 " %" PRIi64, &cost[k][i],
			      &fixedcost[k][i]);
			cost[k][dual.idx] = -cost[k][i];
		}
	}

	for (u32 k = 0; k < N_constraints; k++) {
		scanf("%" PRIi64, &bound[k]);
	}

	int source, destination;
	s64 amount;
	scanf("%d %d %ld", &source, &destination, &amount);
	
	// printf("source=%d destination=%d amount=%ld\n", source, destination, amount);
	
	struct node src = {.idx = source};
	struct node dst = {.idx = destination};

	excess[src.idx] = amount;
	excess[dst.idx] = -amount;
	
	s64 best_cost;
	scanf("%"PRIi64, &best_cost);
	
	bool result_constrained = solve_constrained_fcnfp(
		this_ctx, graph, excess, capacity, N_constraints,
		cost, fixedcost, bound, 0.10, 100);
	const s64 cost_constrained = flow_cost_with_charge(graph, capacity,
		cost[0], fixedcost[0]);
	int satisfied_constraints = flow_satisfy_constraints(
	    graph, capacity, N_constraints, cost,
	    fixedcost, bound);
	assert(result_constrained);
	
	assert(node_balance(graph, src, capacity) == -amount);
	assert(node_balance(graph, dst, capacity) == amount);

	for (u32 i = 0; i < N_nodes; i++){
		if(i==src.idx || i==dst.idx) continue;
		assert(node_balance(graph, node_obj(i), capacity) == 0);
	}

	for (u32 i = 0; i < N_nodes; i++)
		assert(excess[i] == 0);
	
	
	bool result_unconstrained = solve_fcnfp_approximate(this_ctx, graph,
			     excess, capacity, cost[0],
			     fixedcost[0],
			     100);
	const s64 cost_unconstrained = flow_cost_with_charge(graph, capacity,
		cost[0], fixedcost[0]);
	assert(result_unconstrained);
	int satisfied_UNconstraints = flow_satisfy_constraints(
	    graph, capacity, N_constraints, cost, fixedcost, bound);

	printf("%lld %d %lld %d %lld %d\n",
	       best_cost,
	       N_constraints,
	       cost_unconstrained,
	       satisfied_UNconstraints,
	       cost_constrained,
	       satisfied_constraints);

	// read the proof and skip it
	for (u32 i = 0; i < N_arcs; i++) {
		long long x;
		scanf("%lf", &x);
	}

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
