#pragma once

#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/wrapped.hpp>

namespace rl
{
    class Camera : public godot::Camera2D
    {
        GDCLASS(Camera, godot::Camera2D);

    public:
        Camera();
        ~Camera();

        void _ready() override;
        void _draw() override;

    protected:
        static void _bind_methods()
        {
        }
    };
}
