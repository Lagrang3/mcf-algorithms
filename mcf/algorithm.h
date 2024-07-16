#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <mcf/network.h>

bool dijkstra(const tal_t *ctx, const struct graph *graph,
	      const struct node source, const struct node destination,
	      bool prune, const s64 *capacity, const s64 *cost,
	      struct arc *prev, s64 *distance);

#endif /* ALGORITHM_H */
