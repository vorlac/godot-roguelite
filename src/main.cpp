#include "main.hpp"

#include "nodes/character.hpp"
#include "util/utils.hpp"

#include <cstdint>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
#include <strstream>
#include <type_traits>

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/circle_shape2d.hpp>
#include <godot_cpp/classes/engine_debugger.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/shape_cast2d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl
{
    Main::Main()
    {
        this->set_name("Main");

        resource::preload::scene<rl::MainDialog> dialog{ path::ui::MainDialog };
        m_main_dialog = dialog.instantiate();
        debug::assert(m_main_dialog != nullptr);
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

        debug::assert(m_main_dialog != nullptr);
        if (m_main_dialog != nullptr)
        {
            m_canvas_layer = rl::as<godot::CanvasLayer>(
                m_main_dialog->find_child("MainCanvasLayer", true, false));

            debug::assert(m_canvas_layer != nullptr);
            debug::assert(m_active_level != nullptr);

            if (m_active_level != nullptr && m_canvas_layer != nullptr)
                m_canvas_layer->add_child(m_active_level);

            if (m_main_dialog != nullptr)
                this->add_child(m_main_dialog);
        }
    }
}
