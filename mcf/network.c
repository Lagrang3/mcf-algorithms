#include <mcf/network.h>

static void graph_push_outbound_arc(struct graph *graph, const struct arc arc,
				    const struct node node) {
	assert(arc.idx < tal_count(graph->arc_tail));
	graph->arc_tail[arc.idx] = node;

	assert(node.idx < tal_count(graph->node_adjacency_first));
	const struct arc first_arc = graph->node_adjacency_first[node.idx];

	assert(arc.idx < tal_count(graph->node_adjacency_next));
	graph->node_adjacency_next[arc.idx] = first_arc;

	assert(node.idx < tal_count(graph->node_adjacency_first));
	graph->node_adjacency_first[node.idx] = arc;
}

struct arc graph_add_arc(struct graph *graph, const struct arc arc,
			 const struct node from, const struct node to) {
	assert(from.idx < graph->max_num_nodes);
	assert(to.idx < graph->max_num_nodes);
	assert(arc.idx < graph->max_num_arcs);

	graph_push_outbound_arc(graph, arc, from);
	graph_push_outbound_arc(graph, arc_dual(graph, arc), to);

	return arc;
}

struct graph *graph_new(const tal_t *ctx, const size_t max_num_nodes,
			const size_t max_num_arcs, const size_t arc_dual_bit) {
	struct graph *graph;
	graph = tal(ctx, struct graph);

	// bad allocation of graph
	if (!graph) return graph;

	graph->max_num_arcs = max_num_arcs;
	graph->max_num_nodes = max_num_nodes;
	graph->arc_dual_bit = arc_dual_bit;

	graph->arc_tail = tal_arr(graph, struct node, graph->max_num_arcs);
	graph->node_adjacency_first =
	    tal_arr(graph, struct arc, graph->max_num_nodes);
	graph->node_adjacency_next =
	    tal_arr(graph, struct arc, graph->max_num_arcs);

	if (!graph->arc_tail || !graph->node_adjacency_first ||
	    !graph->node_adjacency_next) {
		return tal_free(graph);
	}

	for (size_t i = 0; i < tal_count(graph->arc_tail); i++)
		graph->arc_tail[i] = node_obj(INVALID_INDEX);
	for (size_t i = 0; i < tal_count(graph->node_adjacency_first); i++)
		graph->node_adjacency_first[i] = arc_obj(INVALID_INDEX);
	for (size_t i = 0; i < tal_count(graph->node_adjacency_next); i++)
		graph->node_adjacency_next[i] = arc_obj(INVALID_INDEX);

	return graph;
}

