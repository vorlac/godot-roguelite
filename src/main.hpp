#pragma once

#include "nodes/level.hpp"
#include "util/signals.hpp"
#include "util/utils.hpp"

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot
{
    class Input;
    class InputMap;
    class Vector2;
    class EditorNode;

    class Main : public Node2D
    {
        GDCLASS(Main, Node2D);

        Main();

    public:
        void _ready() override;
        void _enter_tree() override;
        void _exit_tree() override;

        static void _bind_methods();

    private:
        Level* m_level{ memnew(Level) };
    };
}
