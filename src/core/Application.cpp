#include "core/Application.h"
#include <iostream>
#include <chrono>

// Include GLFW first which includes OpenGL headers correctly
#include <GLFW/glfw3.h>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace gamma {
namespace core {

namespace {
    // Static callback for GLFW error handling
    void glfwErrorCallback(int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    }

    // Static callback for GLFW window close
    void glfwWindowCloseCallback(GLFWwindow* window) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) {
            // Signal the application to stop running
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    // Static callback for keyboard input
    void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) {
            // ESC key to exit fullscreen/close application
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }
    }
}

Application::Application() 
    : _initialized(false)
    , _shouldRun(false)
    , _fullscreen(true)
    , _window(nullptr) {
}

Application::~Application() {
    if (_initialized) {
        shutdown();
    }
}

bool Application::initialize(bool fullscreen) {
    if (_initialized) {
        std::cout << "Application already initialized" << std::endl;
        return true;
    }

    _fullscreen = fullscreen;
    std::cout << "Initializing Gamma Array in " << (_fullscreen ? "fullscreen" : "windowed") << " mode..." << std::endl;

    // Initialize in order: Window → OpenGL → ImGui → Subsystems
    if (!initializeWindow()) {
        std::cerr << "Failed to initialize window system" << std::endl;
        return false;
    }

    if (!initializeOpenGL()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        cleanupWindow();
        return false;
    }

    if (!initializeImGui()) {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        cleanupOpenGL();
        cleanupWindow();
        return false;
    }

    if (!initializeSubsystems()) {
        std::cerr << "Failed to initialize subsystems" << std::endl;
        cleanupImGui();
        cleanupOpenGL();
        cleanupWindow();
        return false;
    }

    _initialized = true;
    _shouldRun = true;
    
    std::cout << "Gamma Array initialized successfully" << std::endl;
    return true;
}

void Application::run() {
    if (!_initialized) {
        std::cerr << "Cannot run uninitialized application" << std::endl;
        return;
    }

    std::cout << "Starting main application loop..." << std::endl;

    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (shouldRun()) {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime);
        float deltaTime = duration.count() / 1000000.0f; // Convert to seconds
        lastTime = currentTime;

        // Main loop steps
        processEvents();
        update(deltaTime);
        render();
    }

    std::cout << "Main loop ended" << std::endl;
}

void Application::shutdown() {
    if (!_initialized) {
        return;
    }

    std::cout << "Shutting down Gamma Array..." << std::endl;

    _shouldRun = false;

    // Cleanup in reverse order: Subsystems → ImGui → OpenGL → Window
    cleanupSubsystems();
    cleanupImGui();
    cleanupOpenGL();
    cleanupWindow();

    _initialized = false;
    std::cout << "Gamma Array shutdown complete" << std::endl;
}

bool Application::shouldRun() const {
    return _shouldRun && _window && !glfwWindowShouldClose(_window);
}

bool Application::initializeWindow() {
    std::cout << "Initializing window system..." << std::endl;

    // Set GLFW error callback
    glfwSetErrorCallback(glfwErrorCallback);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Get primary monitor and its video mode
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor) {
        std::cerr << "Failed to get primary monitor" << std::endl;
        glfwTerminate();
        return false;
    }

    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    if (!videoMode) {
        std::cerr << "Failed to get video mode" << std::endl;
        glfwTerminate();
        return false;
    }

    std::cout << "Detected monitor resolution: " << videoMode->width << "x" << videoMode->height 
              << " @ " << videoMode->refreshRate << "Hz" << std::endl;

    // Configure GLFW for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window (fullscreen or windowed based on setting)
    if (_fullscreen) {
        // Set refresh rate hint for fullscreen
        glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
        
        _window = glfwCreateWindow(
            videoMode->width, 
            videoMode->height, 
            "Gamma Array", 
            primaryMonitor,  // Fullscreen on primary monitor
            nullptr
        );
        
        if (!_window) {
            std::cerr << "Failed to create fullscreen GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        std::cout << "Fullscreen window created: " << videoMode->width << "x" << videoMode->height << std::endl;
    } else {
        // Create windowed mode with reasonable size
        int windowWidth = static_cast<int>(videoMode->width * 0.8f);
        int windowHeight = static_cast<int>(videoMode->height * 0.8f);
        
        _window = glfwCreateWindow(
            windowWidth,
            windowHeight,
            "Gamma Array", 
            nullptr,  // Windowed mode
            nullptr
        );
        
        if (!_window) {
            std::cerr << "Failed to create windowed GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        
        // Center the window on screen
        int xpos = (videoMode->width - windowWidth) / 2;
        int ypos = (videoMode->height - windowHeight) / 2;
        glfwSetWindowPos(_window, xpos, ypos);
        
        std::cout << "Windowed mode created: " << windowWidth << "x" << windowHeight << std::endl;
    }

    // Make the window's context current
    glfwMakeContextCurrent(_window);

    // Set window user pointer for callbacks
    glfwSetWindowUserPointer(_window, this);

    // Set callbacks
    glfwSetWindowCloseCallback(_window, glfwWindowCloseCallback);
    glfwSetKeyCallback(_window, glfwKeyCallback);

    // Enable vsync
    glfwSwapInterval(1);

    return true;
}

bool Application::initializeOpenGL() {
    std::cout << "Initializing OpenGL..." << std::endl;

    // Get OpenGL version info
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    }
    if (renderer) {
        std::cout << "OpenGL Renderer: " << renderer << std::endl;
    }

    // Set basic OpenGL state
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    return true;
}

