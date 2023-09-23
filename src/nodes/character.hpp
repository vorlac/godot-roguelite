#pragma once

#include "nodes/camera.hpp"
#include "nodes/player_controller.hpp"
#include "util/attributes.hpp"
#include "util/scene.hpp"

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

namespace rl::inline node
{
    class Character : public godot::CharacterBody2D
    {
        GDCLASS(Character, godot::CharacterBody2D);

    public:
        Character();
        ~Character() = default;

        void _ready() override;

        PlayerController* get_controller() const;

    protected:
        [[property]] double get_movement_speed() const;
        [[property]] double get_movement_friction() const;
        [[property]] double get_rotation_speed() const;
        [[property]] void set_movement_speed(const double move_speed);
        [[property]] void set_movement_friction(const double move_friction);
        [[property]] void set_rotation_speed(const double rotation_speed);

        [[signal_slot]] void on_player_shoot();
        [[signal_slot]] void on_player_rotate(double rotation_angle, double delta_time);
        [[signal_slot]] void on_player_movement(godot::Vector2 movement_velocity, double delta_time);

    protected:
        static void _bind_methods();
        static void bind_properties();

    protected:
        // Rate of acceleration/deceleration (unit/s/s)
        double m_movement_friction{ 5.0 };
        // Rate of rotational acceleration/deceleration (unit/s/s)
        double m_rotation_speed{ 5.0 };
        // target movement speed (units/s)
        double m_movement_speed{ 500.0 };
        // target facing angle (radians)
        double m_rotation_angle{ 0.0 };

        // the player character camera
        Camera* m_camera{ memnew(Camera) };
        // handles all input related player controls
        PlayerController* m_player_controller{ memnew(PlayerController) };
    };
}
