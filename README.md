# Gamma Array

[![Build Status](https://github.com/alexbernardi/gamma-array/workflows/Build%20an### Development Status
- Complete: Core project structure and build system
- Complete: All dependencies configured (GLFW, ImGui, RtMidi, PortAudio)
- Current: Basic application skeleton
- Next: DDJ-REV1 MIDI controller integration
- Planned: UI layout system (Adobe Premiere-style)
- Planned: Video processing and scratching enginet/badge.svg)](https://github.com/ale- Issues: [GitHub Issues](https://github.com/alexbernardi/gamma-array/issues)
- Discussions: [GitHub Discussions](https://github.com/alexbernardi/gamma-array/discussions)ernardi/gamma-array/actions)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://github.com/alexbernardi/gamma-array)

A high-performance VJing application built with C++ and OpenGL, designed for real-time video manipulation and DJ controller integration.

> **Development Status**: Early development phase - Core foundation and DDJ-REV1 controller integration in progress

## Overview

Gamma Array is a modular VJing application that provides professional video mixing capabilities with a focus on turntable-style video scratching. Built from the ground up with performance in mind, it leverages C++ and OpenGL to deliver smooth real-time video processing.

## Key Features

- **Real-time Video Processing**: Hardware-accelerated video effects and transitions using OpenGL
- **DJ Controller Integration**: Native support for MIDI controllers, starting with DDJ-REV1 turntable mapping
- **Modular Architecture**: Extensible plugin system for custom effects and controllers
- **Professional UI**: Adobe Premiere-inspired interface with resizable panels and professional workflow
- **Cross-Platform**: Built with portable libraries for future platform expansion

## Architecture

Gamma Array follows a modular design pattern with clear separation of concerns:

- **Core Engine**: Video processing, OpenGL rendering, and resource management
- **Audio Engine**: PortAudio integration for audio-visual synchronization
- **MIDI System**: RtMidi-based controller input and mapping
- **UI Framework**: ImGui-based interface with dockable panels
- **Plugin System**: Extensible architecture for custom effects and features

## Quick Start

### Prerequisites
- Windows 10/11
- Visual Studio 2022 (with C++ Desktop Development workload)
- VS Code (recommended for development)
- Git

### Setup and Build
```powershell
# Clone the repository
git clone https://github.com/alexbernardi/gamma-array.git
cd gamma-array

# One-command setup (installs dependencies and configures build)
.\dev.ps1 setup

# Build and run
.\dev.ps1 run
```

### Alternative Manual Setup
```powershell
# Install dependencies
.\scripts\setup_dependencies.ps1

# Configure and build
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug

# Run
.\build\bin\Debug\GammaArray_d.exe
```

For detailed instructions, see [BUILD.md](BUILD.md).

## Project Structure

```
gamma_array/
├── src/                 # Source code
├── include/             # Header files
├── libs/                # Third-party libraries
├── docs/                # Documentation
├── examples/            # Example code and tutorials
├── build/               # Build output
└── .vscode/             # VS Code configuration
```

## Development

### Quick Development Commands
```powershell
.\dev.ps1 build     # Build the project
.\dev.ps1 run       # Build and run
.\dev.ps1 deps      # Check dependency status
.\dev.ps1 rebuild   # Clean rebuild
```

### VS Code Integration
- Open project in VS Code
- Use `Ctrl+Shift+P` → `Tasks: Run Task` for build tasks
- Press `F5` to debug
- Full IntelliSense and error checking configured

### Development Status
- Complete: Core project structure and build system
- Complete: All dependencies configured (GLFW, ImGui, RtMidi, PortAudio)
- Current: Basic application skeleton
- Next: DDJ-REV1 MIDI controller integration
- Planned: UI layout system (Adobe Premiere-style)
- Planned: Video processing and scratching engine
- Video processing and scratching engine

## Documentation

- [QUICKSTART.md](QUICKSTART.md) - Fast setup and daily commands
- [CONTEXT.md](CONTEXT.md) - Complete development context and reference
- [BUILD.md](BUILD.md) - Detailed build instructions and setup
- [DEPENDENCIES.md](DEPENDENCIES.md) - Required libraries and installation
- [DEVELOPMENT.md](DEVELOPMENT.md) - Development guidelines and standards
- [docs/](docs/) - Detailed documentation and API reference

## Development Environment

This project is optimized for development in VS Code with Visual Studio 2022 build tools. The build system uses CMake for cross-platform compatibility.

## Roadmap

- **Phase 1**: Core foundation and DDJ-REV1 integration
- **Phase 2**: Basic video effects and UI panels
- **Phase 3**: Advanced video scratching features
- **Phase 4**: Plugin system and effect marketplace

## License

MIT License - see [LICENSE](LICENSE) file for details

## Contributing

We welcome contributions! Please see [DEVELOPMENT.md](DEVELOPMENT.md) for contribution guidelines and coding standards.

1. Fork the repository
2. Create a feature branch
3. Follow the coding standards in DEVELOPMENT.md
4. Test your changes with `.\dev.ps1 build`
5. Submit a pull request

## Support

- **Issues**: [GitHub Issues](https://github.com/your-username/gamma-array/issues)
- **Discussions**: [GitHub Discussions](https://github.com/your-username/gamma-array/discussions)
- **Documentation**: See [docs/](docs/) directory

## Acknowledgments

Built with these excellent libraries:
- [GLFW](https://www.glfw.org/) - OpenGL window management
- [ImGui](https://github.com/ocornut/imgui) - Immediate mode GUI
- [RtMidi](https://github.com/thestk/rtmidi) - MIDI I/O
- [PortAudio](http://portaudio.com/) - Audio I/O