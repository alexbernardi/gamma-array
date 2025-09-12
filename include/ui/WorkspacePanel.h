#pragma once

namespace gamma {
namespace ui {

/**
 * @brief Base class for workspace panels in the VJing interface
 * 
 * Provides a common interface for the main workspace panels:
 * Timeline, Output, Import, and Effects. Each panel is modular
 * and handles its own rendering and event processing.
 */
class WorkspacePanel {
public:
    WorkspacePanel(const char* name);
    virtual ~WorkspacePanel() = default;

    /**
     * @brief Render the panel's UI content
     * Called every frame during the main render loop
     */
    virtual void render() = 0;

    /**
     * @brief Update panel state
     * @param deltaTime Time elapsed since last frame in seconds
     */
    virtual void update(float deltaTime);

    /**
     * @brief Get the panel's display name
     */
    const char* getName() const { return _name; }

    /**
     * @brief Check if panel is currently visible
     */
    bool isVisible() const { return _visible; }

    /**
     * @brief Set panel visibility
     */
    void setVisible(bool visible) { _visible = visible; }

protected:
    const char* _name;
    bool _visible;
};

} // namespace ui
} // namespace gamma