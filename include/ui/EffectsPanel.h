#pragma once

#include "WorkspacePanel.h"
#include <vector>
#include <string>
#include <map>

namespace gamma {
namespace ui {

struct EffectParameter {
    std::string name;
    float value;
    float minValue;
    float maxValue;
    float defaultValue;
    bool isEnabled;
};

struct Effect {
    std::string name;
    std::string category;
    bool isActive;
    bool isBypassed;
    std::vector<EffectParameter> parameters;
};

class EffectsPanel : public WorkspacePanel {
public:
    EffectsPanel();
    
    void render() override;
    void update(float deltaTime) override;
    
private:
    void renderEffectControls();
    void renderEffectChain();
    void renderEffectLibrary();
    void renderParameterControls();
    
    // Effect management
    std::vector<Effect> _activeEffects;
    std::vector<Effect> _availableEffects;
    int _selectedEffect;
    
    // Categories
    std::vector<std::string> _categories;
    std::string _selectedCategory;
    
    // Performance
    bool _bypassAll;
    float _masterMix;
    float _cpuUsage;
};

} // namespace ui
} // namespace gamma