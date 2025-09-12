#pragma once

#include "WorkspacePanel.h"
#include <vector>
#include <string>

namespace gamma {
namespace ui {

struct MediaItem {
    std::string name;
    std::string path;
    std::string type;
    float duration;
    bool isLoaded;
};

class ImportPanel : public WorkspacePanel {
public:
    ImportPanel();
    
    void render() override;
    void update(float deltaTime) override;
    
private:
    void renderImportControls();
    void renderMediaLibrary();
    void renderFileExplorer();
    void renderMediaPreview();
    
    // Media library
    std::vector<MediaItem> _mediaItems;
    int _selectedItem;
    
    // File explorer
    std::string _currentPath;
    std::vector<std::string> _directoryContents;
    
    // Import settings
    bool _autoLoadMedia;
    bool _showPreview;
    float _previewVolume;
};

} // namespace ui
} // namespace gamma