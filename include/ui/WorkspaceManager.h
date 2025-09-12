#pragma once

#include "WorkspacePanel.h"
#include "TimelinePanel.h"
#include "OutputPanel.h"
#include "ImportPanel.h"
#include "EffectsPanel.h"
#include <memory>

namespace gamma {
namespace ui {

class WorkspaceManager {
public:
    WorkspaceManager();
    ~WorkspaceManager();
    
    void initialize();
    void render();
    void update(float deltaTime);
    void shutdown();
    
    // Panel access
    TimelinePanel& getTimelinePanel() { return *_timelinePanel; }
    OutputPanel& getOutputPanel() { return *_outputPanel; }
    ImportPanel& getImportPanel() { return *_importPanel; }
    EffectsPanel& getEffectsPanel() { return *_effectsPanel; }
    
    // Layout management
    void setFullscreen(bool fullscreen);
    void togglePanelVisibility(const std::string& panelName);
    void resetLayout();
    
private:
    void calculateLayout();
    void updatePanelSizes();
    void renderWorkspaceOverlay();
    
    // Panel instances
    std::unique_ptr<TimelinePanel> _timelinePanel;
    std::unique_ptr<OutputPanel> _outputPanel;
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