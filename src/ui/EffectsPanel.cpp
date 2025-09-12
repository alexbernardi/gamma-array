#include "ui/EffectsPanel.h"
#include "imgui.h"
#include <cmath>

namespace gamma {
namespace ui {

EffectsPanel::EffectsPanel()
    : WorkspacePanel("Effects")
    , _selectedEffect(-1)
    , _selectedCategory("All")
    , _bypassAll(false)
    , _masterMix(1.0f)
    , _cpuUsage(0.0f) {
    
    // Initialize categories
    _categories = {"All", "Color", "Distortion", "Blur", "Geometry", "Time"};
    
    // Initialize available effects
    _availableEffects = {
        {"Color Correction", "Color", false, false, {
            {"Brightness", 0.0f, -1.0f, 1.0f, 0.0f, true},
            {"Contrast", 1.0f, 0.0f, 3.0f, 1.0f, true},
            {"Saturation", 1.0f, 0.0f, 2.0f, 1.0f, true},
            {"Hue Shift", 0.0f, -180.0f, 180.0f, 0.0f, true}
        }},
        {"Chromatic Aberration", "Color", false, false, {
            {"Strength", 0.0f, 0.0f, 1.0f, 0.0f, true},
            {"Red Offset", 0.0f, -50.0f, 50.0f, 0.0f, true},
            {"Blue Offset", 0.0f, -50.0f, 50.0f, 0.0f, true}
        }},
        {"Datamosh", "Distortion", false, false, {
            {"Intensity", 0.0f, 0.0f, 1.0f, 0.0f, true},
            {"Block Size", 8.0f, 1.0f, 32.0f, 8.0f, true},
            {"Chaos", 0.5f, 0.0f, 1.0f, 0.5f, true}
        }},
        {"Motion Blur", "Blur", false, false, {
            {"Amount", 0.0f, 0.0f, 1.0f, 0.0f, true},
            {"Angle", 0.0f, 0.0f, 360.0f, 0.0f, true},
            {"Samples", 8.0f, 1.0f, 32.0f, 8.0f, true}
        }},
        {"Mirror", "Geometry", false, false, {
            {"Mode", 0.0f, 0.0f, 3.0f, 0.0f, true}, // 0=none, 1=horizontal, 2=vertical, 3=both
            {"Center X", 0.5f, 0.0f, 1.0f, 0.5f, true},
            {"Center Y", 0.5f, 0.0f, 1.0f, 0.5f, true}
        }},
        {"Time Echo", "Time", false, false, {
            {"Delay", 0.1f, 0.01f, 1.0f, 0.1f, true},
            {"Feedback", 0.5f, 0.0f, 0.95f, 0.5f, true},
            {"Mix", 0.5f, 0.0f, 1.0f, 0.5f, true}
        }}
    };
    
    // Start with a few effects active
    _activeEffects.push_back(_availableEffects[0]); // Color Correction
    _activeEffects.back().isActive = true;
}

void EffectsPanel::render() {
    if (!_visible) return;

    // Calculate panel dimensions (right sidebar)
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float navBarHeight = 32.0f;
    float sidebarWidth = 300.0f;
    
    ImVec2 panelPos = ImVec2(viewport->Size.x - sidebarWidth, navBarHeight);
    ImVec2 panelSize = ImVec2(sidebarWidth, viewport->Size.y - navBarHeight);
    
    ImGui::SetNextWindowPos(panelPos);
    ImGui::SetNextWindowSize(panelSize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_NoTitleBar;
    
    if (ImGui::Begin("Effects", &_visible, flags)) {
        renderEffectControls();
        ImGui::Separator();
        
        // Split panel into sections
        if (ImGui::BeginTabBar("EffectsTabs")) {
            if (ImGui::BeginTabItem("Chain")) {
                renderEffectChain();
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Library")) {
                renderEffectLibrary();
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Parameters")) {
                renderParameterControls();
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void EffectsPanel::update(float deltaTime) {
    // Simulate CPU usage based on active effects
    float targetCPU = _activeEffects.size() * 0.15f;
    if (_bypassAll) targetCPU = 0.0f;
    
    _cpuUsage += (targetCPU - _cpuUsage) * deltaTime * 5.0f;
    _cpuUsage = std::max(0.0f, std::min(1.0f, _cpuUsage));
}

void EffectsPanel::renderEffectControls() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f)); // Cyan
    ImGui::Text("⚡ VJ Effects");
    ImGui::PopStyleColor();
    
    // Master controls
    ImGui::Checkbox("Bypass All", &_bypassAll);
    
    ImGui::Text("Master Mix:");
    ImGui::SliderFloat("##MasterMix", &_masterMix, 0.0f, 1.0f, "%.2f");
    
    // CPU usage indicator
    ImGui::Text("CPU: %.1f%%", _cpuUsage * 100.0f);
    ImGui::ProgressBar(_cpuUsage, ImVec2(-1, 0));
}

void EffectsPanel::renderEffectChain() {
    ImGui::Text("Effect Chain (%d active)", static_cast<int>(_activeEffects.size()));
    
    if (ImGui::BeginChild("EffectChain", ImVec2(0, -60))) {
        for (int i = 0; i < static_cast<int>(_activeEffects.size()); ++i) {
            auto& effect = _activeEffects[i];
            
            ImGui::PushID(i);
            
            // Effect header with controls
            bool isSelected = (_selectedEffect == i);
            if (ImGui::Selectable(("##effect" + std::to_string(i)).c_str(), isSelected, 0, ImVec2(0, 30))) {
                _selectedEffect = i;
            }
            
            // Draw effect info on the selectable area
            ImVec2 pos = ImGui::GetItemRectMin();
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            
            // Effect name
            ImU32 textColor = effect.isActive ? IM_COL32(255, 255, 255, 255) : IM_COL32(128, 128, 128, 255);
            if (effect.isBypassed) textColor = IM_COL32(255, 128, 0, 255); // Orange for bypassed
            
            drawList->AddText(ImVec2(pos.x + 5, pos.y + 5), textColor, effect.name.c_str());
            drawList->AddText(ImVec2(pos.x + 5, pos.y + 18), IM_COL32(150, 150, 150, 255), effect.category.c_str());
            
            // Control buttons (right side)
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 80, ImGui::GetCursorPosY() - 25));
            
            if (ImGui::SmallButton(effect.isActive ? "ON" : "OFF")) {
                effect.isActive = !effect.isActive;
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("BYP")) {
                effect.isBypassed = !effect.isBypassed;
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("X")) {
                _activeEffects.erase(_activeEffects.begin() + i);
                if (_selectedEffect >= i) _selectedEffect--;
                ImGui::PopID();
                break;
            }
            
            ImGui::PopID();
        }
    }
    ImGui::EndChild();
    
    // Drop target for new effects
    ImGui::Separator();
    ImGui::Text("Drop effects here or use Library tab");
    
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EFFECT")) {
            int effectIndex = *static_cast<const int*>(payload->Data);
            if (effectIndex >= 0 && effectIndex < static_cast<int>(_availableEffects.size())) {
                _activeEffects.push_back(_availableEffects[effectIndex]);
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void EffectsPanel::renderEffectLibrary() {
    ImGui::Text("Effect Library");
    
    // Category filter
    ImGui::SetNextItemWidth(-1);
    if (ImGui::BeginCombo("##Category", _selectedCategory.c_str())) {
        for (const auto& category : _categories) {
            bool isSelected = (_selectedCategory == category);
            if (ImGui::Selectable(category.c_str(), isSelected)) {
                _selectedCategory = category;
            }
        }
        ImGui::EndCombo();
    }
    
    ImGui::Separator();
    
    // Available effects
    if (ImGui::BeginChild("AvailableEffects")) {
        for (int i = 0; i < static_cast<int>(_availableEffects.size()); ++i) {
            const auto& effect = _availableEffects[i];
            
            // Filter by category
            if (_selectedCategory != "All" && effect.category != _selectedCategory) {
                continue;
            }
            
            ImGui::PushID(i);
            
            if (ImGui::Button(("+ " + effect.name).c_str(), ImVec2(-1, 0))) {
                _activeEffects.push_back(effect);
                _activeEffects.back().isActive = true;
            }
            
            // Drag source
            if (ImGui::BeginDragDropSource()) {
                ImGui::SetDragDropPayload("EFFECT", &i, sizeof(int));
                ImGui::Text("Adding: %s", effect.name.c_str());
                ImGui::EndDragDropSource();
            }
            
            // Show effect info on hover
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Effect: %s", effect.name.c_str());
                ImGui::Text("Category: %s", effect.category.c_str());
                ImGui::Text("Parameters: %d", static_cast<int>(effect.parameters.size()));
                ImGui::EndTooltip();
            }
            
            ImGui::PopID();
        }
    }
    ImGui::EndChild();
}

void EffectsPanel::renderParameterControls() {
    if (_selectedEffect >= 0 && _selectedEffect < static_cast<int>(_activeEffects.size())) {
        auto& effect = _activeEffects[_selectedEffect];
        
        ImGui::Text("Parameters: %s", effect.name.c_str());
        ImGui::Separator();
        
        if (ImGui::BeginChild("Parameters")) {
            for (auto& param : effect.parameters) {
                ImGui::PushID(&param);
                
                if (param.isEnabled) {
                    ImGui::Text("%s:", param.name.c_str());
                    
                    // Different control types based on parameter range
                    if (param.maxValue - param.minValue <= 5.0f && param.minValue >= 0.0f) {
                        // Integer slider for small ranges
                        int intValue = static_cast<int>(param.value);
                        if (ImGui::SliderInt("##param", &intValue, 
                                           static_cast<int>(param.minValue), 
                                           static_cast<int>(param.maxValue))) {
                            param.value = static_cast<float>(intValue);
                        }
                    } else {
                        // Float slider
                        ImGui::SliderFloat("##param", &param.value, param.minValue, param.maxValue);
                    }
                    
                    // Reset button
                    ImGui::SameLine();
                    if (ImGui::SmallButton("R")) {
                        param.value = param.defaultValue;
                    }
                } else {
                    ImGui::TextDisabled("%s: (disabled)", param.name.c_str());
                }
                
                ImGui::PopID();
            }
        }
        ImGui::EndChild();
        
    } else {
        ImGui::Text("No effect selected");
        ImGui::Text("Select an effect from the Chain tab to edit parameters");
    }
}

} // namespace ui
} // namespace gamma