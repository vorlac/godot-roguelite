#include "nodes/level.hpp"

#include "nodes/character.hpp"
#include "util/bind.hpp"
#include "util/conversions.hpp"
#include "util/debug.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl::inline node
{
    Level::Level()
    {
        // TODO: kill magic string
        scene::node::set_unique_name(this, "Level1");
        this->activate(true);
    }

    void Level::_ready()
    {
        resource::preload::scene<Character> player_scene{ path::scene::Player };
        m_player = player_scene.instantiate();
        m_projectile_spawner = memnew(rl::ProjectileSpawner);

        this->add_child(m_player);
        this->add_child(m_projectile_spawner);

        signal<event::position_changed>::connect<Character>(m_player) <=> [this]() {
            return slot(this, on_character_position_changed);
        }();

        signal<event::spawn_projectile>::connect<Character>(m_player) <=> [this]() {
            return slot(this, on_character_spawn_projectile);
        }();
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
        if (this->active()) [[likely]]
        {
            godot::Point2 mouse_pos{ this->get_global_mouse_position() };
            this->draw_circle(mouse_pos, 10, { "DARK_CYAN" });
        }
    }

    void Level::activate(bool active)
    {
        m_active = active;
    }

    bool Level::active() const
    {
        return m_active;
    }

    [[signal_slot]]
    void Level::on_character_spawn_projectile(godot::Node* obj)
    {
        godot::Node2D* node{ gdcast<godot::Node2D>(obj) };
        rl::Projectile* proj{ m_projectile_spawner->spawn_projectile() };

        proj->set_position(node->get_global_position());
        proj->set_rotation(node->get_rotation() - godot::Math::deg_to_rad(45.0));

        rl::Character* character{ gdcast<rl::Character>(node) };
        if (character != nullptr)
            proj->set_velocity(godot::Vector2{ 0, -1 }.rotated(character->get_global_rotation()));

        this->add_child(proj);
    }

    [[signal_slot]]
    void Level::on_character_position_changed(const godot::Object* const node,
                                              godot::Vector2 location) const
    {
        runtime_assert(node != nullptr);
        log::info(node->get_class() + " location: " + location);
    }

    void Level::_bind_methods()
    {
        bind_member_function(Level, on_character_position_changed);
        bind_member_function(Level, on_character_spawn_projectile);
    }
}
