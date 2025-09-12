#include "ui/TimelinePanel.h"
#include "ui/WorkspaceManager.h"
#include "imgui.h"

namespace gamma {
namespace ui {

TimelinePanel::TimelinePanel()
    : WorkspacePanel("Timeline")
    , _currentTime(0.0f)
    , _totalDuration(100.0f)
    , _isPlaying(false)
    , _isScrubbing(false) {
}

void TimelinePanel::render() {
    if (!_visible) return;

    // Get layout dimensions from workspace manager
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float timelineHeight = _workspaceManager ? _workspaceManager->getTimelineHeight() : 120.0f;
    
    // Position at bottom of screen
    ImVec2 panelPos = ImVec2(0, viewport->Size.y - timelineHeight);
    ImVec2 panelSize = ImVec2(viewport->Size.x, timelineHeight);
    
    ImGui::SetNextWindowPos(panelPos);
    ImGui::SetNextWindowSize(panelSize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_NoTitleBar;
    
    if (ImGui::Begin("Timeline", &_visible, flags)) {
        // Panel header
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
        ImGui::Text("Timeline & Scratching Interface");
        ImGui::PopStyleColor();
        ImGui::Separator();
        
        // Render timeline components
        renderPlaybackButtons();
        ImGui::SameLine();
        renderScrubber();
        renderTimelineControls();
    }
    ImGui::End();
}

void TimelinePanel::update(float deltaTime) {
    if (_isPlaying && !_isScrubbing) {
        _currentTime += deltaTime;
        if (_currentTime > _totalDuration) {
            _currentTime = _totalDuration;
            _isPlaying = false;
        }
    }
}

void TimelinePanel::renderPlaybackButtons() {
    // Play/Pause button
    const char* playIcon = _isPlaying ? "||" : ">";
    if (ImGui::Button(playIcon, ImVec2(40, 25))) {
        _isPlaying = !_isPlaying;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip(_isPlaying ? "Pause" : "Play");
    }
    
    ImGui::SameLine();
    
    // Stop button
    if (ImGui::Button("[]", ImVec2(30, 25))) {
        _isPlaying = false;
        _currentTime = 0.0f;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Stop");
    }
}

void TimelinePanel::renderScrubber() {
    ImGui::PushItemWidth(-100); // Leave space for time display
    
    float oldTime = _currentTime;
    if (ImGui::SliderFloat("##timeline", &_currentTime, 0.0f, _totalDuration, "")) {
        if (oldTime != _currentTime) {
            _isScrubbing = true;
            _isPlaying = false;
        }
    } else {
        _isScrubbing = false;
    }
    
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Drag to scrub timeline - Ready for DDJ-REV1 jog wheel integration");
    }
    
    ImGui::PopItemWidth();
}

void TimelinePanel::renderTimelineControls() {
    // Time display
    ImGui::SameLine();
    ImGui::Text("%.1fs / %.1fs", _currentTime, _totalDuration);
    
    // Additional controls row
    ImGui::Text("[AUDIO] Audio Sync | [MIDI] MIDI: Ready | [VIDEO] Video: Loading...");
    
    // Status indicators
    ImGui::SameLine();
    if (_isScrubbing) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
        ImGui::Text("SCRUBBING");
        ImGui::PopStyleColor();
    } else if (_isPlaying) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
        ImGui::Text("PLAYING");
        ImGui::PopStyleColor();
    } else {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f)); // Gray
        ImGui::Text("STOPPED");
        ImGui::PopStyleColor();
    }
}

} // namespace ui
} // namespace gamma