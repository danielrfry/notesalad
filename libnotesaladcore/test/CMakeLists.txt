cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    libnotesaladcore_test
    MIDI/OPL/OPLParamInfo.cpp
    MIDI/OPM/OPMParamInfo.cpp
    MIDI/SD1/SD1ParamInfo.cpp
    MIDI/MIDIDriver.cpp
    MIDI/MIDIParser.cpp
    MIDI/PatchSerialization.cpp
    MIDI/TestTone.cpp
    OPL/OPLReadWriteDeviceBase.cpp
    SD1/SD1Utils.cpp
    7BitEncoding.cpp
)
set_target_properties(libnotesaladcore_test PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
target_link_libraries(
    libnotesaladcore_test
    PUBLIC
    gmock_main
    notesaladcore
)

include(GoogleTest)
gtest_discover_tests(libnotesaladcore_test)
