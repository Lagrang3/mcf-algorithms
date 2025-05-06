#include <stdio.h>
#include <time.h>

#include "mcf/stack.h"

STACK_DEFINE_TYPE(int, istack);

int test_istack(int n)
{
	int sum = 0;
	struct istack s;
	istack_init(&s, NULL);
	for (int i = 1; i <= n; i++) {
		istack_insert(&s, i);
	}
	while (!istack_empty(&s)) {
		int x = istack_pop(&s);
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
		test_istack(tot_elements);
	t2 = clock();
	dt_sec = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
	printf("istack took: %lf ms (%.0lf operations/sec)\n", 1000 * dt_sec,
	       (repeat * tot_elements * 2) / dt_sec);
	return 0;
}
