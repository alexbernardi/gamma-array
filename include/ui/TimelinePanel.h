#pragma once

#include "ui/WorkspacePanel.h"

namespace gamma {
namespace ui {

/**
 * @brief Timeline workspace panel for video scrubbing and scratching
 * 
 * This panel stretches across the bottom of the application and provides
 * timeline-based controls for video manipulation, scratching, and sequencing.
 * Will integrate with DDJ-REV1 jog wheel controls for turntable-style interaction.
 */
class TimelinePanel : public WorkspacePanel {
public:
    TimelinePanel();
    virtual ~TimelinePanel() = default;

    /**
     * @brief Render the timeline panel UI
     * Shows timeline scrubber, playback controls, and scratch interface
     */
    void render() override;

    /**
     * @brief Update timeline state
     * @param deltaTime Time elapsed since last frame
     */
    void update(float deltaTime) override;

private:
    // Timeline state
    float _currentTime;
    float _totalDuration;
    bool _isPlaying;
    bool _isScrubbing;
    
    // UI helpers
    void renderTimelineControls();
    void renderScrubber();
    void renderPlaybackButtons();
};

} // namespace ui
} // namespace gamma