# Development Guide

This document outlines the development standards, project structure, and contribution guidelines for Gamma Array.

## Project Architecture

Gamma Array follows a modular architecture designed for scalability and maintainability:

### Core Modules

- **Engine Core** (`src/core/`) - Main application lifecycle and resource management
- **Rendering** (`src/rendering/`) - OpenGL-based video processing and effects
- **Audio** (`src/audio/`) - PortAudio integration and audio-visual synchronization
- **MIDI** (`src/midi/`) - RtMidi-based controller input and mapping system
- **UI** (`src/ui/`) - ImGui-based user interface components
- **Plugins** (`src/plugins/`) - Extensible plugin system for effects and features

### Directory Structure

```
gamma_array/
├── src/                     # Source code
│   ├── core/               # Core engine functionality
│   ├── rendering/          # OpenGL rendering and effects
│   ├── audio/              # Audio processing and synchronization
│   ├── midi/               # MIDI input and controller mapping
│   ├── ui/                 # User interface components
│   ├── plugins/            # Plugin system and base classes
│   └── main.cpp            # Application entry point
├── include/                # Public header files
│   ├── core/
│   ├── rendering/
│   ├── audio/
│   ├── midi/
│   ├── ui/
│   └── plugins/
├── libs/                   # Third-party libraries
│   ├── imgui/              # ImGui library
│   ├── rtmidi/             # RtMidi library
│   ├── portaudio/          # PortAudio library
│   └── glfw/               # GLFW library
├── docs/                   # Documentation
├── examples/               # Example code and tutorials
├── build/                  # Build output
└── .vscode/                # VS Code configuration
```

## Coding Standards

### C++ Style Guidelines

Follow these coding standards for consistency:

#### Naming Conventions

```cpp
// Classes: PascalCase
class VideoProcessor { };
class MidiController { };

// Functions and variables: camelCase
void processVideo();
int frameCount;
bool isPlaying;

// Constants: UPPER_SNAKE_CASE
const int MAX_EFFECTS = 32;
const float DEFAULT_OPACITY = 1.0f;

// Private members: camelCase with underscore prefix
class Example {
private:
    int _privateVariable;
    void _privateMethod();
};

// Namespaces: lowercase
namespace gamma {
namespace rendering {
    // implementation
}
}
```

#### File Organization

```cpp
// Header file structure (.h/.hpp)
#pragma once

#include <system_headers>
#include "local_headers.h"

namespace gamma {

class ExampleClass {
public:
    // Public interface first
    ExampleClass();
    ~ExampleClass();
    
    void publicMethod();
    
private:
    // Private members last
    int _privateMember;
    void _privateMethod();
};

} // namespace gamma
```

#### Code Formatting

- **Indentation**: 4 spaces (no tabs)
- **Braces**: Opening brace on same line for classes/functions
- **Line Length**: Maximum 100 characters
- **Include Order**: System headers, then local headers, alphabetically sorted

```cpp
// Good formatting example
class VideoRenderer {
public:
    VideoRenderer(int width, int height) 
        : _width(width), _height(height) {
        initialize();
    }
    
    void render(const Frame& frame) {
        if (!_initialized) {
            return;
        }
        
        // Implementation
    }
    
private:
    int _width;
    int _height;
    bool _initialized = false;
    
    void initialize();
};
```

### Memory Management

- Use RAII principles for resource management
- Prefer smart pointers over raw pointers
- Use `std::unique_ptr` for single ownership
- Use `std::shared_ptr` when shared ownership is needed

```cpp
// Good memory management
class ResourceManager {
private:
    std::unique_ptr<VideoProcessor> _processor;
    std::shared_ptr<AudioEngine> _audioEngine;
    
public:
    void initialize() {
        _processor = std::make_unique<VideoProcessor>();
        _audioEngine = std::make_shared<AudioEngine>();
    }
};
```

### Error Handling

- Use exceptions for error conditions that should not be ignored
- Use return codes for expected failure cases
- Provide meaningful error messages
- Log errors appropriately

```cpp
// Error handling example
class MidiInput {
public:
    enum class Result {
        Success,
        DeviceNotFound,
        PermissionDenied,
        InvalidConfiguration
    };
    
    Result openDevice(const std::string& deviceName) {
        try {
            // Implementation
            return Result::Success;
        } catch (const std::exception& e) {
            logError("Failed to open MIDI device: " + std::string(e.what()));
            return Result::DeviceNotFound;
        }
    }
};
```

## Development Workflow

### Setting Up Development Environment

1. Clone the repository
2. Install dependencies (see [DEPENDENCIES.md](DEPENDENCIES.md))
3. Configure VS Code with recommended extensions
4. Build the project in Debug mode
5. Run tests to verify setup

### Recommended VS Code Extensions

- **C/C++** (Microsoft) - IntelliSense and debugging
- **CMake Tools** (Microsoft) - CMake integration
- **GitLens** - Enhanced Git integration
- **Bracket Pair Colorizer** - Code readability
- **clang-format** - Code formatting

