add_executable(ex-pqueue ex-pqueue.c)
target_link_libraries(ex-pqueue mcf)

add_executable(ex-graph ex-graph.c)
target_link_libraries(ex-graph mcf)

add_executable(ex-bfs ex-bfs.c)
target_link_libraries(ex-bfs mcf)

add_executable(ex-dijkstra ex-dijkstra.c)
target_link_libraries(ex-dijkstra mcf)

add_executable(ex-flow ex-flow.c)
target_link_libraries(ex-flow mcf)

add_executable(ex-mcf ex-mcf.c)
target_link_libraries(ex-mcf mcf)

add_executable(ex-mcf-validate ex-mcf-validate.c)
target_link_libraries(ex-mcf-validate mcf)

add_executable(ex-fcmcf-approx-validate ex-fcmcf-approx-validate.c)
target_link_libraries(ex-fcmcf-approx-validate mcf)

add_executable(ex-constrained-fcmcf-validate ex-constrained-fcmcf-validate.c)
target_link_libraries(ex-constrained-fcmcf-validate mcf)

add_executable(ex-queue ex-queue.c)
target_link_libraries(ex-queue mcf)

add_executable(ex-stack ex-stack.c)
target_link_libraries(ex-stack mcf)

add_executable(ex-goldberg-tarjan-validate ex-goldberg-tarjan-validate.c)
target_link_libraries(ex-goldberg-tarjan-validate mcf)
