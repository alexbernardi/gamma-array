# Quick Reference

## Fast Development Commands

### One-Time Setup
```powershell
.\dev.ps1 setup
```

### Daily Development
```powershell
# Quick build and run
.\dev.ps1 run

# Just build
.\dev.ps1 build

# Check what's installed
.\dev.ps1 deps

# Clean rebuild if issues
.\dev.ps1 rebuild
```

### VS Code Tasks (Ctrl+Shift+P → Tasks: Run Task)
- CMake Build Debug
- Run Gamma Array (Debug)

### Next Development Steps
1. Run dependency setup
2. Create core application class
3. Add DDJ-REV1 MIDI mapping
4. Build basic UI layout

See CONTEXT.md for complete development reference.