# Gamma Array - AI Assistant Context
NO EMOJIIS PLEASE
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
- Complete: Project structure, build system, documentation, GitHub repository, dependency setup, core application architecture, MIDI system integration, UI framework with tabbed interface
- Current: DDJ-REV1 MIDI controller fully integrated with real-time signal logging, Adobe-style workspace with four-panel layout, working build system
- Next: Video processing pipeline, audio integration, effects system, MIDI mapping for VJ controls

### Technical Stack
- Platform: Windows 10/11, VS Code, Visual Studio 2022 build tools
- Language: C++17 with modern CMake 3.20+
- Graphics: OpenGL 3.3+ for real-time rendering
- Libraries: GLFW (windowing), ImGui (UI), RtMidi (MIDI), PortAudio (audio)
- Architecture: Modular design with clear separation between core, rendering, audio, MIDI, and UI modules

### Build Commands
```powershell
.\dev.ps1 setup     # Install dependencies and configure build
.\dev.ps1 build     # Build release configuration (due to RtMidi compatibility)
.\dev.ps1 run       # Build and run application with MIDI support
.\dev.ps1 deps      # Check dependency installation status
.\dev.ps1 rebuild   # Clean rebuild when issues occur
```

### Key Project Files
- CMakeLists.txt - Build system configuration
- src/main.cpp - Application entry point
- src/core/Application.{h,cpp} - Main application class with subsystem management
- src/midi/MidiManager.{h,cpp} - DDJ-REV1 MIDI controller integration
- src/ui/ - ImGui workspace panels (Output, Timeline, Import, Effects)
- include/ - Matching header file structure
- dev.ps1 - Development automation script with build commands
- CONTEXT.md - Detailed development reference
- BUILD.md - Build instructions
- DEPENDENCIES.md - Library installation guide
- QUICKSTART.md - Quick start guide for new developers

### Current File Structure
```
gamma_array/
├── src/             # Source code with modular architecture
│   ├── core/        # Core application and system management
│   ├── midi/        # MIDI controller integration (DDJ-REV1)
│   ├── ui/          # ImGui user interface panels
│   └── main.cpp     # Application entry point
├── include/         # Header files with matching structure
│   ├── core/        # Core system headers
│   ├── midi/        # MIDI management headers
│   └── ui/          # UI component headers
├── libs/            # Third-party libraries (fully populated)
│   ├── glfw/        # Window management and OpenGL context
│   ├── imgui/       # Immediate mode GUI library
│   ├── rtmidi/      # Real-time MIDI I/O
│   └── portaudio/   # Cross-platform audio I/O
├── build/           # Build output directory
├── .vscode/         # VS Code configuration
├── .github/         # GitHub workflows and templates
├── scripts/         # Setup automation scripts
├── docs/            # Documentation
├── examples/        # Code examples and demos
└── [config files]   # CMakeLists.txt, dev.ps1, README.md, etc.
```

### Performance Requirements
- 60 FPS video processing minimum
- Audio latency under 10ms
- MIDI input latency under 5ms
- Real-time video scratching capability

### Immediate Development Tasks
1. ✅ Run dependency setup script to install GLFW, ImGui, RtMidi, PortAudio
2. ✅ Create basic Application class structure in src/core/
3. ✅ Implement OpenGL context initialization
4. ✅ Add MIDI device detection for DDJ-REV1
5. ✅ Build basic UI layout system
6. 🔄 Implement video processing pipeline for VJ scratching
7. 🔄 Add audio playback and analysis capabilities
8. 🔄 Create effects processing system
9. 🔄 Map DDJ-REV1 controls to VJ functions

### Important Notes
- Always verify build works after each change
- Use incremental development approach
- Test on actual hardware when MIDI features are added
- Maintain clean modular architecture
- Focus on performance from the beginning
- Do not use emojiis
- After every update lets make a commit and push so we can revert if necessary

This context enables efficient development assistance while maintaining project quality and avoiding over-engineering.