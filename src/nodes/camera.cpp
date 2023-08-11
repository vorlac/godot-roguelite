#include "nodes/camera.hpp"

namespace godot
{
    Camera::Camera()
    {
        this->set_name("PlayerCamera");
        this->set_margin_drawing_enabled(true);
        auto anchor{ AnchorMode::ANCHOR_MODE_DRAG_CENTER };
        this->set_anchor_mode(anchor);
    }

    Camera::~Camera()
    {
        if (!this->is_queued_for_deletion())
            this->queue_free();
    }

    void Camera::_ready()
    {
        //
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
}
