#pragma once

#include "nodes/level.hpp"
#include "ui/main_dialog.hpp"

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/node.hpp>

namespace rl
{
    class Main : public godot::Node
    {
        GDCLASS(Main, godot::Node);

    public:
        Main();
        ~Main();

        void _ready() override;

    protected:
        void apply_default_settings();

        static void _bind_methods()
        {
        }

    private:
        rl::Level* m_active_level{ memnew(rl::Level) };
        godot::CanvasLayer* m_canvas_layer{ nullptr };
        rl::MainDialog* m_main_dialog{ nullptr };
    };
}
