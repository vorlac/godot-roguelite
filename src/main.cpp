#include "main.hpp"

#include "nodes/character.hpp"
#include "nodes/level.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot
{
    Main::Main()
    {
        this->set_name("Main");
    }

    void Main::_ready()
    {
        rl::log::info(FUNCTION_STR);
        rl::engine::set_fps(60);
    }

    void Main::_enter_tree()
    {
        rl::log::info(FUNCTION_STR);
        this->add_child(m_level);
    }

    void Main::_exit_tree()
    {
    }

    void Main::_bind_methods()
    {
    }
}
