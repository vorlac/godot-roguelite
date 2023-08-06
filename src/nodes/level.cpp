#include "nodes/level.hpp"

#include "nodes/character.hpp"

#include <godot_cpp/variant/callable.hpp>

namespace godot
{
    Level::Level()
    {
        this->set_name("Level Manager");
    }

    void Level::_ready()
    {
        this->add_child(m_player);
        m_player->connect(Character::Signals::PositionChanged,
                          Callable(this, "on_character_position_changed"));
    }

    void Level::_enter_tree()
    {
    }

    void Level::_exit_tree()
    {
        this->queue_free();
    }

    void Level::_input(const Ref<InputEvent>& event)
    {
    }

    void Level::_bind_methods()
    {
        ClassDB::bind_method(D_METHOD("on_character_position_changed"),
                             &Level::on_character_position_changed);
    }

    void Level::bind_signals()
    {
    }

    void Level::bind_properties()
    {
    }

    void Level::on_character_position_changed(const Object* node, Vector2 location) const
    {
        gdutils::print(node->get_class() + " new location: " + location);
    }
}
