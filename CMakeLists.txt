cmake_minimum_required(VERSION 3.25)
project(mcf C)
add_compile_options(-Wall -Werror -Wextra -O2)

add_subdirectory(external)
add_subdirectory(mcf)
add_subdirectory(example)
