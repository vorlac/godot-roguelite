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
        Camera();

        void _ready() override;
        void _enter_tree() override;
        void _exit_tree() override;
        void _draw() override;

    protected:
        static void _bind_methods();
    };
}
