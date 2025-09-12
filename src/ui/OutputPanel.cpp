#include "ui/OutputPanel.h"
#include "ui/WorkspaceManager.h"
#include "imgui.h"
#include <cmath>

namespace gamma {
namespace ui {

OutputPanel::OutputPanel()
    : WorkspacePanel("Output")
    , _showWaveform(true)
    , _showMonitoring(true)
    , _outputLevel(0.75f) {
}

void OutputPanel::render() {
    if (!_visible) return;

    // Safety check for ImGui context
    if (!ImGui::GetCurrentContext()) return;

    // Get layout dimensions from workspace manager
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    if (!viewport) return; // Safety check
    
    float navBarHeight = _workspaceManager ? _workspaceManager->getNavBarHeight() : 0.0f;
    float timelineHeight = _workspaceManager ? _workspaceManager->getTimelineHeight() : 120.0f;
    float sidebarWidth = _workspaceManager ? _workspaceManager->getSidebarWidth() : 300.0f;
    
    // Calculate panel dimensions (center area between sidebars, above timeline)
    ImVec2 panelPos = ImVec2(sidebarWidth, navBarHeight);
    ImVec2 panelSize = ImVec2(viewport->Size.x - (sidebarWidth * 2), 
                             viewport->Size.y - navBarHeight - timelineHeight);
    
    ImGui::SetNextWindowPos(panelPos);
    ImGui::SetNextWindowSize(panelSize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_NoTitleBar;
    
    if (ImGui::Begin("Output", &_visible, flags)) {
        // Create tabbed interface
        if (ImGui::BeginTabBar("OutputTabs", ImGuiTabBarFlags_None)) {
            
            // Output Tab - for video display
            if (ImGui::BeginTabItem("Output")) {
                renderOutputTab();
                ImGui::EndTabItem();
            }
            
            // MIDI Control Setup Tab - for MIDI configuration
            if (ImGui::BeginTabItem("MIDI Control Setup")) {
                renderMidiSetupTab();
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void OutputPanel::update(float deltaTime) {
    // Simulate output level fluctuation
    static float time = 0.0f;
    time += deltaTime;
    _outputLevel = 0.5f + 0.3f * sinf(time * 2.0f);
}

void OutputPanel::renderOutputControls() {
    if (!ImGui::GetCurrentContext()) return; // Safety check
    
    try {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f)); // Cyan
        ImGui::Text("[OUT] Main Output");
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 200);
        
        if (ImGui::Checkbox("Waveform", &_showWaveform)) {
            // Checkbox changed - no special handling needed for now
        }
        ImGui::SameLine();
        if (ImGui::Checkbox("Monitor", &_showMonitoring)) {
            // Checkbox changed - no special handling needed for now
        }
    } catch (...) {
        // Fallback rendering if there's any issue
        ImGui::Text("[OUT] Output Controls (Error)");
    }
}

void OutputPanel::renderVideoOutput() {
    // Get available content area
    ImVec2 contentStart = ImGui::GetCursorPos();
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    contentSize.y -= 60; // Leave space for overlays
    
    // Create an invisible button to define the video area
    ImGui::InvisibleButton("VideoArea", contentSize);
    
    // Draw video placeholder using draw list
    ImVec2 videoStart = ImGui::GetItemRectMin();
    ImVec2 videoEnd = ImGui::GetItemRectMax();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Background
    drawList->AddRectFilled(videoStart, videoEnd, IM_COL32(20, 20, 20, 255));
    
    // Video placeholder content
    ImVec2 center = ImVec2(videoStart.x + contentSize.x * 0.5f, 
                          videoStart.y + contentSize.y * 0.5f);
    
    // Draw crosshairs
    drawList->AddLine(ImVec2(center.x - 50, center.y), 
                     ImVec2(center.x + 50, center.y),
                     IM_COL32(0, 200, 255, 128), 2.0f);
    drawList->AddLine(ImVec2(center.x, center.y - 50), 
                     ImVec2(center.x, center.y + 50),
                     IM_COL32(0, 200, 255, 128), 2.0f);
    
    // Text overlay
    ImVec2 textPos = ImVec2(center.x - 100, center.y + 60);
    drawList->AddText(textPos, IM_COL32(200, 200, 200, 255), 
                     "Video Output Ready");
    
    ImVec2 textPos2 = ImVec2(center.x - 120, center.y + 80);
    drawList->AddText(textPos2, IM_COL32(150, 150, 150, 255), 
                     "Awaiting video input...");
}

void OutputPanel::renderWaveformOverlay() {
    ImGui::Text("[WAV] Audio Waveform");
    ImGui::SameLine();
    
    // Simple waveform visualization with proper ImGui item
    ImVec2 waveSize = ImVec2(ImGui::GetContentRegionAvail().x - 100, 30);
    
    // Create an invisible button for the waveform area
    ImGui::InvisibleButton("WaveformArea", waveSize);
    
    ImVec2 waveStart = ImGui::GetItemRectMin();
    ImVec2 waveEnd = ImGui::GetItemRectMax();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Waveform background
    drawList->AddRectFilled(waveStart, waveEnd, IM_COL32(15, 15, 15, 255));
    
    // Draw simple waveform
    for (int i = 0; i < static_cast<int>(waveSize.x); i += 2) {
        float phase = i * 0.1f;
        float amplitude = _outputLevel * sinf(phase) * 10.0f;
        float y = waveStart.y + waveSize.y * 0.5f + amplitude;
        
        drawList->AddLine(ImVec2(waveStart.x + i, waveStart.y + waveSize.y * 0.5f),
                         ImVec2(waveStart.x + i, y),
                         IM_COL32(0, 255, 100, 180), 1.0f);
    }
    
    ImGui::NewLine();
}

void OutputPanel::renderMonitoringInfo() {
    if (!ImGui::GetCurrentContext()) {
        return; // Safety check for ImGui context
    }
    
    try {
        ImGui::Text("[INFO] Output Level: %.1f%%", _outputLevel * 100.0f);
        ImGui::SameLine();
        ImGui::Text("| FPS: 60 | Res: 3440x1440 | Format: RGB24");
    } catch (...) {
        // Fallback if there's any rendering issue
        ImGui::Text("[INFO] Monitoring data unavailable");
    }
}

void OutputPanel::renderOutputTab() {
    // Move existing output functionality into this tab
    
    // Panel header with controls
    renderOutputControls();
    ImGui::Separator();
    
    // Main video output area
    renderVideoOutput();
    
    // Overlays
    if (_showWaveform) {
        renderWaveformOverlay();
    }
    
    if (_showMonitoring) {
        renderMonitoringInfo();
    }
}

void OutputPanel::renderMidiSetupTab() {
    // MIDI Control Setup tab content
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f)); // Cyan
    ImGui::Text("MIDI Control Setup");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    // Placeholder content for MIDI setup
    ImGui::Text("MIDI Controller Configuration");
    ImGui::Spacing();
    
    // Device selection section
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Device Selection:");
    ImGui::Indent();
    
    static int selectedDevice = 0;
    const char* devices[] = { "No devices detected", "Pioneer DDJ-REV1", "Generic MIDI Controller" };
    ImGui::Combo("MIDI Device", &selectedDevice, devices, IM_ARRAYSIZE(devices));
    
    ImGui::SameLine();
    if (ImGui::Button("Refresh")) {
        // TODO: Refresh MIDI device list
    }
    
    ImGui::Unindent();
    ImGui::Spacing();
    
    // Control mapping section
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Control Mapping:");
    ImGui::Indent();
    
    ImGui::Text("Jog Wheels: Video Scrubbing");
    ImGui::Text("Crossfader: Video Mix Control");
    ImGui::Text("Play/Pause: Transport Control");
    ImGui::Text("Effects Knobs: Video Effects Parameters");
    
    ImGui::Unindent();
    ImGui::Spacing();
    
    // Status section
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Status:");
    ImGui::Indent();
    
    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "● Disconnected");
    ImGui::Text("Ready to configure MIDI mapping");
    
    ImGui::Unindent();
    
    // Configuration buttons
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    if (ImGui::Button("Start MIDI Mapping")) {
        // TODO: Begin MIDI mapping process
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Load Preset")) {
        // TODO: Load MIDI preset
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Save Preset")) {
        // TODO: Save MIDI preset
    }
}

} // namespace ui
} // namespace gamma