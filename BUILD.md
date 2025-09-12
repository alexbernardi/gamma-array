# Build Instructions

This document provides step-by-step instructions for building Gamma Array on Windows using Visual Studio 2022 and VS Code.

## Prerequisites

Before building Gamma Array, ensure you have the following installed:

- **Visual Studio 2022** (Community, Professional, or Enterprise)
  - C++ Desktop Development workload
  - Windows 10/11 SDK
  - CMake tools (included with VS 2022)
- **Git** for version control
- **VS Code** (recommended for development)

## Quick Build

If you have all dependencies set up, you can build quickly using VS Code:

1. Open the project in VS Code
2. Press `Ctrl+Shift+P` and run `CMake: Configure`
3. Press `Ctrl+Shift+P` and run `CMake: Build`
4. Or use the build task: `Ctrl+Shift+P` > `Tasks: Run Task` > `CMake Build Debug`

## Detailed Build Process

### Step 1: Clone and Setup

```powershell
# Clone the repository
git clone <repository-url> gamma_array
cd gamma_array

# Create build directory
mkdir build
```

### Step 2: Install Dependencies

See [DEPENDENCIES.md](DEPENDENCIES.md) for detailed dependency installation instructions.

### Step 3: Configure CMake

```powershell
# Configure with Visual Studio 2022
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
```

### Step 4: Build the Project

```powershell
# Build Debug configuration
cmake --build build --config Debug

# Build Release configuration
cmake --build build --config Release
```

### Step 5: Run the Application

```powershell
# Run Debug build
./build/bin/Debug/GammaArray_d.exe

# Run Release build
./build/bin/Release/GammaArray.exe
```

## VS Code Integration

### Building from VS Code

1. Open the project folder in VS Code
2. Install recommended extensions when prompted
3. Use the Command Palette (`Ctrl+Shift+P`):
   - `CMake: Configure` - Configure the project
   - `CMake: Build` - Build the project
   - `CMake: Clean` - Clean build artifacts

### Using Tasks

Press `Ctrl+Shift+P` > `Tasks: Run Task` and choose from:

- **CMake Configure** - Configure CMake project
- **CMake Build Debug** - Build debug configuration
- **CMake Build Release** - Build release configuration
- **Clean Build** - Clean all build artifacts
- **Run Gamma Array (Debug)** - Build and run debug version
- **Run Gamma Array (Release)** - Build and run release version

### Debugging

1. Set breakpoints in your code
2. Press `F5` or use `Debug: Start Debugging`
3. Choose "Debug Gamma Array" configuration

## Build Configurations

### Debug Build
- Optimizations disabled
- Debug symbols included
- Additional runtime checks
- Console output enabled

```powershell
cmake --build build --config Debug
```

### Release Build
- Full optimizations enabled
- Debug symbols stripped
- Performance optimized
- Minimal console output

```powershell
cmake --build build --config Release
```

## Troubleshooting

### Common Issues

**CMake not found:**
```powershell
# Add CMake to PATH or install via Visual Studio Installer
```

**MSVC compiler not found:**
```powershell
# Ensure Visual Studio 2022 with C++ workload is installed
# Or specify compiler path:
cmake -B build -S . -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_COMPILER="C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe"
```

**Missing dependencies:**
- See [DEPENDENCIES.md](DEPENDENCIES.md) for installation instructions
- Ensure all required libraries are in the `libs/` directory

**Build errors:**
- Check that all header files are properly included
- Verify that library paths are correct in CMakeLists.txt
- Ensure Windows SDK version matches your installation

### Clean Build

If you encounter persistent build issues:

```powershell
# Remove build directory and start fresh
Remove-Item -Recurse -Force build
mkdir build
cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
```

### Performance Tips

- Use Release builds for performance testing
- Enable parallel builds: `cmake --build build --config Release --parallel`
- Use Ninja generator for faster builds: `-G Ninja`

## Build System Details

### CMake Configuration

The project uses CMake 3.20+ with the following features:

- Automatic dependency detection
- Cross-platform library linking
- Debug/Release configurations
- VS Code integration
- Extensible for additional platforms

### Output Structure

```
build/
├── bin/
│   ├── Debug/
│   │   └── GammaArray_d.exe
│   └── Release/
│       └── GammaArray.exe
└── lib/
    ├── Debug/
    └── Release/
```

### Dependency Management

Dependencies are managed through:
- Git submodules for source libraries
- Package managers (vcpkg) for system libraries
- Manual installation for specialized libraries

See [DEPENDENCIES.md](DEPENDENCIES.md) for complete dependency setup instructions.