#include "nodes/camera.hpp"

namespace godot
{
    Camera::Camera()
    {
        this->set_name("PlayerCamera");
    }

    void Camera::_ready()
    {
        auto anchor_mode{ ANCHOR_MODE_DRAG_CENTER };
        this->set_anchor_mode(anchor_mode);
        this->set_margin_drawing_enabled(true);
    }

    void Camera::_enter_tree()
    {
    }

    void Camera::_exit_tree()
    {
        // this->queue_free();
    }

    void Camera::_draw()
    {
        /*
        this->get_drag_margin(Side::SIDE_TOP);
        this->get_drag_margin(Side::SIDE_BOTTOM);
        this->get_drag_margin(Side::SIDE_LEFT);
        this->get_drag_margin(Side::SIDE_RIGHT);
        */
    }

    void Camera::_bind_methods()
    {
    }
}
