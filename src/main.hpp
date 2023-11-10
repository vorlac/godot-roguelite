#pragma once

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/node.hpp>

#include "entity/level.hpp"
#include "ui/main_dialog.hpp"

namespace rl
{
    class Main : public godot::Node
    {
        GDCLASS(Main, godot::Node);

    public:
        Main();
        ~Main() = default;

        void _ready() override;

    protected:
        void apply_default_settings();

        static void _bind_methods()
        {
        }

    private:
        godot::CanvasLayer* m_canvas_layer{ nullptr };
        MainDialog* m_main_dialog{ nullptr };
        Level* m_active_level{ nullptr };
    };
}
