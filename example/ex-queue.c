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
	clock_t t1, t2;
	t1 = clock();
	for (int i = 0; i < 1000; i++)
		test_pure_lqueue(10000);
	t2 = clock();
	printf("Pure lqueue took: %lf ms\n",
	       1000 * (double)(t2 - t1) / CLOCKS_PER_SEC);
	t1 = clock();
	for (int i = 0; i < 1000; i++)
		test_pure_iqueue(10000);
	t2 = clock();
	printf("iqueue took: %lf ms\n",
	       1000 * (double)(t2 - t1) / CLOCKS_PER_SEC);
	return 0;
}
