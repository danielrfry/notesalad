cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)

project(libnotesalad)
option(WASM "Compile to WASM" OFF)

add_compile_options(-Wall -Werror)

enable_testing()

add_subdirectory(libnotesaladcore)
add_subdirectory(libnotesalad)

install(
    TARGETS notesalad
    RUNTIME
        DESTINATION lib
    PUBLIC_HEADER
        DESTINATION include/notesalad
)
