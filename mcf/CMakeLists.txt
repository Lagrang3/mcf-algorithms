add_library(mcf STATIC
        mcf/algorithm.h
        mcf/algorithm.c
        mcf/graph.h
        mcf/graph.c
        mcf/priorityqueue.h
        mcf/priorityqueue.c
)
target_include_directories(mcf PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(mcf PUBLIC ccan)
target_link_libraries(mcf PUBLIC gheap)
target_link_libraries(mcf PUBLIC m)
