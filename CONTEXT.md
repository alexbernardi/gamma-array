# Gamma Array - Quick Development Context

## Project Overview
- **Name**: Gamma Array
- **Type**: C++ OpenGL VJing Application
- **Target**: DDJ-REV1 turntable integration and video scratching
- **Environment**: Windows, VS Code, Visual Studio 2022 build tools

## Quick Commands

### Build & Run (Use these in VS Code terminal)
```powershell
# Full rebuild
cmake -B build -S . -G "Visual Studio 17 2022" -A x64; cmake --build build --config Debug

# Quick build (after initial configure)
cmake --build build --config Debug

# Run application
./build/bin/Debug/GammaArray_d.exe

# Clean build
cmake --build build --target clean; cmake -B build -S . -G "Visual Studio 17 2022" -A x64; cmake --build build --config Debug
```

### VS Code Tasks (Ctrl+Shift+P > Tasks: Run Task)
- **CMake Configure** - Initial project setup
- **CMake Build Debug** - Build debug version
- **CMake Build Release** - Build release version  
- **Run Gamma Array (Debug)** - Build and run debug
- **Setup Dependencies** - Download and configure libraries

### Development Shortcuts
```powershell
# Setup dependencies (run once)
.\scripts\setup_dependencies.ps1

# Quick test build
cmake --build build --config Debug --target gamma_array

# Check for errors
cmake --build build --config Debug 2>&1 | Select-String "error"
```

## Current Development Status

### ✅ Completed
- Project structure and build system
- Documentation and development guides
- VS Code configuration
- CMake setup for all required libraries

### 🔄 Next Priorities
1. **Dependency Installation** - Download and configure libraries
2. **Basic Application Skeleton** - Core application class and main loop
3. **DDJ-REV1 MIDI Mapping** - Controller input detection and mapping
4. **Basic UI Layout** - Adobe Premiere-style panel system

### 📁 Key Directories
- `src/` - All source code
- `include/` - Header files
- `libs/` - Third-party libraries (GLFW, ImGui, RtMidi, PortAudio)
- `build/` - Build output
- `.vscode/` - VS Code configuration

## Quick File References

### Core Files to Edit
- `src/main.cpp` - Application entry point
- `CMakeLists.txt` - Build configuration
- `src/core/` - Core application classes (when created)
- `src/midi/` - MIDI/controller handling (when created)

### Documentation
- `BUILD.md` - Build instructions
- `DEPENDENCIES.md` - Library setup
- `DEVELOPMENT.md` - Coding standards

## Common Issues & Solutions

### Build Fails
```powershell
# Clear and rebuild
Remove-Item -Recurse -Force build; cmake -B build -S . -G "Visual Studio 17 2022" -A x64; cmake --build build --config Debug
```

### Missing Dependencies
```powershell
# Re-run dependency setup
.\scripts\setup_dependencies.ps1 -Clean
```

### VS Code IntelliSense Issues
- Restart VS Code
- Run "C/C++: Reset IntelliSense Database"
- Check `.vscode/c_cpp_properties.json`

## Development Workflow

### Starting Development Session
1. Open VS Code in project directory
2. Run **CMake Configure** task if needed
3. Run **CMake Build Debug** task
4. Start coding with live error checking

### Adding New Features
1. Create header in `include/`
2. Create implementation in `src/`
3. Update `CMakeLists.txt` if needed
4. Build and test

### Testing Changes
1. Build: `cmake --build build --config Debug`
2. Run: `./build/bin/Debug/GammaArray_d.exe`
3. Debug: Use F5 in VS Code

## Library Integration Status

### Required Libraries
- **GLFW** - Window management ⏳ (needs installation)
- **ImGui** - UI framework ⏳ (needs installation)
- **RtMidi** - MIDI input ⏳ (needs installation)
- **PortAudio** - Audio engine ⏳ (needs installation)
- **OpenGL** - Graphics ✅ (system drivers)

### Integration Notes
- All libraries configured in CMakeLists.txt
- Headers will be in `libs/*/include/`
- Built libraries in `libs/*/build/`

## Performance Targets
- 60 FPS video processing
- <10ms audio latency  
- <5ms MIDI input latency
- Real-time video scratching capability