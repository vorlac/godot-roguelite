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
        ~Main() = default;

        void _ready() override;
        void initialize();
        //void notification(int notification_type);

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
