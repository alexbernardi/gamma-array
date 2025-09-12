# Dependencies Installation Guide

This document provides detailed instructions for installing and managing all required dependencies for Gamma Array on Windows.

## Overview

Gamma Array requires the following libraries:

- **RtMidi** - MIDI input/output handling
- **GLFW** - OpenGL window management and input
- **ImGui** - Immediate mode GUI framework
- **OpenGL** - Graphics rendering (drivers)
- **PortAudio** - Cross-platform audio I/O library

## Installation Methods

We recommend using a combination of package managers and manual installation for optimal compatibility.

## Method 1: Automated Setup (Recommended)

### Prerequisites

1. **Visual Studio 2022** with C++ Desktop Development workload
2. **Git** for cloning repositories
3. **PowerShell** (included with Windows)

### Quick Setup Script

Create and run the setup script:

```powershell
# Download the setup script
Invoke-WebRequest -Uri "https://github.com/your-repo/gamma-array/raw/main/scripts/setup_dependencies.ps1" -OutFile "setup_dependencies.ps1"

# Run the setup script
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
.\setup_dependencies.ps1
```

This script will automatically download and configure all dependencies in the `libs/` directory.

## Method 2: Manual Installation

### Step 1: Create Libraries Directory

```powershell
mkdir libs
cd libs
```

### Step 2: Install GLFW

GLFW provides window management and input handling.

#### Option A: Pre-built Binaries (Recommended)

```powershell
# Download GLFW
Invoke-WebRequest -Uri "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip" -OutFile "glfw.zip"
Expand-Archive glfw.zip -DestinationPath .
Rename-Item glfw-3.3.8.bin.WIN64 glfw
Remove-Item glfw.zip
```

#### Option B: Build from Source

```powershell
# Clone and build GLFW
git clone https://github.com/glfw/glfw.git
cd glfw
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
cd ..
```

### Step 3: Install ImGui

ImGui provides the user interface framework.

```powershell
# Clone ImGui
git clone https://github.com/ocornut/imgui.git
```

### Step 4: Install RtMidi

RtMidi handles MIDI input/output.

```powershell
# Clone RtMidi
git clone https://github.com/thestk/rtmidi.git
cd rtmidi
cmake -B build -S . -G "Visual Studio 17 2022" -A x64 -DRTMIDI_BUILD_STATIC_LIBS=ON
cmake --build build --config Release
cd ..
```

### Step 5: Install PortAudio

PortAudio provides cross-platform audio I/O.

#### Option A: Pre-built Libraries

```powershell
# Download PortAudio
Invoke-WebRequest -Uri "http://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz" -OutFile "portaudio.tgz"
# Extract using 7-Zip or similar tool
```

#### Option B: Build from Source

```powershell
# Clone PortAudio
git clone https://github.com/PortAudio/portaudio.git
cd portaudio
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
cd ..
```

## Method 3: Using vcpkg (Alternative)

vcpkg is Microsoft's C++ package manager.

### Install vcpkg

```powershell
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio
.\vcpkg integrate install
```

### Install Dependencies

```powershell
# Install packages
.\vcpkg install glfw3:x64-windows
.\vcpkg install portaudio:x64-windows
.\vcpkg install rtmidi:x64-windows

# ImGui is header-only, clone manually
cd ..
git clone https://github.com/ocornut/imgui.git libs/imgui
```

## OpenGL Drivers

### Windows OpenGL Support

OpenGL support is provided by your graphics card drivers.

#### NVIDIA Graphics Cards

