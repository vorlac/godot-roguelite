#include "nodes/camera.hpp"

#include <godot_cpp/classes/camera2d.hpp>

namespace rl::inline node
{
    Camera::Camera()
    {
        this->set_name("PlayerCamera");
        this->set_margin_drawing_enabled(true);
        auto anchor{ AnchorMode::ANCHOR_MODE_DRAG_CENTER };
        this->set_anchor_mode(anchor);
    }
}
