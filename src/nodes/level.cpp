#include "nodes/level.hpp"

#include "nodes/character.hpp"
#include "singletons/console.hpp"
#include "util/bind.hpp"
#include "util/conversions.hpp"
#include "util/debug.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/marker2d.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl::inline node
{
    Level::Level()
        : m_player{ player_scene.instantiate() }
    {
        // TODO: kill magic string
        scene::node::set_unique_name(this, "Level1");
        this->activate(true);
    }

    void Level::_ready()
    {
        this->add_child(m_player);
        this->add_child(m_projectile_spawner);

        signal<event::position_changed>::connect<PlayerController>(m_player->get_controller()) <=>
            slot(this, on_character_position_changed);

        signal<event::spawn_projectile>::connect<Character>(m_player) <=>
            slot(this, on_character_spawn_projectile);
    }

    void Level::_process(double delta_time)
    {
        if (engine::editor_active())
            return;

        if (this->active() && input::cursor_visible()) [[likely]]
            input::hide_cursor();
        else if (!this->active() && !input::cursor_visible()) [[unlikely]]
            input::show_cursor();

        this->queue_redraw();
    }

    void Level::_draw()
    {
        if (this->active()) [[likely]]
        {
            godot::Point2 mouse_pos{ this->get_global_mouse_position() };
            this->draw_circle(mouse_pos, 5, { "DARK_CYAN" });
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
        Projectile* projectile{ m_projectile_spawner->spawn_projectile() };
        if (projectile != nullptr)
        {
            projectile->set_position(node->get_global_position());
            projectile->set_rotation(node->get_global_rotation() - godot::Math::deg_to_rad(45.0));

            godot::Marker2D* firing_pt{ gdcast<godot::Marker2D>(node) };
            if (firing_pt != nullptr)
            {
                projectile->set_velocity(
                    godot::Vector2(0, -1).rotated(firing_pt->get_global_rotation()));
            }

            this->add_child(projectile);
        }
    }

    [[signal_slot]]
    void Level::on_character_position_changed(const godot::Object* const node,
                                              godot::Vector2 location) const
    {
        runtime_assert(node != nullptr);
        auto console{ Console<godot::RichTextLabel>::get() };
        console->print("{} ({},{})\n", io::green(to<std::string>(node->get_class()) + " location: "),
                       io::orange(location.x), io::orange(location.y));
    }

    void Level::_bind_methods()
    {
        bind_member_function(Level, on_character_position_changed);
        bind_member_function(Level, on_character_spawn_projectile);
    }
}
