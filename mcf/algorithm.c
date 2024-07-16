#include <ccan/bitmap/bitmap.h>
#include <ccan/tal/tal.h>
#include <mcf/algorithm.h>
#include <mcf/priorityqueue.h>

/* Computes the distance from the source to every other node in the network.
 *
 * input:
 * @ctx: tal context for internal allocation
 * @graph: topological information of the graph
 * @source: source node
 * @destination: destination node
 * @prune: if prune is true the algorithm stops when the optimal path is found
 * for the destination node
 * @capacity: arc's capacity, only arcs with capacity>0 are traversable
 * @cost: arc's cost
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
bool dijkstra(const tal_t *ctx, const struct graph *graph,
	      const struct node source, const struct node destination,
	      bool prune, const s64 *capacity, const s64 *cost,
	      struct arc *prev, s64 *distance) {
	bool target_found = false;
	const size_t max_num_arcs = graph_max_num_arcs(graph);
	const size_t max_num_nodes = graph_max_num_nodes(graph);

	// check preconditions
	if (!graph || source.idx == INVALID_INDEX || !cost || !capacity ||
	    !prev || !distance)
		goto finish;

	/* if prune is true then the destination cannot be invalid */
	if (destination.idx == INVALID_INDEX && prune) goto finish;

	if (tal_count(cost) != max_num_arcs ||
	    tal_count(capacity) != max_num_arcs ||
	    tal_count(prev) != max_num_nodes ||
	    tal_count(distance) != max_num_nodes)
		goto finish;

	tal_t *this_ctx = tal(ctx, tal_t);

	// FIXME: maybe this is unnecessary
	bitmap *visited = tal_arrz(this_ctx, bitmap,
				   BITMAP_NWORDS(graph_max_num_nodes(graph)));

	if (!visited)
		// bad allocation
		goto finish;

	for (size_t i = 0; i < tal_count(prev); ++i)
		prev[i].idx = INVALID_INDEX;

	struct priorityqueue *q;
	q = priorityqueue_new(this_ctx, max_num_nodes);
	const s64 *const dijkstra_distance = priorityqueue_value(q);

	priorityqueue_init(q);
	priorityqueue_update(q, source.idx, 0);

	while (!priorityqueue_empty(q)) {
		const u32 cur = priorityqueue_top(q);
		priorityqueue_pop(q);

		// FIXME: maybe this is unnecessary
		if (bitmap_test_bit(visited, cur)) continue;
		bitmap_set_bit(visited, cur);

		if (cur == destination.idx) {
			target_found = true;
			if (prune) break;
		}

		for (struct arc arc =
			 node_adjacency_begin(graph, node_obj(cur));
		     !node_adjacency_end(arc);
		     arc = node_adjacency_next(graph, arc)) {
			// check if this arc is traversable
			if (capacity[arc.idx] <= 0) continue;

			const struct node next = arc_head(graph, arc);

			const s64 cij = cost[arc.idx];

			// Dijkstra only works with non-negative weights
			assert(cij >= 0);

			if (dijkstra_distance[next.idx] <=
			    dijkstra_distance[cur] + cij)
				continue;

			priorityqueue_update(q, next.idx,
					     dijkstra_distance[cur] + cij);
			prev[next.idx] = arc;
		}
	}
	for (size_t i = 0; i < max_num_nodes; i++)
		distance[i] = dijkstra_distance[i];

finish:
	tal_free(this_ctx);
	return target_found;
}
