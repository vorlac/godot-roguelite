#include "nodes/level.hpp"

#include "nodes/character.hpp"
#include "util/io.hpp"
#include "util/utils.hpp"

#include <godot_cpp/variant/callable.hpp>

namespace rl
{
    Level::Level()
    {
        this->set_name("LevelManager");
    }

    Level::~Level()
    {
        if (!this->is_queued_for_deletion())
            this->queue_free();

        for (const auto& conn : m_signal_connections)
        {
            const auto& [signal, slot] = conn;
            m_player->disconnect(signal, godot::Callable(slot));
        }
    }

    void Level::_ready()
    {
        rl::log::info(FUNCTION_STR);
        this->add_child(m_player);
        if (m_signal_connections.empty())
        {
            m_signal_connections = {
                std::pair{
                    Character::Signals::PositionChanged,
                    godot::Callable(this, "on_character_position_changed"),
                },
            };

            for (const auto& conn : m_signal_connections)
            {
                const auto& [signal, slot] = conn;
                m_player->connect(signal, slot);
            }
        }
    }

    void Level::_input(const godot::Ref<godot::InputEvent>& event)
    {
    }

    void Level::on_character_position_changed(const godot::Object* node,
                                              godot::Vector2 location) const
    {
        log::info(node->get_class() + " new location: " + location);
    }
}
