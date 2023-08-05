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
        }

        void _ready() override
        {
        }

        void _enter_tree() override
        {
            this->set_name("PlayerCamera");
            this->set_anchor_mode(Camera2D::ANCHOR_MODE_DRAG_CENTER);
            this->set_editor_description("PlayerCamera");
            this->set_margin_drawing_enabled(true);
            this->set_enabled(true);
        }

        void _draw() override
        {
            this->get_drag_margin(Side::SIDE_TOP);
            this->get_drag_margin(Side::SIDE_BOTTOM);
            this->get_drag_margin(Side::SIDE_LEFT);
            this->get_drag_margin(Side::SIDE_RIGHT);
        }

        void _exit_tree() override
        {
        }

        static void _bind_methods()
        {
        }
    };
}
