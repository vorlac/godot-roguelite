#include "nodes/character.hpp"

#include "nodes/camera.hpp"
#include "nodes/player_controller.hpp"
#include "util/assert.hpp"
#include "util/bind.hpp"
#include "util/constants.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"
#include "util/io.hpp"
#include "util/scene.hpp"

#include <array>
#include <tuple>
#include <vector>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/marker2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    Character::Character()
    {
        scene::node::set_unique_name(this, name::character::player);
        this->set_motion_mode(MotionMode::MOTION_MODE_FLOATING);
    }

    void Character::_ready()
    {
        this->add_child(m_camera);
        this->add_child(m_player_controller);

        m_firing_point = gdcast<godot::Marker2D>(
            this->find_child(name::character::firing_pt, true, false));
        runtime_assert(m_firing_point != nullptr);

        signal<event::player_move>::connect<PlayerController>(m_player_controller)
            <=> slot(this, on_player_movement);

        signal<event::player_rotate>::connect<PlayerController>(m_player_controller)
            <=> slot(this, on_player_rotate);

        signal<event::player_shoot>::connect<PlayerController>(m_player_controller)
            <=> slot(this, on_player_shoot);
    }

    PlayerController* Character::get_controller() const
    {
        return m_player_controller;
    }

    [[signal_slot]]
    void Character::on_player_movement(godot::Vector2 movement_velocity, double delta_time)
    {
        double increment = m_movement_friction * delta_time;
        godot::Vector2 velocity{ this->get_velocity().lerp(movement_velocity, increment) };
        velocity = velocity.clamp({ -1.0, -1.0 }, { 1.0, 1.0 });
        this->translate(velocity * this->get_movement_speed() * delta_time);
        this->set_velocity(velocity);
        this->move_and_slide();
    }

    [[signal_slot]]
    void Character::on_player_rotate(double rotation_angle, double delta_time)
    {
        double smoothed_angle = godot::Math::lerp_angle(this->get_rotation(), rotation_angle,
                                                        m_rotation_speed * delta_time);
        this->set_rotation(smoothed_angle);
    }

    [[signal_slot]]
    void Character::on_player_shoot()
    {
        // TODO: fix this
        this->emit_signal(event::spawn_projectile, m_firing_point);
    }

    [[property]]
    double Character::get_movement_speed() const
    {
        return m_movement_speed;
    }

    [[property]]
    void Character::set_movement_speed(const double move_speed)
    {
        m_movement_speed = move_speed;
    }

    [[property]]
    double Character::get_movement_friction() const
    {
        return m_movement_friction;
    }

    [[property]]
    void Character::set_movement_friction(const double move_friction)
    {
        m_movement_friction = move_friction;
    }

    [[property]]
    double Character::get_rotation_speed() const
    {
        return m_rotation_speed;
    }

    [[property]]
    void Character::set_rotation_speed(const double rotation_speed)
    {
        m_rotation_speed = rotation_speed;
    }

    void Character::_bind_methods()
    {
        bind_member_function(Character, on_player_movement);
        bind_member_function(Character, on_player_rotate);
        bind_member_function(Character, on_player_shoot);

        bind_property(Character, movement_speed, double);
        bind_property(Character, movement_friction, double);
        bind_property(Character, rotation_speed, double);

        signal_binding<Character, event::position_changed>::add<godot::Object*, godot::Vector2>();
        signal_binding<Character, event::spawn_projectile>::add<>();
    }
}
