# Gamma Array Project Examples

This directory contains example code and tutorials for Gamma Array development.

## Getting Started

- `hello_world/` - Basic application setup and window creation
- `opengl_test/` - OpenGL context verification and basic rendering
- `midi_input/` - MIDI device detection and input handling
- `audio_test/` - Audio device enumeration and basic I/O
- `ui_layout/` - ImGui interface examples and panel layouts

## DDJ-REV1 Integration Examples

- `ddj_mapping/` - Controller mapping configuration
- `scratch_detection/` - Turntable input processing
- `video_scratch/` - Basic video scratching implementation

## Build Instructions

Each example can be built independently:

```bash
cd examples/hello_world
cmake -B build -S .
cmake --build build
```

Or build all examples from the main project:

```bash
cmake -B build -S . -DBUILD_EXAMPLES=ON
cmake --build build
```