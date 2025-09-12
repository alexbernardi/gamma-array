#include <iostream>
#include "core/Application.h"

int main(int argc, char* argv[]) {
    std::cout << "=== Gamma Array - VJ Application ===" << std::endl;
    std::cout << "Version: Development Build" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    // Force windowed mode (fullscreen capability disabled)
    bool fullscreen = false;  // Always windowed
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--fullscreen" || arg == "-f") {
            std::cout << "Fullscreen mode requested but disabled - using windowed mode" << std::endl;
        } else if (arg == "--windowed" || arg == "-w") {
            std::cout << "Windowed mode requested via command line" << std::endl;
        }
        // Note: fullscreen remains false regardless of arguments
    }
    
    try {
        // Create application instance
        gamma::core::Application app;
        
        // Initialize the application
        if (!app.initialize(fullscreen)) {
            std::cerr << "Failed to initialize application" << std::endl;
            return -1;
        }
        
        std::cout << "Press ESC to exit" << std::endl;
        
        // Run the main loop
        app.run();
        
        // Cleanup is handled by destructor
        std::cout << "Application exited normally" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown application error occurred" << std::endl;
        return -1;
    }
}