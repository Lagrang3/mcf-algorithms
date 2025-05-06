add_library(config INTERFACE config.h)
target_include_directories(config INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
