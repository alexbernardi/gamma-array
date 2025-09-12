# Gamma Array Dependencies Setup Script
# Run this script to automatically download and configure all required dependencies

param(
    [switch]$Clean,
    [switch]$UseVcpkg,
    [switch]$Help
)

if ($Help) {
    Write-Host @"
Gamma Array Dependencies Setup Script

Usage: .\setup_dependencies.ps1 [options]

Options:
  -Clean     Remove existing libs directory before setup
  -UseVcpkg  Use vcpkg package manager (alternative method)
  -Help      Show this help message

Examples:
  .\setup_dependencies.ps1          # Standard setup
  .\setup_dependencies.ps1 -Clean   # Clean install
  .\setup_dependencies.ps1 -UseVcpkg # Use vcpkg
"@
    exit 0
}

Write-Host "=== Gamma Array Dependencies Setup ===" -ForegroundColor Green

# Check prerequisites
Write-Host "Checking prerequisites..." -ForegroundColor Yellow

# Check Git
try {
    $gitVersion = git --version
    Write-Host "[OK] Git found: $gitVersion" -ForegroundColor Green
} catch {
    Write-Host "[X] Git not found. Please install Git first." -ForegroundColor Red
    exit 1
}

# Check CMake
try {
    $cmakeVersion = cmake --version | Select-Object -First 1
    Write-Host "[OK] CMake found: $cmakeVersion" -ForegroundColor Green
} catch {
    Write-Host "[X] CMake not found. Please install CMake first." -ForegroundColor Red
    exit 1
}

# Clean existing dependencies if requested
if ($Clean -and (Test-Path "libs")) {
    Write-Host "Cleaning existing dependencies..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force libs
}

# Create libs directory
New-Item -ItemType Directory -Force -Path libs | Out-Null
Set-Location libs

Write-Host "Setting up dependencies manually..." -ForegroundColor Yellow

# Install GLFW
Write-Host "Installing GLFW..." -ForegroundColor Yellow
if (-not (Test-Path "glfw")) {
    try {
        Invoke-WebRequest -Uri "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip" -OutFile "glfw.zip"
        Expand-Archive glfw.zip -DestinationPath .
        Rename-Item glfw-3.3.8.bin.WIN64 glfw
        Remove-Item glfw.zip
        Write-Host "[OK] GLFW installed" -ForegroundColor Green
    } catch {
        Write-Host "[X] Failed to install GLFW: $($_.Exception.Message)" -ForegroundColor Red
    }
}

# Install ImGui
Write-Host "Installing ImGui..." -ForegroundColor Yellow
if (-not (Test-Path "imgui")) {
    try {
        git clone https://github.com/ocornut/imgui.git
        Write-Host "[OK] ImGui installed" -ForegroundColor Green
    } catch {
        Write-Host "[X] Failed to install ImGui: $($_.Exception.Message)" -ForegroundColor Red
    }
}

# Install RtMidi
Write-Host "Installing RtMidi..." -ForegroundColor Yellow
if (-not (Test-Path "rtmidi")) {
    try {
        git clone https://github.com/thestk/rtmidi.git
        Set-Location rtmidi
        cmake -B build -S . -G "Visual Studio 17 2022" -A x64 -DRTMIDI_BUILD_STATIC_LIBS=ON
        cmake --build build --config Release
        Set-Location ..
        Write-Host "[OK] RtMidi installed and built" -ForegroundColor Green
    } catch {
        Write-Host "[X] Failed to install RtMidi: $($_.Exception.Message)" -ForegroundColor Red
        Set-Location ..
    }
}

# Install PortAudio
Write-Host "Installing PortAudio..." -ForegroundColor Yellow
if (-not (Test-Path "portaudio")) {
    try {
        git clone https://github.com/PortAudio/portaudio.git
        Set-Location portaudio
        cmake -B build -S . -G "Visual Studio 17 2022" -A x64
        cmake --build build --config Release
        Set-Location ..
        Write-Host "[OK] PortAudio installed and built" -ForegroundColor Green
    } catch {
        Write-Host "[X] Failed to install PortAudio: $($_.Exception.Message)" -ForegroundColor Red
        Set-Location ..
    }
}

Set-Location ..

Write-Host "=== Dependencies Setup Complete ===" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "1. Configure the project: cmake -B build -S . -G 'Visual Studio 17 2022' -A x64"
Write-Host "2. Build the project: cmake --build build --config Debug"
Write-Host "3. See BUILD.md for detailed build instructions"