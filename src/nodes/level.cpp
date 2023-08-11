#include "nodes/level.hpp"

#include "nodes/character.hpp"
#include "util/io.hpp"
#include "util/signals.hpp"
#include "util/utils.hpp"

#include <godot_cpp/variant/callable.hpp>

namespace rl
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
        if (!this->is_queued_for_deletion())
            this->queue_free();
    }

    void Level::_ready()
    {
        rl::log::trace(FUNCTION_STR);
        this->add_child(m_player);
    }

    void Level::_input(const godot::Ref<godot::InputEvent>& event)
    {
    }

    void Level::on_character_position_changed(const godot::Object* node,
                                              godot::Vector2 location) const
    {
        rl::log::info(node->get_class() + " new location: " + location);
    }
}
