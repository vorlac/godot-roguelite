#include <godot_cpp/classes/collision_polygon2d.hpp>
#include <godot_cpp/classes/marker2d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include "entity/character/character.hpp"
#include "entity/controller/player_controller.hpp"
#include "entity/level.hpp"
#include "singletons/console.hpp"
#include "util/bind.hpp"
#include "util/conversions.hpp"
#include "util/debug.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/io.hpp"

namespace rl
{
    Level::Level()
    {
        scene::node::set_unique_name(this, name::level::level1);
        this->activate(true);
    }

    void Level::_ready()
    {
        godot::Node* box{ this->find_child(name::level::physics_box) };
        m_physics_box = gdcast<godot::RigidBody2D>(box);

        this->add_child(m_player);
        this->add_child(m_projectile_spawner);

        PlayerController* controller{ gdcast<PlayerController>(m_player->get_controller()) };
        if (controller != nullptr)
        {
            signal<event::position_changed>::connect<CharacterController>(controller) <=>
                signal_callback(this, on_character_position_changed);

            signal<event::spawn_projectile>::connect<Player>(m_player) <=>
                signal_callback(this, on_player_spawn_projectile);
        }
    }

    void Level::_process(double delta_time)
    {
        if (engine::editor_active())
            return;

        if (this->active() && input::cursor_visible()) [[unlikely]]
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
    void Level::on_physics_box_entered(godot::Node* node) const
    {
        console::get()->print("{} > {}", io::yellow("projectile"), to<std::string>(node->get_name()));
    }

    [[signal_slot]]
    void Level::on_physics_box_exited(godot::Node* node) const
    {
        console::get()->print("{} < {}", io::red("projectile"), to<std::string>(node->get_name()));
    }

    [[signal_slot]]
    void Level::on_player_spawn_projectile(godot::Node* obj)
    {
        Projectile* projectile{ m_projectile_spawner->spawn_projectile() };
        if (projectile != nullptr)
        {
            godot::Marker2D* firing_pt{ gdcast<godot::Marker2D>(obj) };
            if (firing_pt != nullptr)
            {
                projectile->set_position(firing_pt->get_global_position());
                projectile->set_rotation(firing_pt->get_global_rotation());

                signal<event::body_entered>::connect<Projectile>(projectile) <=>
                    signal_callback(this, on_physics_box_entered);

                signal<event::body_exited>::connect<Projectile>(projectile) <=>
                    signal_callback(this, on_physics_box_exited);
            }

            this->add_child(projectile);
        }
    }

    [[signal_slot]]
    void Level::on_character_position_changed(const godot::Object* const node,
                                              godot::Vector2 location) const
    {
        runtime_assert(node != nullptr);
        auto console{ console::get() };
        console->print("{} ({},{})", io::green(to<std::string>(node->get_class()) + " location: "),
                       io::orange(location.x), io::orange(location.y));
    }

    void Level::_bind_methods()
    {
        bind_member_function(Level, on_character_position_changed);
        bind_member_function(Level, on_player_spawn_projectile);
        bind_member_function(Level, on_physics_box_entered);
        bind_member_function(Level, on_physics_box_exited);
    }
}
