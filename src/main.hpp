#pragma once

#include "nodes/level.hpp"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/memory.hpp>

namespace godot
{
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
