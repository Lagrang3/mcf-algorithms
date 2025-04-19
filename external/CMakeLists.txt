add_executable(ccan-configurator ccan/tools/configurator/configurator.c)

add_custom_target(generate_ccan_config ALL 
        DEPENDS ccan-configurator
        COMMAND $<TARGET_FILE:ccan-configurator> > config.h
)

add_library(ccan STATIC
        ccan/ccan/tal/tal.c
        ccan/ccan/list/list.c
        ccan/ccan/take/take.c
        ccan/ccan/bitmap/bitmap.c
        config.h)
target_include_directories(ccan PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ccan>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

add_library(gheap INTERFACE
        gheap/gheap.h)
target_include_directories(gheap INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gheap>)
