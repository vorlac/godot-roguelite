#include "main.hpp"

#include "nodes/character.hpp"
#include "util/debug.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/scene.hpp"

namespace rl
{
    Main::Main()
    {
        this->set_name("Main");

        resource::preload::scene<rl::MainDialog> dialog{ path::ui::MainDialog };
        m_main_dialog = dialog.instantiate();
        debug::runtime_assert(m_main_dialog != nullptr);
    }

    Main::~Main()
    {
    }

    void Main::apply_default_settings()
    {
        engine::set_fps(60);
        input::use_accumulated_inputs(false);

        if (not engine::editor_active())
            engine::root_window()->set_size({ 1920, 1080 });
    }

    void Main::_ready()
    {
        this->apply_default_settings();

        debug::runtime_assert(m_main_dialog != nullptr);
        if (m_main_dialog != nullptr)
        {
            m_canvas_layer = rl::as<godot::CanvasLayer>(
                m_main_dialog->find_child("MainCanvasLayer", true, false));

            debug::runtime_assert(m_canvas_layer != nullptr);
            debug::runtime_assert(m_active_level != nullptr);

            if (m_active_level != nullptr && m_canvas_layer != nullptr)
                m_canvas_layer->add_child(m_active_level);

            if (m_main_dialog != nullptr)
                this->add_child(m_main_dialog);
        }
    }
}
