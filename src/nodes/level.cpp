#include "nodes/level.hpp"

#include "nodes/character.hpp"
#include "util/utils.hpp"

#include <godot_cpp/variant/callable.hpp>

namespace godot
{
    Level::Level()
    {
        this->set_name("Level Manager");

        std::array signal_connections = {
            SignalConnection(Character::Signals::PositionChanged, on_character_position_changed),
        };

        for (auto&& conn : signal_connections)
        {
            auto&& [signal, slot] = conn;
            m_player->connect(signal, slot);
        }
    }

    Level::~Level()
    {
        rl::log::trace(FUNCTION_STR);
        if (!this->is_queued_for_deletion())
            this->queue_free();
    }

    void Level::_ready()
    {
        rl::log::trace(FUNCTION_STR);
        this->add_child(m_player);
    }

    void Level::_input(const Ref<InputEvent>& event)
    {
    }

    void Level::on_character_position_changed(const Object* node, Vector2 location) const
    {
        gdutils::print(node->get_class() + " new location: " + location);
    }
}
