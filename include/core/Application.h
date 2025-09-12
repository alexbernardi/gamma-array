#pragma once

// Forward declarations
struct GLFWwindow;

namespace gamma {
namespace core {

/**
 * @brief Main application class managing the lifecycle of Gamma Array
 * 
 * This class coordinates all subsystems including rendering, audio, MIDI, and UI.
 * It follows RAII principles and provides clean initialization/shutdown sequences.
 */
class Application {
public:
    Application();
    ~Application();

    /**
     * @brief Initialize all application subsystems
     * @param fullscreen Whether to run in fullscreen mode (default: true)
     * @return true if initialization succeeds, false otherwise
     */
    bool initialize(bool fullscreen = true);

    /**
     * @brief Main application loop
     * 
     * Runs the main event loop until the application should exit.
     * Handles window events, updates subsystems, and renders frames.
     */
    void run();

    /**
     * @brief Clean shutdown of all subsystems
     */
    void shutdown();

    /**
     * @brief Check if the application should continue running
     * @return true if the application should continue, false to exit
     */
    bool shouldRun() const;

private:
    bool _initialized;
    bool _shouldRun;
    bool _fullscreen;
    GLFWwindow* _window;

    // Core subsystem initialization methods
    bool initializeWindow();
    bool initializeOpenGL();
    bool initializeImGui();
    bool initializeSubsystems();

    // Main loop methods
    void processEvents();
    void update(float deltaTime);
    void render();
    void renderNavigationBar();
    void toggleFullscreen();

    // Cleanup methods
    void cleanupSubsystems();
    void cleanupImGui();
    void cleanupOpenGL();
    void cleanupWindow();
};

} // namespace core
} // namespace gamma