#include "nodes/game.hpp"

#include "nodes/character.hpp"
#include "util/utils.hpp"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/node_path.hpp>

namespace godot
{
    void Game::_bind_methods()
    {
        ClassDB::bind_method(D_METHOD("print_character_position"), &Game::print_character_position);
    }

    void Game::_ready()
    {
        Object* node = this->get_node<Object>("Level/Character");
        if (node != nullptr)
        {
            node->connect(Character::Signals::PositionChanged,
                          Callable(this, "print_character_position"));
        }
    }

    void Game::print_character_position(const Object* node, Vector2 location) const
    {
        rl::utils::print(node->get_class() + " new location: " + location);
    }

    void Game::_enter_tree()
    {
    }

    void Game::_exit_tree()
    {
    }

    void Game::_input(const Ref<InputEvent>& event)
    {
    }

    void Game::bind_signals()
    {
    }

    void Game::bind_properties()
    {
    }
}
