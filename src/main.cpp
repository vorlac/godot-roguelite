#include "main.hpp"

#include "nodes/character.hpp"
#include "nodes/level.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace rl
{
    void Main::init()
    {
        // engine settings
        rl::engine::set_fps(60);
        rl::input::use_accumulated_inputs(false);
        // TODO: set resolution

        // Main node init
        this->set_name("Main");
    }

    void Main::teardown()
    {
        if (!this->is_queued_for_deletion())
            this->queue_free();
    }

    void Main::_ready()
    {
        rl::log::info(FUNCTION_STR);
        this->add_child(m_level);
    }

    void Main::_enter_tree()
    {
        rl::log::info(FUNCTION_STR);
    }

    void Main::_exit_tree()
    {
    }
}
