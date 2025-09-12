# Gamma Array - GitHub Spaces Context

## Project Summary
**Gamma Array** is a high-performance VJing application built with C++ and OpenGL, focusing on turntable-style video scratching with DDJ-REV1 controller integration.

## Quick Context for AI Assistant

### Current Development Status
- **Complete**: Project structure, build system, documentation
- **In Progress**: Dependency setup and basic application skeleton  
- **Next**: DDJ-REV1 MIDI integration and core UI layout

### Key Technical Details
- **Platform**: Windows, VS Code + Visual Studio 2022
- **Language**: C++17 with modern CMake
- **Graphics**: OpenGL 3.3+ with real-time rendering
- **Libraries**: GLFW, ImGui, RtMidi, PortAudio
- **Architecture**: Modular design for scalability

### Fast Development Commands
```powershell
.\dev.ps1 setup     # One-time setup
.\dev.ps1 build     # Quick build  
.\dev.ps1 run       # Build and run
.\dev.ps1 deps      # Check status
```

### Key Files
- `CONTEXT.md` - Complete development reference
- `dev.ps1` - Development automation script
- `src/main.cpp` - Application entry point
- `CMakeLists.txt` - Build configuration

### Performance Targets
- 60 FPS video processing
- <10ms audio latency
- <5ms MIDI input latency
- Real-time video scratching

### Development Priorities
1. Complete dependency installation
2. Create core Application class
3. Implement DDJ-REV1 MIDI mapping
4. Build Adobe Premiere-style UI layout
5. Add basic video scratching effects

This context allows rapid development assistance without re-explaining the project setup each time.