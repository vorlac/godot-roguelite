#pragma once

#include "nodes/level.hpp"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/core/memory.hpp>

namespace rl
{
    class Main : public godot::Node2D
    {
        GDCLASS(Main, godot::Node2D);

    public:
        Main() = default;
        ~Main() = default;

        void _ready() override;
        void _enter_tree() override;
        void _exit_tree() override;

        void _notification(int notification_type)
        {
            switch (notification_type)
            {
                case NOTIFICATION_POSTINITIALIZE:
                    this->init();
                    break;
                case NOTIFICATION_PREDELETE:
                    this->teardown();
                    break;
            }
        }

    protected:
        static void _bind_methods()
        {
        }

    private:
        void init();
        void teardown();

    private:
        rl::Level* m_level{ memnew(rl::Level) };
    };
}
