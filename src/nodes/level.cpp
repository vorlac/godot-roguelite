#include "nodes/level.hpp"

#include "nodes/character.hpp"
#include "util/bind.hpp"
#include "util/debug.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    Level::Level()
    {
        godot::ResourceLoader* resource_loader{ resource::loader::get() };
        godot::Ref<godot::Resource> background_texture{ resource_loader->load(
            path::sprite::Background) };

        m_background->set_name("BackgroundTexture");
        m_background->set_texture(background_texture);
        m_background->set_global_position({ 0, 0 });
        m_background->set_centered(true);
        m_background->set_z_index(-100);

        this->set_name("Level");
        this->activate(true);
    }

    Level::~Level()
    {
        for (const auto& conn : m_signal_connections)
        {
            const auto& [signal_owner, sig, slot] = conn;
            if (signal_owner->has_signal(sig))
                signal_owner->disconnect(sig, slot);
        }
    }

    void Level::_ready()
    {
        this->add_child(m_player);
        this->add_child(m_projectile_spawner);
        this->add_child(m_background);

        m_signal_connections = {
            SignalConnection(m_player, signal::name::position_changed, this,
                             on_character_position_changed),
            SignalConnection(m_player, signal::name::spawn_projectile, this,
                             on_character_spawn_projectile),
        };

        for (const auto& conn : m_signal_connections)
        {
            auto& [signal_owner, sig, slot] = conn;
            signal_owner->connect(sig, slot);
        }
    }

    void Level::_process(double delta_time)
    {
        if (engine::editor_active())
            return;

        if (this->active() && input::cursor_visible()) [[unlikely]]
            input::hide_cursor();
        if (!this->active() && !input::cursor_visible()) [[unlikely]]
            input::show_cursor();

        this->queue_redraw();
    }

    void Level::_draw()
    {
        if constexpr (diag::is_enabled(diag::LevelProcess))
        {
            const godot::Rect2 level_bounds{ this->m_background->get_rect() };
            this->draw_rect(level_bounds, { "RED" }, false, 5.0);
        }

        if (this->active()) [[unlikely]]
        {
            godot::Point2 mouse_pos{ this->get_global_mouse_position() };
            this->draw_circle(mouse_pos, 10, { "DARK_CYAN" });
        }
    }

    [[signal_callback]]
    void Level::on_character_spawn_projectile(godot::Node* obj)
    {
        godot::Node2D* node{ rl::as<godot::Node2D>(obj) };
        debug::runtime_assert(node != nullptr);

        rl::Projectile* proj{ m_projectile_spawner->spawn_projectile() };
        proj->set_position(node->get_global_position());
        proj->set_rotation(node->get_rotation() - godot::Math::deg_to_rad(45.0));

        rl::Character* character{ rl::as<rl::Character>(node) };
        if (character != nullptr)
            proj->set_velocity(godot::Vector2{ 0, -1 }.rotated(character->get_global_rotation()));

        this->add_child(proj);
    }

    [[signal_callback]]
    void Level::on_character_position_changed(const godot::Object* const node,
                                              godot::Vector2 location) const
    {
        debug::runtime_assert(node != nullptr);
        log::info(node->get_class() + " location: " + location);
    }

    void Level::_bind_methods()
    {
        add_method_binding(Level, on_character_position_changed);
        add_method_binding(Level, on_character_spawn_projectile);
    }
}
