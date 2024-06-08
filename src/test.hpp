#pragma once

#include <godot_cpp/classes/node.hpp>

#include "util/engine.hpp"

namespace rl
{
    class Test : public godot::Node
    {
        GDCLASS(Test, godot::Node);

    public:
        Test() = default;

        void _physics_process(double delta) override
        {
            if (engine::editor_active())
                return;

            m_signal_timer += delta;
            if (m_signal_timer > 1.0)
            {
                this->emit_signal(event::signal_example, delta);
                m_signal_timer -= 1.0;
            }
        }

    protected:
        static void _bind_methods()
        {
            signal_binding<Test, event::signal_example>::add<double>();
        }

    private:
        double m_signal_timer{ 0.0 };
    };
}
