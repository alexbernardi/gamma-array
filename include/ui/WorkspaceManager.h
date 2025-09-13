#pragma once

#include "WorkspacePanel.h"
#include "TimelinePanel.h"
#include "MainContainer.h"
#include "MidiControlPanel.h"
#include "ImportPanel.h"
#include "EffectsPanel.h"
#include <memory>

namespace gamma {
namespace core { class Application; }
}

namespace gamma {
namespace ui {

class WorkspaceManager {
public:
    WorkspaceManager();
    ~WorkspaceManager();
    
    void initialize(gamma::core::Application* application = nullptr);
    void render();
    void update(float deltaTime);
    void shutdown();
    
    // Panel access
    TimelinePanel& getTimelinePanel() { return *_timelinePanel; }
    MainContainer& getMainContainer() { return *_mainContainer; }
    MidiControlPanel& getMidiControlPanel() { return *_midiControlPanel; }
    ImportPanel& getImportPanel() { return *_importPanel; }
    EffectsPanel& getEffectsPanel() { return *_effectsPanel; }
    
    // Layout management
    void setFullscreen(bool fullscreen);
    void togglePanelVisibility(const std::string& panelName);
    void resetLayout();
    
    // Layout dimensions for panels
    float getNavBarHeight() const { return _navBarHeight; }
    float getTimelineHeight() const { return _timelineHeight; }
    float getSidebarWidth() const { return _sidebarWidth; }
    
private:
    void calculateLayout();
    void updatePanelSizes();
    void renderWorkspaceOverlay();
    
    // Panel instances
    std::unique_ptr<TimelinePanel> _timelinePanel;
    std::unique_ptr<MainContainer> _mainContainer;
    std::unique_ptr<MidiControlPanel> _midiControlPanel;
    std::unique_ptr<ImportPanel> _importPanel;
    std::unique_ptr<EffectsPanel> _effectsPanel;
    
    // Layout state
    bool _isFullscreen;
    bool _layoutDirty;
    
    // Panel dimensions
    float _navBarHeight;
    float _timelineHeight;
    float _sidebarWidth;
};

} // namespace ui
} // namespace gamma