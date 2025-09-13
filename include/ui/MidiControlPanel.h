#pragma once

#include "ui/WorkspacePanel.h"

namespace gamma {
namespace core { class Application; }
namespace midi { class MidiManager; }
}

namespace gamma {
namespace ui {

/**
 * @brief MIDI Control Panel for MIDI device management and control mapping
 * 
 * This panel handles all MIDI-related functionality including device selection,
 * jog wheel control mapping, MIDI signal monitoring, and controller configuration.
 * Provides visual feedback for MIDI input and controller state.
 */
class MidiControlPanel : public WorkspacePanel {
public:
    MidiControlPanel();
    virtual ~MidiControlPanel() = default;

    /**
     * @brief Set reference to the application for accessing MIDI subsystem
     * @param app Pointer to the main application instance
     */
    void setApplication(gamma::core::Application* app);

    /**
     * @brief Render the MIDI control panel UI
     * Shows MIDI device selection, control mapping, and signal monitoring
     */
    void render() override;

    /**
     * @brief Update MIDI panel state
     * @param deltaTime Time elapsed since last frame
     */
    void update(float deltaTime) override;

private:
    // Application reference for accessing MIDI subsystem
    gamma::core::Application* _application;
    
    // MIDI state
    int _selectedDevice;
    bool _isConnected;
    
    // Jog wheel state
    float _jogWheelLeftRotation;   // Left jog wheel rotation in degrees (0-360)
    float _jogWheelRightRotation;  // Right jog wheel rotation in degrees (0-360)
    
    // MIDI UI rendering methods
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