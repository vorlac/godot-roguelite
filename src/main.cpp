#include "main.hpp"

#include "nodes/character.hpp"
#include "nodes/level.hpp"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot
{
    Main::Main()
    {
        this->set_name("Root");
    }

    void Main::_bind_methods()
    {
    }

    void Main::_ready()
    {
        this->add_child(m_level);
        rl::engine::set_fps(60);
    }

    void Main::_enter_tree()
    {
    }

    void Main::_exit_tree()
    {
    }

}
