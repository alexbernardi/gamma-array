#include "ui/OutputPanel.h"
#include "ui/WorkspaceManager.h"
#include "core/Application.h"
#include "midi/MidiManager.h"
#include "imgui.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace gamma {
namespace ui {

OutputPanel::OutputPanel()
    : WorkspacePanel("Output")
    , _application(nullptr)
    , _showWaveform(true)
    , _showMonitoring(true)
    , _outputLevel(0.75f)
    , _selectedDevice(0)
    , _isConnected(false)
    , _jogWheelLeftRotation(0.0f)
    , _jogWheelRightRotation(0.0f) {
}

void OutputPanel::setApplication(gamma::core::Application* app) {
    _application = app;
    
    // Set up jog wheel callback with MIDI manager
    if (_application && _application->getMidiManager()) {
        auto callback = [this](int channel, float deltaRotation) {
            this->updateJogWheelRotation(channel, deltaRotation);
        };
        _application->getMidiManager()->setJogWheelCallback(callback);
    }
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
    ImGui::Text("MIDI Control Setup - DDJ-REV1");
    ImGui::PopStyleColor();
    
    ImGui::Separator();
    
    // Get MIDI manager if available
    gamma::midi::MidiManager* midiManager = nullptr;
    if (_application) {
        midiManager = _application->getMidiManager();
    }
    
    // Create child regions for organized layout
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    float leftPanelWidth = availableSize.x * 0.4f;
    float rightPanelWidth = availableSize.x * 0.6f - 10.0f; // 10px spacing
    
    // Left panel - Device and control info
    if (ImGui::BeginChild("MidiLeftPanel", ImVec2(leftPanelWidth, 0), true)) {
        renderMidiDeviceSelection();
        ImGui::Spacing();
        renderMidiControlMapping();
        ImGui::Spacing();
        renderMidiStatus();
        ImGui::Spacing();
        renderMidiConfigButtons();
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // Right panel - Signal log
    if (ImGui::BeginChild("MidiRightPanel", ImVec2(rightPanelWidth, 0), true)) {
        renderMidiSignalLog();
    }
    ImGui::EndChild();
}

void OutputPanel::renderMidiDeviceSelection() {
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Device Selection:");
    
    // Get available devices from MIDI manager
    gamma::midi::MidiManager* midiManager = nullptr;
    if (_application) {
        midiManager = _application->getMidiManager();
    }
    
    std::vector<std::string> deviceNames;
    if (midiManager) {
        deviceNames = midiManager->getAvailableDevices();
    }
    
    if (deviceNames.empty()) {
        deviceNames.push_back("No devices detected");
    }
    
    // Convert to const char* array for ImGui combo
    std::vector<const char*> deviceCStrs;
    for (const auto& name : deviceNames) {
        deviceCStrs.push_back(name.c_str());
    }
    
    if (ImGui::Combo("MIDI Device", &_selectedDevice, deviceCStrs.data(), static_cast<int>(deviceCStrs.size()))) {
        // Device selection changed
        _isConnected = false;
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Refresh")) {
        if (midiManager) {
            midiManager->refreshDevices();
        }
    }
    
    ImGui::SameLine();
    if (ImGui::Button(_isConnected ? "Disconnect" : "Connect")) {
        if (midiManager && !deviceNames.empty() && _selectedDevice < static_cast<int>(deviceNames.size())) {
            if (!_isConnected) {
                std::string deviceName = deviceNames[_selectedDevice];
                if (deviceName != "No devices detected") {
                    _isConnected = midiManager->connectToDevice(deviceName);
                }
            } else {
                midiManager->disconnect();
                _isConnected = false;
            }
        }
    }
}

void OutputPanel::renderMidiControlMapping() {
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Jog Wheel Visualization:");
    
    // Left jog wheel (Channel 1)
    ImGui::Text("Left Wheel (Ch1):");
    ImVec2 leftCenter = ImGui::GetCursorScreenPos();
    leftCenter.x += 80;
    leftCenter.y += 80;
    float radius = 60.0f;
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Draw left wheel
    drawList->AddCircle(leftCenter, radius, IM_COL32(100, 100, 100, 255), 32, 3.0f);
    drawList->AddCircleFilled(leftCenter, radius - 10, IM_COL32(30, 30, 30, 255), 32);
    
    // Left wheel rotation indicator
    float leftRotationRad = _jogWheelLeftRotation * (M_PI / 180.0f);
    ImVec2 leftIndicatorPos = ImVec2(
        leftCenter.x + cos(leftRotationRad - M_PI/2) * (radius - 20),
        leftCenter.y + sin(leftRotationRad - M_PI/2) * (radius - 20)
    );
    
    drawList->AddLine(leftCenter, leftIndicatorPos, IM_COL32(0, 200, 255, 255), 3.0f);
    drawList->AddCircleFilled(leftIndicatorPos, 4.0f, IM_COL32(0, 255, 200, 255), 12);
    
    // Right jog wheel (Channel 2) - positioned to the right
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 180); // Move to right side
    ImGui::Text("Right Wheel (Ch2):");
    
    ImVec2 rightCenter = ImGui::GetCursorScreenPos();
    rightCenter.x += 80;
    rightCenter.y += 80;
    
    // Draw right wheel
    drawList->AddCircle(rightCenter, radius, IM_COL32(100, 100, 100, 255), 32, 3.0f);
    drawList->AddCircleFilled(rightCenter, radius - 10, IM_COL32(30, 30, 30, 255), 32);
    
    // Right wheel rotation indicator
    float rightRotationRad = _jogWheelRightRotation * (M_PI / 180.0f);
    ImVec2 rightIndicatorPos = ImVec2(
        rightCenter.x + cos(rightRotationRad - M_PI/2) * (radius - 20),
        rightCenter.y + sin(rightRotationRad - M_PI/2) * (radius - 20)
    );
    
    drawList->AddLine(rightCenter, rightIndicatorPos, IM_COL32(255, 100, 0, 255), 3.0f);
    drawList->AddCircleFilled(rightIndicatorPos, 4.0f, IM_COL32(255, 150, 0, 255), 12);
    
    // Reserve space for both wheels
    ImGui::Dummy(ImVec2(380, 160));
    
    // Display rotation values
    ImGui::Text("Left: %.1f°    Right: %.1f°", _jogWheelLeftRotation, _jogWheelRightRotation);
}

void OutputPanel::renderMidiStatus() {
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Status:");
    
    if (_isConnected) {
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "● Connected");
        ImGui::Text("Ready for MIDI input");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "● Disconnected");
        ImGui::Text("Select and connect a device");
    }
}