### Git Workflow

1. **Feature Branches**: Create feature branches from `main`
2. **Commit Messages**: Use conventional commit format
3. **Pull Requests**: All changes must go through PR review
4. **Testing**: Ensure all tests pass before merging

```bash
# Example workflow
git checkout -b feature/midi-controller-mapping
git add .
git commit -m "feat: add DDJ-REV1 controller mapping"
git push origin feature/midi-controller-mapping
# Create pull request
```

### Testing Strategy

- **Unit Tests**: Test individual components in isolation
- **Integration Tests**: Test module interactions
- **Performance Tests**: Ensure real-time performance requirements
- **Manual Testing**: Test with actual hardware controllers

## Module Development Guidelines

### Core Module

The core module manages application lifecycle and coordinates between other modules.

```cpp
// Core module interface
namespace gamma::core {
class Application {
public:
    bool initialize();
    void run();
    void shutdown();
    
private:
    std::unique_ptr<rendering::Renderer> _renderer;
    std::unique_ptr<audio::AudioEngine> _audioEngine;
    std::unique_ptr<midi::MidiManager> _midiManager;
};
}
```

### Rendering Module

Handles all OpenGL operations and video effects processing.

- Use modern OpenGL (3.3+)
- Implement effects as composable shaders
- Maintain 60 FPS minimum performance
- Support real-time video input/output

### Audio Module

Manages audio processing and synchronization with video.

- Use PortAudio for cross-platform audio
- Implement beat detection for sync features
- Support multiple audio input/output devices
- Maintain low-latency audio processing

### MIDI Module

Handles controller input and mapping.

- Support hot-plugging of MIDI devices
- Implement configurable control mapping
- Provide real-time MIDI event processing
- Start with DDJ-REV1 turntable support

### UI Module

Creates the user interface using ImGui.

- Design resizable, dockable panels
- Implement theme system for customization
- Ensure accessibility and usability
- Follow Adobe Premiere-inspired layout

## Plugin System

Gamma Array supports extensible plugins for effects and features.

### Plugin Interface

```cpp
namespace gamma::plugins {
class IEffect {
public:
    virtual ~IEffect() = default;
    virtual void initialize() = 0;
    virtual void process(const Frame& input, Frame& output) = 0;
    virtual void cleanup() = 0;
    virtual const char* getName() const = 0;
};

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual void load() = 0;
    virtual void unload() = 0;
    virtual std::vector<std::unique_ptr<IEffect>> getEffects() = 0;
};
}
```

## Performance Guidelines

### Real-time Requirements

- Maintain 60 FPS video processing
- Audio latency under 10ms
- MIDI input latency under 5ms
- Memory usage should be predictable

### Optimization Strategies

- Use object pooling for frequently allocated objects
- Minimize memory allocations in real-time code paths
- Profile regularly to identify bottlenecks
- Use SIMD instructions for vector operations

### GPU Utilization

- Implement effects as GPU shaders when possible
- Use vertex buffer objects for efficient geometry
- Minimize CPU-GPU synchronization
- Support multiple GPU contexts for advanced features

## Documentation

### Code Documentation

- Use Doxygen-style comments for public APIs
- Document complex algorithms and data structures
- Provide usage examples for public interfaces
- Keep documentation up-to-date with code changes

```cpp
/**
 * @brief Processes video frames with applied effects
 * @param input Input video frame
 * @param effects Vector of effects to apply
 * @return Processed video frame
 * @throw std::runtime_error if processing fails
 */
Frame processVideo(const Frame& input, const std::vector<Effect>& effects);
```

### API Documentation

- Maintain API documentation in `docs/api/`
- Include code examples and tutorials
- Document breaking changes in migration guides
- Provide comprehensive plugin development guide

## Contributing

### Before Contributing

1. Read this development guide thoroughly
2. Set up the development environment
3. Familiarize yourself with the codebase
4. Check existing issues and feature requests

### Contribution Process

1. **Fork** the repository
2. **Create** a feature branch
3. **Implement** your changes following coding standards
4. **Test** your changes thoroughly
5. **Document** new features or API changes
6. **Submit** a pull request with clear description

### Code Review Process

All contributions go through code review:

- **Functionality**: Does the code work as intended?
- **Performance**: Does it meet real-time requirements?
- **Style**: Does it follow coding standards?
- **Architecture**: Does it fit the modular design?
- **Documentation**: Is it properly documented?

### Release Process

1. **Feature Freeze**: No new features during release preparation
2. **Testing**: Comprehensive testing on target platforms
3. **Documentation**: Update all documentation and changelogs
4. **Tagging**: Create release tags following semantic versioning
5. **Distribution**: Build and distribute release packages

## Support and Community

- **Issues**: Use GitHub issues for bug reports and feature requests
- **Discussions**: Use GitHub discussions for questions and ideas
- **Documentation**: Check docs/ directory for detailed guides
- **Examples**: See examples/ directory for usage examples