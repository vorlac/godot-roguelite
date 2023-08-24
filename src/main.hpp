#pragma once

#include "core/level_loader.hpp"
#include "ui/main_interface_diag.hpp"

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/memory.hpp>

namespace rl
{
    class Main : public godot::Node2D
    {
        GDCLASS(Main, godot::Node2D);

    public:
        Main();
        ~Main();

        void _ready() override;
        void _process(double delta_time) override;
        void _draw() override;

        void _notification(int notification);

    protected:
        void apply_default_settings();

        static void _bind_methods()
        {
        }

    private:
        void init();
        void teardown();

    private:
        rl::LevelLoader* m_level{ memnew(rl::LevelLoader) };
        rl::MainInterfaceDiag* m_debug_overlay{ memnew(rl::MainInterfaceDiag) };
    };
}
