#include <godot_cpp/classes/camera2d.hpp>

#include "entity/camera.hpp"

namespace rl
{
    Camera::Camera()
    {
        this->set_name("PlayerCamera");
        this->set_margin_drawing_enabled(true);
        auto anchor{ AnchorMode::ANCHOR_MODE_DRAG_CENTER };
        this->set_anchor_mode(anchor);
    }
}
