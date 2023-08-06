#pragma once

#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot
{
    class Camera : public Camera2D
    {
        GDCLASS(Camera, Camera2D);

    public:
        Camera()
        {
            this->set_name("PlayerCamera");
        }

        void _ready() override
        {
            this->set_anchor_mode(Camera2D::ANCHOR_MODE_DRAG_CENTER);
            this->set_margin_drawing_enabled(true);
        }

        void _enter_tree() override
        {
        }

        void _exit_tree() override
        {
            this->queue_free();
        }

        void _draw() override
        {
            this->get_drag_margin(Side::SIDE_TOP);
            this->get_drag_margin(Side::SIDE_BOTTOM);
            this->get_drag_margin(Side::SIDE_LEFT);
            this->get_drag_margin(Side::SIDE_RIGHT);
        }

        static void _bind_methods()
        {
        }
    };
}
