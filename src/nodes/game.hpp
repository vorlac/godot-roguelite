#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot
{
    class Input;
    class InputMap;

    class Game : public Node2D
    {
        GDCLASS(Game, Node2D);

    public:
        void _ready() override;
        void _enter_tree() override;
        void _exit_tree() override;
        void _input(const Ref<InputEvent>& event) override;

        static void _bind_methods();
        static void bind_signals();
        static void bind_properties();
    };
}
