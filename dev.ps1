# Gamma Array Development Assistant
# This script provides quick access to common development tasks

param(
    [string]$Action = "help",
    [switch]$Clean,
    [switch]$Release
)

$ProjectRoot = $PSScriptRoot
Set-Location $ProjectRoot

function Show-Help {
    Write-Host @"
Gamma Array Development Assistant

Usage: .\dev.ps1 [action] [options]

Actions:
  setup       - Install dependencies and configure project
  build       - Build the project (Debug by default)
  run         - Build and run the application
  clean       - Clean build artifacts
  rebuild     - Clean and rebuild
  deps        - Check dependency status
  context     - Show development context
  help        - Show this help

Options:
  -Clean      - Clean before building
  -Release    - Use Release configuration instead of Debug

Examples:
  .\dev.ps1 setup                    # First-time setup
  .\dev.ps1 build                    # Quick build
  .\dev.ps1 run                      # Build and run
  .\dev.ps1 build -Release           # Release build
  .\dev.ps1 rebuild -Clean           # Full rebuild

"@ -ForegroundColor Green
}

function Invoke-Setup {
    Write-Host "Setting up Gamma Array development environment..." -ForegroundColor Green
    
    # Run dependency setup
    if (Test-Path "scripts\setup_dependencies.ps1") {
        & "scripts\setup_dependencies.ps1"
    } else {
        Write-Host "Dependency setup script not found" -ForegroundColor Red
        return
    }
    
    # Configure CMake
    Write-Host "Configuring CMake..." -ForegroundColor Yellow
    cmake -B build -S . -G "Visual Studio 17 2022" -A x64
    
    Write-Host "Setup complete! Use '.\dev.ps1 build' to build the project." -ForegroundColor Green
}

function Invoke-Build {
    $Config = if ($Release) { "Release" } else { "Debug" }
    
    if ($Clean) {
        Write-Host "Cleaning build..." -ForegroundColor Yellow
        if (Test-Path "build") {
            Remove-Item -Recurse -Force build
        }
        cmake -B build -S . -G "Visual Studio 17 2022" -A x64
    }
    
    Write-Host "Building Gamma Array ($Config)..." -ForegroundColor Yellow
    cmake --build build --config $Config
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "[OK] Build successful!" -ForegroundColor Green
    } else {
        Write-Host "[X] Build failed!" -ForegroundColor Red
    }
}

function Invoke-Run {
    $Config = if ($Release) { "Release" } else { "Debug" }
    
    # Build first
    Invoke-Build
    
    if ($LASTEXITCODE -eq 0) {
        $ExeName = if ($Release) { "GammaArray.exe" } else { "GammaArray_d.exe" }
        $ExePath = "build\bin\$Config\$ExeName"
        
        if (Test-Path $ExePath) {
            Write-Host "Running Gamma Array..." -ForegroundColor Yellow
            & $ExePath
        } else {
            Write-Host "Executable not found: $ExePath" -ForegroundColor Red
        }
    }
}

function Invoke-Clean {
    Write-Host "Cleaning build artifacts..." -ForegroundColor Yellow
    if (Test-Path "build") {
        Remove-Item -Recurse -Force build
        Write-Host "[OK] Build directory cleaned" -ForegroundColor Green
    }
}

function Invoke-Rebuild {
    Invoke-Clean
    Write-Host "Reconfiguring and building..." -ForegroundColor Yellow
    cmake -B build -S . -G "Visual Studio 17 2022" -A x64
    Invoke-Build
}

function Show-Dependencies {
    Write-Host "Dependency Status:" -ForegroundColor Green
    
    $deps = @(
        @{Name="GLFW"; Path="libs\glfw"},
        @{Name="ImGui"; Path="libs\imgui"}, 
        @{Name="RtMidi"; Path="libs\rtmidi"},
        @{Name="PortAudio"; Path="libs\portaudio"}
    )
    
    foreach ($dep in $deps) {
        $status = if (Test-Path $dep.Path) { "[OK] Installed" } else { "[X] Missing" }
        $color = if (Test-Path $dep.Path) { "Green" } else { "Red" }
        Write-Host "  $($dep.Name): $status" -ForegroundColor $color
    }
    
    if (Test-Path "build\CMakeCache.txt") {
        Write-Host "  CMake: [OK] Configured" -ForegroundColor Green
    } else {
        Write-Host "  CMake: [X] Not configured" -ForegroundColor Red
    }
}

function Show-Context {
    if (Test-Path "CONTEXT.md") {
        Get-Content "CONTEXT.md" | Select-Object -First 50
        Write-Host "..." -ForegroundColor Gray
        Write-Host "See CONTEXT.md for full development context" -ForegroundColor Yellow
    } else {
        Write-Host "CONTEXT.md not found" -ForegroundColor Red
    }
}

# Main script logic
switch ($Action.ToLower()) {
    "setup" { Invoke-Setup }
    "build" { Invoke-Build }
    "run" { Invoke-Run }
    "clean" { Invoke-Clean }
    "rebuild" { Invoke-Rebuild }
    "deps" { Show-Dependencies }
    "context" { Show-Context }
    "help" { Show-Help }
    default { 
        Write-Host "Unknown action: $Action" -ForegroundColor Red
        Show-Help 
    }
}