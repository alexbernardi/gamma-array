#include "ui/MidiControlPanel.h"
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

MidiControlPanel::MidiControlPanel()
    : WorkspacePanel("MIDI Control")
    , _application(nullptr)
    , _selectedDevice(0)
    , _isConnected(false)
    , _jogWheelLeftRotation(0.0f)
    , _jogWheelRightRotation(0.0f) {
}

void MidiControlPanel::setApplication(gamma::core::Application* app) {
    _application = app;
    
    // Note: Jog wheel callback is now handled by MainContainer's MIDI tab
    // to avoid conflicts between the two panels
}

void MidiControlPanel::render() {
    if (!_visible) return;

    // Safety check for ImGui context
    if (!ImGui::GetCurrentContext()) return;

    // Get layout dimensions from workspace manager
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    if (!viewport) return; // Safety check
    
    float navBarHeight = _workspaceManager ? _workspaceManager->getNavBarHeight() : 0.0f;
    float timelineHeight = _workspaceManager ? _workspaceManager->getTimelineHeight() : 120.0f;
    float sidebarWidth = _workspaceManager ? _workspaceManager->getSidebarWidth() : 300.0f;
    
    // Calculate panel dimensions - position it as a side panel or overlay
    // For now, let's make it a floating window that can be positioned by the user
    ImVec2 panelSize = ImVec2(800, 600); // Fixed size for MIDI control panel
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    
    if (ImGui::Begin("MIDI Control Setup - DDJ-REV1", &_visible, flags)) {
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
    ImGui::End();
}

void MidiControlPanel::update(float deltaTime) {
    // No specific updates needed for MIDI panel currently
}

void MidiControlPanel::renderMidiDeviceSelection() {
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

void MidiControlPanel::renderMidiControlMapping() {
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Jog Wheel Visualization:");
    
    // Create columns for side-by-side wheels
    ImGui::Columns(2, "JogWheels", false);
    
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
    
    // Reserve space for the left wheel
    ImGui::Dummy(ImVec2(160, 160));
    ImGui::Text("%.1f°", _jogWheelLeftRotation);
    
    // Move to right column
    ImGui::NextColumn();
    
    // Right jog wheel (Channel 2)
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
    
    // Reserve space for the right wheel
    ImGui::Dummy(ImVec2(160, 160));
    ImGui::Text("%.1f°", _jogWheelRightRotation);
    
    // End columns
    ImGui::Columns(1);
}

void MidiControlPanel::renderMidiStatus() {
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Status:");
    
    if (_isConnected) {
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "● Connected");
        ImGui::Text("Ready for MIDI input");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "● Disconnected");
        ImGui::Text("Select and connect a device");
    }
}

void MidiControlPanel::renderMidiSignalLog() {
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
                    
                    // Create message string for display
                    std::string fullMessage = "[" + std::string(timeStr) + "] " + msg.description;
                    
                    // Simple colored text that can be selected normally
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::TextUnformatted(fullMessage.c_str());
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

void MidiControlPanel::renderMidiConfigButtons() {
    // Simplified controls - just clear log for now
    if (ImGui::Button("Clear Log", ImVec2(-1, 0))) {
        if (_application && _application->getMidiManager()) {
            _application->getMidiManager()->clearMessageHistory();
        }
    }
}

void MidiControlPanel::updateJogWheelRotation(int channel, float deltaRotation) {
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