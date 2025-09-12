#pragma once

#include "ui/WorkspacePanel.h"

namespace gamma {
namespace core { class Application; }
namespace midi { class MidiManager; }
}

namespace gamma {
namespace ui {

/**
 * @brief Output workspace panel for video display and monitoring
 * 
 * This panel occupies the central area and displays the main video output
 * with overlays for monitoring, waveforms, and real-time effects preview.
 * Acts as the primary visual feedback for VJing performance.
 */
class OutputPanel : public WorkspacePanel {
public:
    OutputPanel();
    virtual ~OutputPanel() = default;

    /**
     * @brief Set reference to the application for accessing subsystems
     * @param app Pointer to the main application instance
     */
    void setApplication(gamma::core::Application* app);

    /**
     * @brief Render the output panel UI
     * Shows video output, waveform overlay, and monitoring info
     */
    void render() override;

    /**
     * @brief Update output panel state
     * @param deltaTime Time elapsed since last frame
     */
    void update(float deltaTime) override;

private:
    // Application reference for accessing subsystems
    gamma::core::Application* _application;
    
    // Output state
    bool _showWaveform;
    bool _showMonitoring;
    float _outputLevel;
    
    // MIDI state
    int _selectedDevice;
    bool _isConnected;
    
    // Jog wheel state
    float _jogWheelLeftRotation;   // Left jog wheel rotation in degrees (0-360)
    float _jogWheelRightRotation;  // Right jog wheel rotation in degrees (0-360)
    
    // UI helpers - existing output functionality
    void renderVideoOutput();
    void renderWaveformOverlay();
    void renderMonitoringInfo();
    void renderOutputControls();
    
    // Tab rendering methods
    void renderOutputTab();
    void renderMidiSetupTab();
    
    // MIDI UI helpers
    void renderMidiDeviceSelection();
    void renderMidiControlMapping();
    void renderMidiStatus();
    void renderMidiSignalLog();
    void renderMidiConfigButtons();
    
    // Jog wheel control
    void updateJogWheelRotation(int channel, float deltaRotation); // channel 1=left, 2=right
};

} // namespace ui
} // namespace gamma