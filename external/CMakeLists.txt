add_library(ccan STATIC
        ccan/ccan/tal/tal.c
        ccan/ccan/list/list.c
        ccan/ccan/take/take.c
        ccan/ccan/bitmap/bitmap.c)
target_include_directories(ccan PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ccan>
)
target_link_libraries(ccan PUBLIC config)

add_library(gheap INTERFACE
        gheap/gheap.h)
target_include_directories(gheap INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gheap>)
