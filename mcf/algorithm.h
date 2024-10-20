#ifndef ALGORITHM_H
#define ALGORITHM_H

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
 * and balance constraints:
 * 	-balance[source] = balance[destination] = amount
 * 	balance[node] = 0 for every other node
 *
 * It uses simple augmenting paths algorithm.
 *
 * input:
 * @ctx: tal context for internal allocation
 * @graph: topological information of the graph
 * @source: source node
 * @destination: destination node
 * @capacity: arcs capacity
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

/* Computes the balance of a node.
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
 * and balance constraints:
 * 	-balance[source] = balance[destination] = amount
 * 	balance[node] = 0 for every other node
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

#endif /* ALGORITHM_H */