void OutputPanel::renderMidiSignalLog() {
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "MIDI Signal Log:");
    
    // Create scrolling region for MIDI messages
    if (ImGui::BeginChild("MidiLog", ImVec2(0, -30), false, ImGuiWindowFlags_HorizontalScrollbar)) {
        gamma::midi::MidiManager* midiManager = nullptr;
        if (_application) {
            midiManager = _application->getMidiManager();
        }
        
        if (midiManager) {
            auto recentMessages = midiManager->getRecentMessages();
            
            if (recentMessages.empty()) {
                ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No MIDI messages received yet...");
                ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Connect a device and move some controls!");
            } else {
                // Display messages in reverse order (newest first)
                for (auto it = recentMessages.rbegin(); it != recentMessages.rend(); ++it) {
                    const auto& msg = *it;
                    
                    // Color code by message type
                    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default white
                    if (!msg.data.empty()) {
                        unsigned char msgType = msg.data[0] & 0xF0; // Extract message type
                        
                        if (msgType == 0x90 || msgType == 0x80) { // Note on/off
                            color = ImVec4(0.3f, 1.0f, 0.3f, 1.0f); // Green for notes
                        } else if (msgType == 0xB0) { // Control change
                            color = ImVec4(0.3f, 0.8f, 1.0f, 1.0f); // Cyan for CCs
                        } else if (msgType == 0xE0) { // Pitch bend
                            color = ImVec4(1.0f, 0.8f, 0.3f, 1.0f); // Orange for pitch bend
                        }
                    }
                    
                    // Format timestamp as string
                    char timeStr[32];
                    snprintf(timeStr, sizeof(timeStr), "%.3f", msg.timestamp);
                    
                    // Create selectable text for copy/paste functionality
                    char fullMessage[256];
                    snprintf(fullMessage, sizeof(fullMessage), "[%s] %s", timeStr, msg.description.c_str());
                    
                    // Use Selectable with colored text
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::Selectable(fullMessage, false);
                    ImGui::PopStyleColor();
                }
                
                // Auto-scroll to bottom for new messages
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                    ImGui::SetScrollHereY(1.0f);
                }
            }
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "MIDI system not available");
        }
    }
    ImGui::EndChild();
}

void OutputPanel::renderMidiConfigButtons() {
    // Simplified controls - just clear log for now
    if (ImGui::Button("Clear Log", ImVec2(-1, 0))) {
        if (_application && _application->getMidiManager()) {
            _application->getMidiManager()->clearMessageHistory();
        }
    }
}

void OutputPanel::updateJogWheelRotation(int channel, float deltaRotation) {
    if (channel == 1) {
        _jogWheelLeftRotation += deltaRotation;
        
        // Keep rotation in 0-360 degree range
        while (_jogWheelLeftRotation >= 360.0f) {
            _jogWheelLeftRotation -= 360.0f;
        }
        while (_jogWheelLeftRotation < 0.0f) {
            _jogWheelLeftRotation += 360.0f;
        }
    } else if (channel == 2) {
        _jogWheelRightRotation += deltaRotation;
        
        // Keep rotation in 0-360 degree range
        while (_jogWheelRightRotation >= 360.0f) {
            _jogWheelRightRotation -= 360.0f;
        }
        while (_jogWheelRightRotation < 0.0f) {
            _jogWheelRightRotation += 360.0f;
        }
    }
}

} // namespace ui
} // namespace gamma