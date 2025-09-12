#include "ui/WorkspaceManager.h"
#include "imgui.h"

namespace gamma {
namespace ui {

WorkspaceManager::WorkspaceManager()
    : _isFullscreen(false)
    , _layoutDirty(true)
    , _navBarHeight(32.0f)
    , _timelineHeight(120.0f)
    , _sidebarWidth(300.0f) {
}

WorkspaceManager::~WorkspaceManager() {
    shutdown();
}

void WorkspaceManager::initialize() {
    // Create panel instances
    _timelinePanel = std::make_unique<TimelinePanel>();
    _outputPanel = std::make_unique<OutputPanel>();
    _importPanel = std::make_unique<ImportPanel>();
    _effectsPanel = std::make_unique<EffectsPanel>();
    
    // Set workspace manager reference on all panels
    _timelinePanel->setWorkspaceManager(this);
    _outputPanel->setWorkspaceManager(this);
    _importPanel->setWorkspaceManager(this);
    _effectsPanel->setWorkspaceManager(this);
    
    // Initialize panels
    _timelinePanel->setVisible(true);
    _outputPanel->setVisible(true);
    _importPanel->setVisible(true);
    _effectsPanel->setVisible(true);
    
    calculateLayout();
}

void WorkspaceManager::render() {
    if (_layoutDirty) {
        calculateLayout();
        _layoutDirty = false;
    }
    
    // Render all panels
    if (_importPanel) _importPanel->render();
    if (_outputPanel) _outputPanel->render();
    if (_effectsPanel) _effectsPanel->render();
    if (_timelinePanel) _timelinePanel->render();
    
    // Render workspace status overlay
    renderWorkspaceOverlay();
}

void WorkspaceManager::update(float deltaTime) {
    // Update all panels
    if (_timelinePanel) _timelinePanel->update(deltaTime);
    if (_outputPanel) _outputPanel->update(deltaTime);
    if (_importPanel) _importPanel->update(deltaTime);
    if (_effectsPanel) _effectsPanel->update(deltaTime);
}

void WorkspaceManager::shutdown() {
    _timelinePanel.reset();
    _outputPanel.reset();
    _importPanel.reset();
    _effectsPanel.reset();
}

void WorkspaceManager::setFullscreen(bool fullscreen) {
    if (_isFullscreen != fullscreen) {
        _isFullscreen = fullscreen;
        _layoutDirty = true;
    }
}

void WorkspaceManager::togglePanelVisibility(const std::string& panelName) {
    if (panelName == "Timeline" && _timelinePanel) {
        _timelinePanel->setVisible(!_timelinePanel->isVisible());
    } else if (panelName == "Output" && _outputPanel) {
        _outputPanel->setVisible(!_outputPanel->isVisible());
    } else if (panelName == "Import" && _importPanel) {
        _importPanel->setVisible(!_importPanel->isVisible());
    } else if (panelName == "Effects" && _effectsPanel) {
        _effectsPanel->setVisible(!_effectsPanel->isVisible());
    }
    
    _layoutDirty = true;
}

void WorkspaceManager::resetLayout() {
    _navBarHeight = 32.0f;
    _timelineHeight = 120.0f;
    _sidebarWidth = 300.0f;
    
    if (_timelinePanel) _timelinePanel->setVisible(true);
    if (_outputPanel) _outputPanel->setVisible(true);
    if (_importPanel) _importPanel->setVisible(true);
    if (_effectsPanel) _effectsPanel->setVisible(true);
    
    _layoutDirty = true;
}

void WorkspaceManager::calculateLayout() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    
    if (!viewport) return;
    
    // Adjust dimensions based on screen size and visibility
    float screenWidth = viewport->Size.x;
    float screenHeight = viewport->Size.y;
    
    // Scale sidebar width based on screen size
    _sidebarWidth = std::max(250.0f, std::min(400.0f, screenWidth * 0.15f));
    
    // Scale timeline height based on screen size
    _timelineHeight = std::max(100.0f, std::min(150.0f, screenHeight * 0.12f));
    
    // Adjust for fullscreen mode
    if (_isFullscreen) {
        _navBarHeight = 32.0f; // Show navigation bar in fullscreen
    } else {
        _navBarHeight = 0.0f; // Hide navigation bar in windowed mode
    }
    
    updatePanelSizes();
}

void WorkspaceManager::updatePanelSizes() {
    // Panel sizes are calculated dynamically in each panel's render method
    // This method can be used for any global size adjustments or constraints
    
    // Ensure minimum sizes
    _sidebarWidth = std::max(200.0f, _sidebarWidth);
    _timelineHeight = std::max(80.0f, _timelineHeight);
    
    // Ensure panels fit on screen
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    if (viewport) {
        _sidebarWidth = std::min(_sidebarWidth, viewport->Size.x * 0.3f);
        _timelineHeight = std::min(_timelineHeight, viewport->Size.y * 0.3f);
    }
}

void WorkspaceManager::renderWorkspaceOverlay() {
    // Render a small status overlay in the corner
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    
    ImVec2 overlayPos = ImVec2(viewport->Size.x - 200, _navBarHeight + 10);
    ImVec2 overlaySize = ImVec2(180, 80);
    
    ImGui::SetNextWindowPos(overlayPos);
    ImGui::SetNextWindowSize(overlaySize);
    
    ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoCollapse |
                                   ImGuiWindowFlags_AlwaysAutoResize |
                                   ImGuiWindowFlags_NoBackground |
                                   ImGuiWindowFlags_NoFocusOnAppearing |
                                   ImGuiWindowFlags_NoNav;
    
    if (ImGui::Begin("WorkspaceOverlay", nullptr, overlayFlags)) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 0.8f));
        
        ImGui::Text("Workspace Status");
        ImGui::Separator();
        
        // Panel visibility status
        ImGui::Text("Timeline: %s", _timelinePanel && _timelinePanel->isVisible() ? "ON" : "OFF");
        ImGui::Text("Output: %s", _outputPanel && _outputPanel->isVisible() ? "ON" : "OFF");
        ImGui::Text("Import: %s", _importPanel && _importPanel->isVisible() ? "ON" : "OFF");
        ImGui::Text("Effects: %s", _effectsPanel && _effectsPanel->isVisible() ? "ON" : "OFF");
        
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

} // namespace ui
} // namespace gamma