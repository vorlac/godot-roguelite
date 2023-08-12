#include "main.hpp"

#include "nodes/character.hpp"
#include "nodes/level_manager.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl
{
    Main::Main()
    {
        this->set_name("Main");
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
    }

    void Main::_enter_tree()
    {
    }

    void Main::_exit_tree()
    {
    }

    void Main::apply_default_settings()
    {
        engine::set_fps(60);

        // engine::scene_tree()->get_edited_scene_root()->get_viewport()->set_handle_input_locally(true);
        // engine::root_node()->get_viewport()->set_process_input(false);

        if (editor::active())
            engine::root_window()->set_size({ 1920, 1080 });
        else
            engine::root_window()->set_size({ 1024, 768 });

        input::use_accumulated_inputs(false);
    }
}
