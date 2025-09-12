#pragma once

#include "ui/WorkspacePanel.h"

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
    // Output state
    bool _showWaveform;
    bool _showMonitoring;
    float _outputLevel;
    
    // UI helpers
    void renderVideoOutput();
    void renderWaveformOverlay();
    void renderMonitoringInfo();
    void renderOutputControls();
};

} // namespace ui
} // namespace gamma