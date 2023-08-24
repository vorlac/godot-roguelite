#include "core/level.hpp"

#include "nodes/character.hpp"
#include "util/scene.hpp"
#include "util/utils.hpp"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/rect2.hpp>

namespace rl
{
    namespace gd = ::godot;

    Level::Level()
    {
        gd::ResourceLoader* resource_loader{ resource::loader::get() };
        gd::Ref<gd::Resource> background_texture{ resource_loader->load(path::sprite::Background) };

        m_background->set_name("BackgroundTexture");
        m_background->set_texture(background_texture);
        m_background->set_global_position({ 0, 0 });
        m_background->set_centered(true);
        m_background->set_z_index(-100);

        this->set_name("Level");
    }

    Level::~Level()
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

    void Level::_ready()
    {
        this->add_child(m_player);
        this->add_child(m_background);
        this->add_child(m_projectile_spawner);

        if (m_signal_connections.empty())
        {
            m_signal_connections = {
                SignalConnection(signal::name::position_changed, on_position_changed),
                SignalConnection(signal::name::shoot_projectile, on_shoot_projectile),
            };

            for (const auto& conn : m_signal_connections)
            {
                const auto& [signal, slot] = conn;
                m_player->connect(signal, slot);
            }
        }
    }

    void Level::_draw()
    {
        if constexpr (diag::is_enabled(diag::LevelProcess))
        {
            const godot::Rect2 level_bounds{ this->m_background->get_rect() };
            this->draw_rect(level_bounds, { "RED" }, false, 5.0);
        }
    }
}

namespace rl
{
    [[signal_callback]]
    void Level::on_shoot_projectile(godot::Object* const obj)
    {
        godot::Node2D* const node{ rl::as<godot::Node2D>(obj) };
        debug::assert(node != nullptr);

        rl::Projectile* const proj{ m_projectile_spawner->spawn_projectile() };
        proj->set_position(node->get_global_position());
        proj->set_rotation(node->get_rotation() - math::deg_to_rad(45.0));

        rl::Character* const character{ rl::as<rl::Character>(node) };
        if (character != nullptr)
            proj->set_velocity(godot::Vector2{ 0, -1 }.rotated(character->get_global_rotation()));

        this->add_child(proj);
    }

    [[signal_callback]]
    void Level::on_position_changed(const godot::Object* const node, godot::Vector2 location) const
    {
        debug::assert(node != nullptr);
        log::info(node->get_class() + " location: " + location);
    }
}
