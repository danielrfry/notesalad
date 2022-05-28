add_library(notesaladcore INTERFACE)
target_include_directories(notesaladcore
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(notesaladcore
    INTERFACE
        notesaladcore.h
        7BitEncoding.cpp
        7BitEncoding.h
        BufferIO.cpp
        BufferIO.h
        Utils.cpp
        Utils.h
)

add_subdirectory(MIDI)
add_subdirectory(OPL)
add_subdirectory(OPM)
add_subdirectory(SD1)
