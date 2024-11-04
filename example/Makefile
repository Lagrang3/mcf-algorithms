EXAMPLE_SRC := $(wildcard example/ex-*.c)
EXAMPLE_OBJS := $(EXAMPLE_SRC:.c=.o)
EXAMPLE_PROGRAMS := $(EXAMPLE_OBJS:.o=)

ALL_C_SOURCES += $(EXAMPLE_SRC)
ALL_PROGRAMS += $(EXAMPLE_PROGRAMS)

$(EXAMPLE_OBJS): $(EXAMPLE_SRC)

example/ex-pqueue: example/ex-pqueue.o

example/ex-graph: example/ex-graph.o

example/ex-bfs: example/ex-bfs.o

example/ex-dijkstra: example/ex-dijkstra.o

example/ex-flow: example/ex-flow.o

example/ex-mcf: example/ex-mcf.o

example/ex-mcf-validate: example/ex-mcf-validate.o

example/ex-fcmcf-validate: example/ex-fcmcf-validate.o

example/ex-fcmcf-approx-validate: example/ex-fcmcf-approx-validate.o
