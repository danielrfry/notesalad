# Note Salad

An embeddable, device independent MIDI implementation in C++ with drivers for the Yamaha OPL (YM3812/YMF262), OPM (YM2151) and SD-1 (YMF825) FM synthesizer chips.

_If you're looking for a quick and simple way to try out the MIDI implementation for OPL or OPM, check out [Note Salad Editor](https://github.com/danielrfry/notesalad-editor/), which is a browser-based patch editor and software synth that emulates those chips._

Note Salad parses a MIDI data stream and drives a hardware or software synthesizer by calling abstract methods to start/stop sounds and set synth parameters on individual monophonic channels. It handles channel allocation and basic patch management, and provides software LFOs and mapping of MIDI controls to synth parameters.

**This code should be considered experimental**. It's a hobby project that I hadn't originally intended to release, with fun taking priority over code quality or documentation. There will be plenty of rough edges.

## Components

### In this repository

- **libnotesaladcore**: the core MIDI implementation. Can be built as an RP2040 or Teensyduino library for embedding, or linked into libnotesalad for use on desktop/web platforms.
- **libnotesalad**: library incorporating **libnotesaladcore** and OPL and OPM emulators (from [ymfm](https://github.com/aaronsgiles/ymfm/)). Targets Linux, macOS and web (via WASM). Allows use of the MIDI implementation without dedicated hardware.

### In other repositories

- **[Note Salad Editor](https://github.com/danielrfry/notesalad-editor/)**: GUI patch editor and software synthesizer web app for OPL, OPM and SD-1. Browser based, requiring no additional hardware or software installation.
- **[Note Salad Tools](https://github.com/danielrfry/notesalad-tools/)**: a collection of tools written in Python for converting, 'disassembling' and editing VGM/DRO files, converting MIDI to VGM, playing or rendering VGM/MID files to WAV etc. using **libnotesalad**.

## Building libnotesalad for Linux/macOS

### Requirements

- [CMake](https://cmake.org/) >= 3.14

### Building

Clone the repository and its submodules:

```
git clone --recurse-submodules https://github.com/danielrfry/notesalad.git
```

Generate makefiles using CMake and build:

```
cd notesalad
cmake -DCMAKE_BUILD_TYPE=Release -B build
cd build
make
```

Install:

```
sudo make install
```

### Running tests

There aren't many tests, but those that do exist can be built and run as follows:

```
cd build
make libnotesaladcore_test
ctest
```

## Building libnotesalad for web

### Requirements

- [CMake](https://cmake.org/) >= 3.14
- [Emscripten SDK](https://emscripten.org/)
- [node.js](https://nodejs.org/) - latest LTS version

### Building

Install npm packages:

```
cd web
npm install
```

Build:

```
npm run build
```

## Embedding libnotesaladcore

### In RP2040 projects

libnotesaladcore attempts to follow the Raspberry Pi Pico SDK convention of packaging code as CMake interface libraries. It can be added to an RP2040 project by directly including the libnotesaladcore subdirectory in your project's `CMakeLists.txt`, for example:

```
add_subdirectory(../notesalad/libnotesaladcore/src notesaladcore)
```

Then, add libnotesaladcore to your project's link step, for example:

```
target_link_libraries(your-project pico_stdlib notesaladcore)
```

### In Teensyduino projects

A `library.properties` file is included allowing libnotesaladcore to be used as a Teensyduino library. Arduino targets other than the ARM-based Teensy series may also work but have not been tested.

I find the most convenient way to use the library with Teensyduino is to create a symbolic link to the directory containing `library.properties` in Teensyduino's `libraries` directory. For example:

```
ln -s ~/src/notesalad/libnotesaladcore ~/Documents/Arduino/libraries/
```

Replace `~/src/notesalad` with the directory containing your working copy of the Note Salad repository, and `~/Documents/Arduino/libraries` with the location of Teensyduino's `libraries` directory.