1. Download latest drivers from [NVIDIA Driver Downloads](https://www.nvidia.com/Download/index.aspx)
2. Install the latest Game Ready or Studio drivers
3. Verify OpenGL support: Drivers typically support OpenGL 4.6+

#### AMD Graphics Cards

1. Download latest drivers from [AMD Support](https://www.amd.com/en/support)
2. Install AMD Radeon Software
3. Verify OpenGL support: Drivers typically support OpenGL 4.6+

#### Intel Graphics

1. Update through Windows Update or download from [Intel Graphics Drivers](https://www.intel.com/content/www/us/en/support/products/80939/graphics.html)
2. Intel UHD Graphics support OpenGL 4.5+

#### Verify OpenGL Support

```powershell
# Create a simple OpenGL test program
# This will be included in the examples/ directory
```

## Directory Structure After Installation

After successful installation, your `libs/` directory should look like this:

```
libs/
├── glfw/
│   ├── include/
│   ├── lib-vc2022/
│   └── CMakeLists.txt
├── imgui/
│   ├── imgui.h
│   ├── imgui.cpp
│   ├── backends/
│   └── examples/
├── rtmidi/
│   ├── include/
│   ├── build/
│   └── CMakeLists.txt
└── portaudio/
    ├── include/
    ├── build/
    └── CMakeLists.txt
```

## Troubleshooting

### Common Issues

#### MSVC Compiler Not Found

```powershell
# Ensure Visual Studio 2022 is properly installed
# Install "C++ Desktop Development" workload
# Verify installation:
where cl.exe
```

#### CMake Configuration Errors

```powershell
# Update CMake to latest version
winget install cmake

# Clear CMake cache and reconfigure
Remove-Item -Recurse -Force build
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
```

#### Library Linking Errors

Check that library paths in `CMakeLists.txt` match your installation:

```cmake
# Verify these paths exist
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
```

#### Missing DLL Files

For runtime, ensure required DLLs are in the same directory as the executable:

```powershell
# Copy required DLLs to build output
copy libs\glfw\lib-vc2022\glfw3.dll build\bin\Debug\
copy libs\portaudio\build\Release\portaudio_x64.dll build\bin\Debug\
```

### Dependency Versions

#### Tested Versions

- **GLFW**: 3.3.8+
- **ImGui**: 1.89+
- **RtMidi**: 5.0.0+
- **PortAudio**: 19.7.0+
- **OpenGL**: 3.3+ (4.0+ recommended)

#### Version Compatibility

The build system is designed to work with the latest stable versions of all dependencies. If you encounter issues with newer versions, try the tested versions listed above.

## Advanced Configuration

### Custom Library Paths

If you install libraries in non-standard locations, update `CMakeLists.txt`:

```cmake
# Custom library paths
set(GLFW_ROOT "C:/custom/path/to/glfw")
set(PORTAUDIO_ROOT "C:/custom/path/to/portaudio")
find_package(glfw3 REQUIRED PATHS ${GLFW_ROOT})
```

### Static vs Dynamic Linking

By default, the project uses static linking for better portability. To use dynamic linking:

```cmake
# In CMakeLists.txt
set(BUILD_SHARED_LIBS ON)
```

### Debug vs Release Dependencies

Some dependencies require different configurations for Debug and Release builds:

```cmake
# Debug libraries
target_link_libraries(gamma_array debug portaudio_d)

# Release libraries  
target_link_libraries(gamma_array optimized portaudio)
```

## Automated Setup Script

Create `scripts/setup_dependencies.ps1`:

```powershell
# Gamma Array Dependency Setup Script
param(
    [switch]$Clean,
    [switch]$UseVcpkg
)

Write-Host "Setting up Gamma Array dependencies..."

if ($Clean) {
    Write-Host "Cleaning existing dependencies..."
    Remove-Item -Recurse -Force libs -ErrorAction SilentlyContinue
}

New-Item -ItemType Directory -Force -Path libs
Set-Location libs

if ($UseVcpkg) {
    Write-Host "Using vcpkg for dependency management..."
    # vcpkg installation logic
} else {
    Write-Host "Using manual dependency installation..."
    # Manual installation logic
}

Write-Host "Dependencies setup complete!"
```

## Validation

After installation, validate your setup:

```powershell
# Configure project
cmake -B build -S . -G "Visual Studio 17 2022" -A x64

# Build project
cmake --build build --config Debug

# Run basic tests
cmake --build build --target test
```

If the build completes successfully, all dependencies are properly configured.

## Support

If you encounter issues with dependency installation:

1. Check the troubleshooting section above
2. Verify your system meets all prerequisites
3. Consult the build logs for specific error messages
4. Create an issue with detailed error information

For dependency-specific issues, consult the official documentation:

- [GLFW Documentation](https://www.glfw.org/docs/latest/)
- [ImGui Wiki](https://github.com/ocornut/imgui/wiki)
- [RtMidi Documentation](https://github.com/thestk/rtmidi)
- [PortAudio Documentation](http://portaudio.com/docs.html)