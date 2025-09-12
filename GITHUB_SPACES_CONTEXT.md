# Gamma Array - AI Assistant Context

## Project Summary
Gamma Array is a high-performance VJing application built with C++ and OpenGL, focusing on turntable-style video scratching with DDJ-REV1 controller integration.

## Development Guidelines for AI Assistant

### Working Approach
- Take development one step at a time
- Always verify current file contents before making changes
- Ask for confirmation before making assumptions about system configuration
- Focus on modularity and scalability in all code design
- Avoid making large changes without incremental testing
- Keep code examples minimal and focused on the immediate task

### Current Development Status
- Complete: Project structure, build system, documentation, GitHub repository
- In Progress: Dependency setup and basic application skeleton
- Next: DDJ-REV1 MIDI integration and core UI layout

### Technical Stack
- Platform: Windows 10/11, VS Code, Visual Studio 2022 build tools
- Language: C++17 with modern CMake 3.20+
- Graphics: OpenGL 3.3+ for real-time rendering
- Libraries: GLFW (windowing), ImGui (UI), RtMidi (MIDI), PortAudio (audio)
- Architecture: Modular design with clear separation between core, rendering, audio, MIDI, and UI modules

### Build Commands
```powershell
.\dev.ps1 setup     # Install dependencies and configure build
.\dev.ps1 build     # Build debug configuration
.\dev.ps1 run       # Build and run application
.\dev.ps1 deps      # Check dependency installation status
.\dev.ps1 rebuild   # Clean rebuild when issues occur
```

### Key Project Files
- CMakeLists.txt - Build system configuration
- src/main.cpp - Application entry point (currently placeholder)
- dev.ps1 - Development automation script
- CONTEXT.md - Detailed development reference
- BUILD.md - Build instructions
- DEPENDENCIES.md - Library installation guide

### Current File Structure
```
gamma_array/
├── src/             # Source code (main.cpp only currently)
├── include/         # Header files (empty, to be created)
├── libs/            # Third-party libraries (to be populated)
├── build/           # Build output directory
├── .vscode/         # VS Code configuration
├── scripts/         # Setup automation scripts
└── docs/            # Documentation
```

### Performance Requirements
- 60 FPS video processing minimum
- Audio latency under 10ms
- MIDI input latency under 5ms
- Real-time video scratching capability

### Immediate Development Tasks
1. Run dependency setup script to install GLFW, ImGui, RtMidi, PortAudio
2. Create basic Application class structure in src/core/
3. Implement OpenGL context initialization
4. Add MIDI device detection for DDJ-REV1
5. Build basic UI layout system

### Important Notes
- Always verify build works after each change
- Use incremental development approach
- Test on actual hardware when MIDI features are added
- Maintain clean modular architecture
- Focus on performance from the beginning
- Do not use emojiis
- After every update lets make a commit and push so we can revert if necessary

This context enables efficient development assistance while maintaining project quality and avoiding over-engineering.