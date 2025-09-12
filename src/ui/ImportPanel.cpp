#include "ui/ImportPanel.h"
#include "imgui.h"
#include <filesystem>

namespace gamma {
namespace ui {

ImportPanel::ImportPanel()
    : WorkspacePanel("Import")
    , _selectedItem(-1)
    , _currentPath("C:\\")
    , _autoLoadMedia(true)
    , _showPreview(true)
    , _previewVolume(0.5f) {
    
    // Populate sample media items
    _mediaItems = {
        {"Sample_Video_01.mp4", "C:\\Videos\\Sample_Video_01.mp4", "Video", 120.5f, true},
        {"Background_Loop.avi", "C:\\Videos\\Background_Loop.avi", "Video", 30.0f, false},
        {"Beat_Track.wav", "C:\\Audio\\Beat_Track.wav", "Audio", 180.2f, true},
        {"Transition_FX.mov", "C:\\Effects\\Transition_FX.mov", "Video", 5.0f, false},
        {"Ambient_Texture.mp4", "C:\\Textures\\Ambient_Texture.mp4", "Video", 60.0f, true}
    };
    
    // Initialize directory contents
    _directoryContents = {"Videos\\", "Audio\\", "Effects\\", "Textures\\", "sample.mp4", "test.wav"};
}

void ImportPanel::render() {
    if (!_visible) return;

    // Calculate panel dimensions (left sidebar)
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float navBarHeight = 32.0f;
    float sidebarWidth = 300.0f;
    
    ImVec2 panelPos = ImVec2(0, navBarHeight);
    ImVec2 panelSize = ImVec2(sidebarWidth, viewport->Size.y - navBarHeight);
    
    ImGui::SetNextWindowPos(panelPos);
    ImGui::SetNextWindowSize(panelSize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_NoTitleBar;
    
    if (ImGui::Begin("Import", &_visible, flags)) {
        renderImportControls();
        ImGui::Separator();
        
        // Split panel into sections
        if (ImGui::BeginTabBar("ImportTabs")) {
            if (ImGui::BeginTabItem("Library")) {
                renderMediaLibrary();
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Browser")) {
                renderFileExplorer();
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Preview")) {
                renderMediaPreview();
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void ImportPanel::update(float deltaTime) {
    // Update any preview animations or loading states
}

void ImportPanel::renderImportControls() {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f)); // Cyan
    ImGui::Text("📁 Media Import");
    ImGui::PopStyleColor();
    
    if (ImGui::Button("Import Files", ImVec2(-1, 0))) {
        // TODO: Open file dialog
    }
    
    ImGui::Checkbox("Auto-load", &_autoLoadMedia);
    ImGui::SameLine();
    ImGui::Checkbox("Preview", &_showPreview);
}

void ImportPanel::renderMediaLibrary() {
    ImGui::Text("Media Library (%d items)", static_cast<int>(_mediaItems.size()));
    
    // Filter/search bar
    static char searchBuffer[256] = "";
    ImGui::SetNextItemWidth(-1);
    ImGui::InputTextWithHint("##search", "Search media...", searchBuffer, sizeof(searchBuffer));
    
    ImGui::Separator();
    
    // Media items list
    if (ImGui::BeginChild("MediaList", ImVec2(0, -60))) {
        for (int i = 0; i < static_cast<int>(_mediaItems.size()); ++i) {
            const auto& item = _mediaItems[i];
            
            bool isSelected = (_selectedItem == i);
            
            // Item background color based on load status
            if (item.isLoaded) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green for loaded
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); // Gray for unloaded
            }
            
            if (ImGui::Selectable(item.name.c_str(), isSelected)) {
                _selectedItem = i;
            }
            
            ImGui::PopStyleColor();
            
            // Show item details on hover
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Path: %s", item.path.c_str());
                ImGui::Text("Type: %s", item.type.c_str());
                ImGui::Text("Duration: %.1fs", item.duration);
                ImGui::Text("Status: %s", item.isLoaded ? "Loaded" : "Not Loaded");
                ImGui::EndTooltip();
            }
            
            // Context menu
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Load")) {
                    _mediaItems[i].isLoaded = true;
                }
                if (ImGui::MenuItem("Unload")) {
                    _mediaItems[i].isLoaded = false;
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Remove")) {
                    _mediaItems.erase(_mediaItems.begin() + i);
                    if (_selectedItem >= i) _selectedItem--;
                }
                ImGui::EndPopup();
            }
        }
    }
    ImGui::EndChild();
    
