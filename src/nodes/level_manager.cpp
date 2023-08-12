#include "nodes/level_manager.hpp"

#include "nodes/character.hpp"
#include "util/io.hpp"
#include "util/signals.hpp"
#include "util/utils.hpp"

#include <godot_cpp/variant/callable.hpp>

namespace rl
{
    LevelManager::LevelManager()
    {
        this->set_name("LevelManager");
    }

    LevelManager::~LevelManager()
    {
        if (!this->is_queued_for_deletion())
            this->queue_free();

        for (const auto& conn : m_signal_connections)
        {
            const auto& [signal, slot] = conn;
            if (m_player->has_signal(signal))
                m_player->disconnect(signal, slot);
        }
    }

    void LevelManager::_ready()
    {
        this->add_child(m_player);
        if (m_signal_connections.empty())
        {
            m_signal_connections = {
                std::pair{
                    SignalConnection(Character::Signals::PositionChanged, on_position_changed),
                },
            };

            for (const auto& conn : m_signal_connections)
            {
                const auto& [signal, slot] = conn;
                m_player->connect(signal, slot);
            }
        }
    }

    void LevelManager::_input(const godot::Ref<godot::InputEvent>& event)
    {
    }

    void LevelManager::on_position_changed(const godot::Object* node, godot::Vector2 location) const
    {
        rl::assert(node != nullptr);
        log::info(node->get_class() + " location: " + location);
    }
}
