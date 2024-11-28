#ifndef ALGORITHM_H
#define ALGORITHM_H

/* Implementation of network algorithms: shortests path, minimum cost flow, etc.
 */

#include <mcf/graph.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/* Search any path from source to destination using Breadth First Search.
 *
 * input:
 * @ctx: tal allocator,
 * @graph: graph of the network,
 * @source: source node,
 * @destination: destination node,
 * @capacity: arcs capacity
 * @cap_threshold: an arc i is traversable if capacity[i]>=cap_threshold
 *
 * output:
 * @prev: prev[i] is the arc that leads to node i for an optimal solution, it
 * @return: true if the destination node was reached.
 *
 * precondition:
 * |capacity|=graph_max_num_arcs
 * |prev|=graph_max_num_nodes
 *
 * The destination is only used as a stopping condition, if destination is
 * passed with an invalid idx then the algorithm will produce a discovery tree
 * of all reacheable nodes from the source.
 * */
bool BFS_path(const tal_t *ctx, const struct graph *graph,
	      const struct node source, const struct node destination,
	      const s64 *capacity, const s64 cap_threshold, struct arc *prev);


/* Computes the distance from the source to every other node in the network
 * using Dijkstra's algorithm.
 *
 * input:
 * @ctx: tal context for internal allocation
 * @graph: topological information of the graph
 * @source: source node
 * @destination: destination node
 * @prune: if prune is true the algorithm stops when the optimal path is found
 * for the destination node
 * @capacity: arcs capacity
 * @cap_threshold: an arc i is traversable if capacity[i]>=cap_threshold
 * @cost: arc's cost
 * @potential: nodes' potential, ie. reduced cost for an arc
 * 	c_ij = cost_ij - potential[i] + potential[j]
 *
 * output:
 * @prev: for each node, this is the arc that was used to arrive to it, this can
 * be used to reconstruct the path from the destination to the source,
 * @distance: node's best distance
 * returns true if an optimal path is found for the destination, false otherwise
 *
 * precondition:
 * |capacity|=|cost|=graph_max_num_arcs
 * |prev|=|distance|=graph_max_num_nodes
 * cost[i]>=0
 * if prune is true the destination must be valid
 * */
bool dijkstra_path(const tal_t *ctx, const struct graph *graph,
		   const struct node source, const struct node destination,
		   bool prune, const s64 *capacity, const s64 cap_threshold,
		   const s64 *cost, const s64 *potential, struct arc *prev,
		   s64 *distance);


/* Finds any flow that satisfy the capacity constraints:
 * 	flow[i] <= capacity[i]
 * and supply/demand constraints:
 * 	supply[source] = demand[destination] = amount
 * 	supply/demand[node] = 0 for every other node
 *
 * It uses simple augmenting paths algorithm.
 *
 * input:
 * @ctx: tal context for internal allocation
 * @graph: topological information of the graph
 * @source: source node
 * @destination: destination node
 * @capacity: arcs capacity
 * @amount: supply/demand
 *
 * output:
 * @capacity: residual capacity
 * returns true if the balance constraint can be satisfied
 *
 * precondition:
 * |capacity|=graph_max_num_arcs
 * amount>=0
 * */
bool simple_feasibleflow(const tal_t *ctx, const struct graph *graph,
			 const struct node source,
			 const struct node destination, s64 *capacity,
			 s64 amount);


/* Computes the balance of a node, ie. the incoming flows minus the outgoing.
 *
 * @graph: topology
 * @node: node
 * @capacity: capacity in the residual sense, not the constrain capacity
 *
 * This works because in the adjacency list an arc wich is dual is associated
 * with an inconming arc i, then we add this flow, while an arc which is not
 * dual corresponds to and outgoing flow that we need to substract.
 * The flow on the arc i (not dual) is computed as:
 * 	flow[i] = residual_capacity[i_dual],
 * while the constrain capacity is
 * 	capacity[i] = residual_capacity[i] + residual_capacity[i_dual] */
s64 node_balance(const struct graph *graph, const struct node node,
		 const s64 *capacity);


/* Finds the minimum cost flow that satisfy the capacity constraints:
 * 	flow[i] <= capacity[i]
 * and supply/demand constraints:
 * 	supply[source] = demand[destination] = amount
 * 	supply/demand[node] = 0 for every other node
 *
 * It uses successive shortest path algorithm.
 *
 * input:
 * @ctx: tal context for internal allocation
 * @graph: topological information of the graph
 * @source: source node
 * @destination: destination node
 * @capacity: arcs capacity
 * @amount: desired balance at the destination
 * @cost: cost per unit of flow
 *
 * output:
 * @capacity: residual capacity
 * returns true if the balance constraint can be satisfied
 *
 * precondition:
 * |capacity|=graph_max_num_arcs
 * |cost|=graph_max_num_arcs
 * amount>=0
 * */
bool simple_mcf(const tal_t *ctx, const struct graph *graph,
		const struct node source, const struct node destination,
		s64 *capacity, s64 amount, const s64 *cost);

