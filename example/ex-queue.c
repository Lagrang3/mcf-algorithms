#include <stdio.h>
#include <time.h>

#include "mcf/queue.h"

QUEUE_DEFINE_TYPE(int, iqueue);

struct queue_data {
	int x;
	struct lqueue_link ql;
};

int test_pure_lqueue(int n)
{
	int sum = 0;
	LQUEUE(struct queue_data, ql) q = LQUEUE_INIT;
	struct queue_data *qdata;

	for (int i = 1; i <= n; i++) {
		qdata = tal(NULL, struct queue_data);
		qdata->x = i;
		lqueue_enqueue(&q, qdata);
	}
	while (!lqueue_empty(&q)) {
		qdata = lqueue_dequeue(&q);
		sum += qdata->x;
		tal_free(qdata);
	}
	return sum;
}

int test_pure_iqueue(int n)
{
	int sum = 0;
	struct iqueue q;
	iqueue_init(&q, NULL);
	for (int i = 1; i <= n; i++) {
		iqueue_insert(&q, i);
	}
	while (!iqueue_empty(&q)) {
		int x = iqueue_pop(&q);
		sum += x;
	}
	return sum;
}

int main()
{
	const int repeat = 1000, tot_elements = 10000;
	clock_t t1, t2;
        double dt_sec;
	t1 = clock();
	for (int i = 0; i < repeat; i++)
		test_pure_lqueue(tot_elements);
	t2 = clock();
	dt_sec = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
	printf("Pure lqueue took: %lf ms (%.0lf operations/sec)\n", 1000 * dt_sec,
	       (repeat * tot_elements * 2) / dt_sec);
	t1 = clock();
	for (int i = 0; i < repeat; i++)
		test_pure_iqueue(tot_elements);
	t2 = clock();
	dt_sec = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
	printf("iqueue took: %lf ms (%.0lf operations/sec)\n", 1000 * dt_sec,
	       (repeat * tot_elements * 2) / dt_sec);
	return 0;
}
