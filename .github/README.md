# Gamma Array

[![License](https://img.shields.io/badge/license-MIT-blue.svg)]((https://github.com/alexbernardi/gamma-array?tab=MIT-1-ov-file#readme))
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://github.com/alexbernardi/gamma-array)
[![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)](https://github.com/alexbernardi/gamma-array)

A high-performance VJing application built with C++ and OpenGL, focusing on turntable-style video scratching with DDJ-REV1 controller integration.

> **Current Status**: MIDI controller integration complete with velocity-based jog wheel control and comprehensive logging capabilities

## Overview

Gamma Array is a modular VJing application that provides professional video mixing capabilities with a focus on turntable-style video scratching. Built from the ground up with performance in mind, it leverages C++ and OpenGL to deliver smooth real-time video processing.

## Current Features

### MIDI Integration
- **DDJ-REV1 Controller Support**: Full integration with Pioneer DDJ-REV1 controller
- **Velocity-Responsive Jog Wheels**: Accurate rotation speed mapping based on physical wheel velocity
  - Clockwise: 0x41+ (0x41 = slowest, higher values = faster)
  - Counter-clockwise: 0x3F- (0x3F = slowest, lower values = faster)
- **Comprehensive MIDI Logging**: Real-time message capture with hex formatting
- **CSV Export**: Timestamped CSV export with parsed MIDI data for analysis
- **Real-time Performance**: Sub-5ms MIDI input latency for responsive control

### User Interface
- **Adobe-Style Workspace**: Four-panel layout with central tabbed area
- **MIDI Control Panel**: Real-time MIDI device management and monitoring
- **Modular UI Components**: Resizable panels for Import, Timeline, Effects, and Output

## Technical Stack

- **Platform**: Windows 10/11, VS Code, Visual Studio 2022 build tools
- **Language**: C++17 with modern CMake 3.20+
- **Graphics**: OpenGL 3.3+ for real-time rendering
- **Libraries**: GLFW (windowing), ImGui (UI), RtMidi (MIDI), PortAudio (audio)
- **Architecture**: Modular design with clear separation between core, rendering, audio, MIDI, and UI modules

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

### Development Commands
```powershell
.\dev.ps1 setup     # Install dependencies and configure build
.\dev.ps1 build     # Build release configuration (due to RtMidi compatibility)
.\dev.ps1 run       # Build and run application with MIDI support
.\dev.ps1 deps      # Check dependency installation status
.\dev.ps1 rebuild   # Clean rebuild when issues occur
```

## Project Structure

```
gamma_array/
├── src/             # Source code with modular architecture
│   ├── core/        # Core application and system management
│   ├── midi/        # MIDI controller integration (DDJ-REV1)
│   ├── ui/          # ImGui user interface panels
│   └── main.cpp     # Application entry point
├── include/         # Header files with matching structure
├── libs/            # Third-party libraries (fully populated)
│   ├── glfw/        # Window management and OpenGL context
│   ├── imgui/       # Immediate mode GUI library
│   ├── rtmidi/      # Real-time MIDI I/O
│   └── portaudio/   # Cross-platform audio I/O
├── build/           # Build output directory
├── docs/            # Documentation
├── examples/        # Code examples and demos
└── dev.ps1          # Development automation script
```

## Development Status

### Complete
- Project structure, build system, documentation
- GitHub repository with CI/CD workflows
- Dependency setup and configuration
- Core application architecture
- MIDI system integration with velocity-based jog wheel control
- CSV export functionality
- UI framework with tabbed interface

### Current
- DDJ-REV1 MIDI controller fully integrated with accurate velocity-responsive rotation
- Comprehensive MIDI logging and CSV export
- Adobe-style workspace with four-panel layout
- Working build system

### Next
- Video processing pipeline
- Audio integration
- Effects system
- MIDI mapping for VJ controls

## Performance Requirements

- 60 FPS video processing minimum
- Audio latency under 10ms
- MIDI input latency under 5ms
- Real-time video scratching capability

## Documentation

- [CONTEXT.md](CONTEXT.md) - Complete development context and reference
- [BUILD.md](BUILD.md) - Detailed build instructions and setup
- [DEPENDENCIES.md](DEPENDENCIES.md) - Required libraries and installation
- [QUICKSTART.md](QUICKSTART.md) - Quick start guide for new developers
- [DEVELOPMENT.md](DEVELOPMENT.md) - Development workflow guide
- [ddj_rev1_mapping.md](ddj_rev1_mapping.md) - DDJ-REV1 controller mapping reference

## Contributing

We welcome contributions! Please follow our development guidelines:

1. Take development one step at a time
2. Always verify current file contents before making changes
3. Focus on modularity and scalability in all code design
4. Avoid making large changes without incremental testing
5. After every update, make a commit and push for easy reversion

### Contributing Process
1. Fork the repository
2. Create a feature branch
3. Follow the coding standards
4. Test your changes with `.\dev.ps1 build`
5. Submit a pull request

## License

MIT License - see [LICENSE](LICENSE) file for details

## Support

- **Issues**: [GitHub Issues](https://github.com/alexbernardi/gamma-array/issues)
- **Discussions**: [GitHub Discussions](https://github.com/alexbernardi/gamma-array/discussions)

## Acknowledgments

Built with these excellent libraries:
- [GLFW](https://www.glfw.org/) - OpenGL window management
- [ImGui](https://github.com/ocornut/imgui) - Immediate mode GUI
- [RtMidi](https://github.com/thestk/rtmidi) - MIDI I/O
- [PortAudio](http://portaudio.com/) - Audio I/O