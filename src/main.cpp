#include "main.hpp"

#include "nodes/character.hpp"
#include "nodes/level_manager.hpp"
#include "util/debug.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/circle_shape2d.hpp>
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

        m_debug_node->set_name("Diagnostics");
        m_debug_canvas->set_name("DebugCanvasLayer");
        m_debug_canvas->set_follow_viewport(true);
        m_debug_canvas->set_follow_viewport_scale(true);
        m_debug_canvas->set_visible(true);
        // m_debug_canvas->set_layer(1234);
    }

    Main::~Main()
    {
        if (!this->is_queued_for_deletion())
            this->queue_free();
    }

    void Main::_ready()
    {
        this->apply_default_settings();

        this->add_child(m_level);
        this->add_child(m_debug_canvas);
        m_debug_canvas->add_child(m_debug_node);
    }

    void Main::_process(double delta_time)
    {
        if constexpr (diag::is_enabled(diag::RootProcess))
            this->queue_redraw();
    }

    void Main::_enter_tree()
    {
    }

    void Main::_exit_tree()
    {
    }

    void Main::_draw()
    {
        godot::Point2 mouse_pos{ this->get_global_mouse_position() };
        m_debug_node->draw_circle(mouse_pos, 20, { "DARK_CYAN" });
        m_debug_node->queue_redraw();
    }

    void Main::apply_default_settings()
    {
        engine::set_fps(60);

        // engine::scene_tree()->get_edited_scene_root()->get_viewport()->set_handle_input_locally(true);
        // engine::root_node()->get_viewport()->set_process_input(false);

        if (not editor::active())
            engine::root_window()->set_size({ 1024, 768 });

        input::use_accumulated_inputs(false);
    }
}