/* Compute the cost of a flow in the network.
 *
 * @graph: network topology
 * @capacity: residual capacity (encodes the flow)
 * @cost: cost per unit of flow */
s64 flow_cost(const struct graph *graph, const s64 *capacity, const s64 *cost);

/* Take an existent flow and find an optimal redistribution:
 *
 * inputs:
 * @ctx: tal context for internal allocation,
 * @graph: topological information of the graph,
 * @excess: supply/demand of nodes,
 * @capacity: residual capacity in the arcs,
 * @cost: cost per unit of flow for every arc,
 * @potential: node potential,
 *
 * outputs:
 * @excess: all values become zero if there exist a feasible solution,
 * @capacity: encodes the resulting flow,
 * @potential: the potential that proves the solution using the complementary
 * slackness optimality condition.
 * */
bool mcf_refinement(const tal_t *ctx,
		    const struct graph *graph,
		    s64 *excess,
		    s64 *capacity,
		    const s64 *cost,
		    s64 *potential);

/* An approximate solver to the Fixed Charge Network Flow Problem (FCNFP).
 * Based on dynamic slope scaling by Kim et Pardalos,
 * Operations Research Letters 24 (1999) 195--203
 *
 * Given a graph G=(N,A)
 *
 * minimize f(x) = sum_{(i,j) in A} c[i,j] x[i,j] + s[i,j] y[i,j]
 *
 * where:
 * 	y[i,j] = x[i,j]>0 ? 1 : 0;
 * 	// y indicates if the arc is active or not
 * 	// s[i,j]>=0 is constant arc activation cost
 * 	// c[i,j]>= is a cost proportional to the flow
 *
 * such that:
 * 	0<= x[i,j] <= u[i,j] // capacity constraints
 *
 * 	sum_{(i,j)} x[i,j] - sum_{(j,i)} x[j,i] = b[i]
 * 	// flow conservation constraints
 * 	// b[i]>0 is a supply node, b[i]<0 is a demand node
 *
 * */
bool solve_fcnfp(const tal_t *ctx, const struct graph *graph, s64 *excess,
		 s64 *capacity, const s64 *cost, const s64 *charge,
		 const size_t max_num_iterations);

/* Similar to solve_fcnfp, but with additional constraints.
 *
 * Given a graph G=(N,A) and a list of cost functions z[num_constraints]
 *
 * minimize z[0](x,y) = sum_{(i,j) in A} cost[0][i,j] * x[i,j] + charge[0][i,j] * y[i,j]
 *
 * such that:
 * 	// capacity constraints
 * 	0<= x[i,j] <= capacity[i,j]
 *
 * 	// flow conservation constraints
 * 	sum_{(i,j)} x[i,j] - sum_{(j,i)} x[j,i] = excess[i]
 *
 * 	// coupling between x and y
 * 	0<= x[i,j] <= capacity[i,j] * y[i,j]
 *
 * 	// other constraints
 * 	z[k](x,y) = sum_{(i,j) in A} cost[k][i,j] * x[i,j] + charge[k][i,j] * y[i,j]
 * 		<= bound[k]
 * 	for 1<=k<num_constraints
 *
 * ctx: allocator
 * graph: problem's topology, nodes, arcs and duals
 * excess[num_nodes]: defines the problem's sources and sinks, ie.
 * 	a source is a node n for which excess[n]>0, for a sink excess[n]<0 and
 * 	routing nodes have excess[n]=0. The problem is solved when excess[n]=0
 * 	for every node.
 * capacity[num_arcs]: the residual capacity of every arc and
 * 	corresponding dual, here we write the final solution.
 * num_constraints: the number of cost functions in our problem, one is the
 * 	objective function (that we want to minimize) and the rest are side
 * 	constraints.
 * cost[num_constraints][num_arcs]: proportional cost, ie. cost per
 * 	unit of flow for every cost function and arc.
 * charge[num_constraints][num_arcs]: fixed charge, ie. activation
 *	cost for every cost function and arc.
 * bound[num_constraints]: defines the problem constraints,
 * 	cost function k cannot exceed bound[k].
 * tolerance: we stop once we find a feasible solution that deviates at most
 *	this number from the optimal solution, ie.
 *		|C-C*|/C* < tolerance
 *	where C* is the optimal solution and C is the approximative solution.
 * max_num_iterations: we stop if we reach this number of iterations
 *
 * */
bool solve_constrained_fcnfp(const tal_t *ctx, const struct graph *graph,
			     s64 *excess, s64 *capacity,
			     const size_t num_constraints, s64 **cost,
			     s64 **charge, const s64 *bound,
			     const double tolerance,
			     const size_t max_num_iterations);

/* Helper, count the number of satisfied constraints */
int flow_satisfy_constraints(const struct graph *graph, s64 *capacity,
			     const size_t num_constraints, s64 **cost,
			     s64 **charge, const s64 *bound);

/* Helper to obtain the cost of flow for a given cost function with activation
 * costs. */
s64 flow_cost_with_charge(const struct graph *graph, const s64 *capacity,
			  const s64 *cost, const s64 *charge);

#endif /* ALGORITHM_H */
