#include <assert.h>
#include <ccan/tal/tal.h>
#include <inttypes.h>
#include <mcf/priorityqueue.h>
#include <stdio.h>

void priorityqueue_show(struct priorityqueue *q) {
	printf("size of queue: %zu\n", priorityqueue_size(q));
	printf("empty?: %s\n", priorityqueue_empty(q) ? "true" : "false");
	if (!priorityqueue_empty(q))
		printf("top of the queue: %" PRIu32 "\n", priorityqueue_top(q));
	const s64 *value = priorityqueue_value(q);
	for (u32 i = 0; i < priorityqueue_maxsize(q); i++) {
		printf("(%" PRIu32 ", %" PRIi64 ")", i, value[i]);
	}

	printf("\n\n");
}

int main() {
	printf("Hello world!\n");

	printf("Allocating a memory context\n");
	tal_t *ctx = tal(NULL, tal_t);
	assert(ctx);

	printf("Allocating a priorityqueue\n");
	struct priorityqueue *q;
	q = priorityqueue_new(ctx, 5);
	assert(q);

	// reset all values
	priorityqueue_init(q);
	priorityqueue_show(q);

	priorityqueue_update(q, 0, 10);
	priorityqueue_show(q);

	priorityqueue_update(q, 0, 3);
	priorityqueue_show(q);

	priorityqueue_update(q, 1, 3);
	priorityqueue_show(q);

	priorityqueue_update(q, 1, 5);
	priorityqueue_show(q);

	priorityqueue_update(q, 1, -1);
	priorityqueue_show(q);

	priorityqueue_pop(q);
	priorityqueue_show(q);

	priorityqueue_update(q, 1, 0);
	priorityqueue_show(q);

	priorityqueue_update(q, 4, -10);
	priorityqueue_show(q);

	priorityqueue_pop(q);
	priorityqueue_show(q);

	printf("Freeing memory\n");
	ctx = tal_free(ctx);
	return 0;
}