    // Status bar
    ImGui::Separator();
    int loadedCount = 0;
    for (const auto& item : _mediaItems) {
        if (item.isLoaded) loadedCount++;
    }
    ImGui::Text("Loaded: %d/%d", loadedCount, static_cast<int>(_mediaItems.size()));
}

void ImportPanel::renderFileExplorer() {
    ImGui::Text("File Browser");
    
    // Current path
    ImGui::Text("Path: %s", _currentPath.c_str());
    
    if (ImGui::Button("Up")) {
        // Navigate up one directory
        size_t lastSlash = _currentPath.find_last_of("\\/");
        if (lastSlash != std::string::npos) {
            _currentPath = _currentPath.substr(0, lastSlash + 1);
        }
    }
    
    ImGui::Separator();
    
    // Directory contents
    if (ImGui::BeginChild("FileList")) {
        for (const auto& item : _directoryContents) {
            bool isDirectory = item.back() == '\\';
            
            if (isDirectory) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow for folders
                if (ImGui::Selectable(("📁 " + item).c_str())) {
                    _currentPath += item;
                }
                ImGui::PopStyleColor();
            } else {
                if (ImGui::Selectable(("📄 " + item).c_str())) {
                    // TODO: Add to import queue or preview
                }
                
                // Drag source for files
                if (ImGui::BeginDragDropSource()) {
                    ImGui::SetDragDropPayload("FILE_PATH", item.c_str(), item.size() + 1);
                    ImGui::Text("Dragging: %s", item.c_str());
                    ImGui::EndDragDropSource();
                }
            }
        }
    }
    ImGui::EndChild();
}

void ImportPanel::renderMediaPreview() {
    if (_selectedItem >= 0 && _selectedItem < static_cast<int>(_mediaItems.size())) {
        const auto& item = _mediaItems[_selectedItem];
        
        ImGui::Text("Preview: %s", item.name.c_str());
        ImGui::Separator();
        
        // Preview placeholder
        ImVec2 previewSize = ImVec2(250, 140);
        ImVec2 previewStart = ImGui::GetCursorScreenPos();
        
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(previewStart, 
                               ImVec2(previewStart.x + previewSize.x, previewStart.y + previewSize.y),
                               IM_COL32(30, 30, 30, 255));
        
        // Preview content
        if (item.type == "Video") {
            drawList->AddText(ImVec2(previewStart.x + 10, previewStart.y + 10), 
                             IM_COL32(200, 200, 200, 255), "Video Preview");
            drawList->AddText(ImVec2(previewStart.x + 10, previewStart.y + 30), 
                             IM_COL32(150, 150, 150, 255), ("Duration: " + std::to_string(item.duration) + "s").c_str());
        } else {
            drawList->AddText(ImVec2(previewStart.x + 10, previewStart.y + 10), 
                             IM_COL32(200, 200, 200, 255), "Audio Waveform");
        }
        
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + previewSize.y + 10);
        
        // Preview controls
        if (ImGui::Button("Play")) {
            // TODO: Start preview playback
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            // TODO: Stop preview playback
        }
        
        ImGui::Text("Volume:");
        ImGui::SliderFloat("##PreviewVolume", &_previewVolume, 0.0f, 1.0f);
        
    } else {
        ImGui::Text("No media selected");
        ImGui::Text("Select an item from the Library tab to preview");
    }
}

} // namespace ui
} // namespace gamma