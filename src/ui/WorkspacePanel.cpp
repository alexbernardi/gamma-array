#include "ui/WorkspacePanel.h"

namespace gamma {
namespace ui {

WorkspacePanel::WorkspacePanel(const char* name)
    : _name(name)
    , _visible(true) {
}

void WorkspacePanel::update(float deltaTime) {
    // Default implementation - panels can override if needed
    (void)deltaTime; // Suppress unused parameter warning
}

} // namespace ui
} // namespace gamma