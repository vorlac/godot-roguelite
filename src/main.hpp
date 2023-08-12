#pragma once

#include "nodes/level_manager.hpp"

#include <godot_cpp/classes/node2d.hpp>
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
        void _enter_tree() override;
        void _exit_tree() override;

    protected:
        void apply_default_settings();

        static void _bind_methods()
        {
        }

    private:
        void init();
        void teardown();

    private:
        LevelManager* m_level{ memnew(LevelManager) };
    };
}
