add_compile_definitions(
    OUTSIDE_SPEEX
    RANDOM_PREFIX=ntsld
    FLOATING_POINT
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WASM)
    message(STATUS "Setting compilation target to WASM")
    add_executable(notesalad)
    set_target_properties(notesalad PROPERTIES OUTPUT_NAME libnotesalad)
    set_target_properties(notesalad
        PROPERTIES LINK_FLAGS "\
            -O1 \
            -s WASM=1 \
            -s BINARYEN_ASYNC_COMPILATION=0 \
            -s SINGLE_FILE=1 \
            -s ENVIRONMENT='web' \
            -s MODULARIZE=1 \
            -s EXPORT_ES6=1 \
            -s EXPORTED_FUNCTIONS='[ \
                _ntsld_midi_set_recv_cbk, \
                _ntsld_midi_send, \
                _ntsld_midi_set_time, \
                _ntsld_midi_update, \
                _ntsld_midi_reset, \
                _ntsld_midi_delete, \
                _ntsld_opl_emu_getsamples, \
                _ntsld_opl_emu_new, \
                _ntsld_opl_cbkdev_new, \
                _ntsld_opl_delete, \
                _ntsld_opl_reset, \
                _ntsld_opl_write, \
                _ntsld_opl2midi_new, \
                _ntsld_opl3midi_new, \
                _ntsld_oplsd1_new, \
                _ntsld_opm_emu_getsamples, \
                _ntsld_opm_emu_new, \
                _ntsld_opm_cbkdev_new, \
                _ntsld_opm_delete, \
                _ntsld_opm_reset, \
                _ntsld_opm_write, \
                _ntsld_opmmidi_new, \
                _ntsld_sd1_cbkdev_new, \
                _ntsld_sd1_delete, \
                _ntsld_sd1midi_new, \
                _calloc, \
                _free \
            ]' \
            -s EXPORTED_RUNTIME_METHODS='[ \
                getValue, \
                setValue, \
                addFunction, \
                removeFunction \
            ]' \
            -s RESERVED_FUNCTION_POINTERS=20"
    )
else()
    add_library(notesalad SHARED)
endif()

target_sources(notesalad
    PRIVATE
        CallbackOPLDevice.cpp
        CallbackOPLDevice.h
        CallbackOPMDevice.cpp
        CallbackOPMDevice.h
        CallbackSD1Device.cpp
        CallbackSD1Device.h
        EmulatorBase.cpp
        EmulatorBase.h
        IMIDIManager.cpp
        IMIDIManager.h
        OPL2MIDIManager.cpp
        OPL2MIDIManager.h
        OPL3MIDIManager.cpp
        OPL3MIDIManager.h
        OPLEmulator.cpp
        OPLEmulator.h
        OPMEmulator.cpp
        OPMEmulator.h
        OPMMIDIManager.cpp
        OPMMIDIManager.h
        SD1MIDIManager.cpp
        SD1MIDIManager.h
        midi.cpp
        midi.h
        opl.cpp
        opl.h
        oplsd1.cpp
        oplsd1.h
        opm.cpp
        opm.h
        sd1.cpp
        sd1.h
)

add_subdirectory(resampler)

add_library(ymfm STATIC
    ymfm/src/ymfm_opl.cpp
    ymfm/src/ymfm_pcm.cpp
    ymfm/src/ymfm_adpcm.cpp
    ymfm/src/ymfm_opm.cpp
)
target_compile_options(ymfm PRIVATE -Wno-deprecated-declarations)

target_link_libraries(notesalad PUBLIC notesaladcore ymfm)
target_include_directories(notesalad
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