bool Application::initializeImGui() {
    std::cout << "Initializing ImGui..." << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Enable keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Dear ImGui style (Adobe-like dark theme)
    ImGui::StyleColorsDark();
    
    // Customize colors to match Adobe products
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    
    // Adobe-inspired color scheme
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);  // Dark background
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f); // Very dark menu bar
    colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);    // Button color
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);      // Light text
    
    // Style adjustments for a more professional look
    style.WindowRounding = 0.0f;
    style.FrameRounding = 2.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 2.0f;
    
    // Setup Platform/Renderer backends
    if (!ImGui_ImplGlfw_InitForOpenGL(_window, true)) {
        std::cerr << "Failed to initialize ImGui GLFW backend" << std::endl;
        return false;
    }
    
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        std::cerr << "Failed to initialize ImGui OpenGL3 backend" << std::endl;
        ImGui_ImplGlfw_Shutdown();
        return false;
    }

    std::cout << "ImGui initialized successfully" << std::endl;
    return true;
}

bool Application::initializeSubsystems() {
    std::cout << "Initializing subsystems..." << std::endl;

    // TODO: Initialize rendering engine
    // TODO: Initialize audio engine  
    // TODO: Initialize MIDI system
    // TODO: Initialize UI system

    std::cout << "Core subsystems initialized" << std::endl;
    return true;
}

void Application::processEvents() {
    // Poll for and process events
    glfwPollEvents();
}

void Application::update(float deltaTime) {
    // TODO: Update all subsystems with delta time
    // TODO: Update MIDI input
    // TODO: Update audio processing
    // TODO: Update UI state
}

void Application::render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create the navigation bar
    renderNavigationBar();

    // TODO: Render video effects
    // TODO: Render main UI panels

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap front and back buffers
    if (_window) {
        glfwSwapBuffers(_window);
    }
}

void Application::renderNavigationBar() {
    // Get window size for full-width navigation bar
    int windowWidth, windowHeight;
    glfwGetWindowSize(_window, &windowWidth, &windowHeight);
    
    // Set navigation bar to full width at top of window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(windowWidth), 32));
    
    // Navigation bar window flags (no title bar, no resize, etc.)
    ImGuiWindowFlags navFlags = ImGuiWindowFlags_NoTitleBar | 
                               ImGuiWindowFlags_NoResize | 
                               ImGuiWindowFlags_NoMove | 
                               ImGuiWindowFlags_NoScrollbar | 
                               ImGuiWindowFlags_NoSavedSettings |
                               ImGuiWindowFlags_MenuBar;
    
    if (ImGui::Begin("NavigationBar", nullptr, navFlags)) {
        if (ImGui::BeginMenuBar()) {
            // Left side - Application name with icon
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f)); // Cyan color
            ImGui::Text("● Gamma Array");
            ImGui::PopStyleColor();
            
            // Center info (optional)
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 50);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f)); // Gray text
            ImGui::Text("VJ Application");
            ImGui::PopStyleColor();
            
            // Right side - Window controls
            float availableWidth = ImGui::GetContentRegionAvail().x;
            float buttonWidth = 28.0f;
            float spacing = 2.0f;
            float totalButtonWidth = (buttonWidth * 3) + (spacing * 2);
            
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availableWidth - totalButtonWidth);
            
            // Minimize button
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
            
            if (ImGui::Button("—", ImVec2(buttonWidth, 20))) {
                glfwIconifyWindow(_window);
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Minimize");
            }
            
            ImGui::SameLine(0, spacing);
            
            // Fullscreen/Windowed toggle button
            const char* toggleIcon = _fullscreen ? "⧉" : "⧉";
            const char* toggleTooltip = _fullscreen ? "Switch to Windowed" : "Switch to Fullscreen";
            
            if (ImGui::Button(toggleIcon, ImVec2(buttonWidth, 20))) {
                toggleFullscreen();
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", toggleTooltip);
            }
            
            ImGui::SameLine(0, spacing);
            
            // Exit button
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // Red on hover
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.1f, 1.0f));
            
            if (ImGui::Button("×", ImVec2(buttonWidth, 20))) {
                glfwSetWindowShouldClose(_window, GLFW_TRUE);
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Exit");
            }
            
            ImGui::PopStyleColor(5); // Pop all button style colors
            
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}

void Application::toggleFullscreen() {
    if (!_window) return;
    
    _fullscreen = !_fullscreen;
    
    // Get monitor information
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    
    if (_fullscreen) {
        // Switch to fullscreen
        glfwSetWindowMonitor(_window, primaryMonitor, 0, 0, 
                           videoMode->width, videoMode->height, 
                           videoMode->refreshRate);
        std::cout << "Switched to fullscreen mode" << std::endl;
    } else {
        // Switch to windowed mode
        int windowWidth = static_cast<int>(videoMode->width * 0.8f);
        int windowHeight = static_cast<int>(videoMode->height * 0.8f);
        int xpos = (videoMode->width - windowWidth) / 2;
        int ypos = (videoMode->height - windowHeight) / 2;
        
        glfwSetWindowMonitor(_window, nullptr, xpos, ypos, 
                           windowWidth, windowHeight, 0);
        std::cout << "Switched to windowed mode" << std::endl;
    }
}

void Application::cleanupImGui() {
    std::cout << "Cleaning up ImGui..." << std::endl;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::cleanupSubsystems() {
    std::cout << "Cleaning up subsystems..." << std::endl;
    // TODO: Cleanup UI system
    // TODO: Cleanup MIDI system
    // TODO: Cleanup audio engine
    // TODO: Cleanup rendering engine
}

void Application::cleanupOpenGL() {
    std::cout << "Cleaning up OpenGL..." << std::endl;
    // OpenGL cleanup is handled by GLFW context destruction
}

void Application::cleanupWindow() {
    std::cout << "Cleaning up window system..." << std::endl;
    glfwTerminate();
}

} // namespace core
} // namespace gamma