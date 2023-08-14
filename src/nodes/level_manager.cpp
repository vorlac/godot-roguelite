#include "nodes/level_manager.hpp"

#include "nodes/character.hpp"
#include "util/assets.hpp"
#include "util/debug.hpp"
#include "util/io.hpp"
#include "util/signals.hpp"
#include "util/utils.hpp"

#include <godot_cpp/variant/callable.hpp>

namespace rl
{
    LevelManager::LevelManager()
    {
        godot::ResourceLoader* resource_loader{ resource::loader::get() };
        godot::Ref<godot::Resource> background_texture{ resource_loader->load(
            rl::asset::path::BackgroundImage) };

        m_background->set_name("BackgroundTexture");
        m_background->set_texture(background_texture);
        m_background->set_global_position({ 0, 0 });
        m_background->set_centered(true);
        m_background->set_z_index(-100);

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
        this->add_child(m_background);

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
        debug::assert(node != nullptr);
        log::info(node->get_class() + " location: " + location);
    }
}
