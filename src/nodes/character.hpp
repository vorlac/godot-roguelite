#pragma once

#include "nodes/camera.hpp"
#include "nodes/player_controller.hpp"
#include "util/bind.hpp"

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

// TODO: move allocations to cpp file so
//       these can just be forward declared
// namespace rl {
//     class Camera;
// }
// namespace godot {
//    class CollisionShape2D;
//    class Sprite2D;
//}

namespace rl
{
    class Character : public godot::CharacterBody2D
    {
        GDCLASS(Character, godot::CharacterBody2D);

    public:
        Character();
        ~Character();

        void _ready() override;
        void _process(double delta_time) override;

        [[node_property]] double get_movement_speed() const;
        [[node_property]] double get_movement_friction() const;
        [[node_property]] double get_rotation_speed() const;
        [[node_property]] void set_movement_speed(const double move_speed);
        [[node_property]] void set_movement_friction(const double move_friction);
        [[node_property]] void set_rotation_speed(const double rotation_speed);

        [[signal_callback]] void on_player_shoot();
        [[signal_callback]] void on_player_rotate(double rotation_angle, double delta_time);
        [[signal_callback]] void on_player_move(godot::Vector2 movement_velocity,
                                                double delta_time);

    protected:
        static void _bind_methods();
        static void bind_signals();
        static void bind_properties();

    protected:
        // the player character camera
        rl::Camera* m_camera{ memnew(rl::Camera) };
        // the player character's texture
        godot::Sprite2D* m_sprite{ memnew(godot::Sprite2D) };
        // the player character's collision area geometry
        godot::CollisionShape2D* m_collision_shape{ memnew(godot::CollisionShape2D) };
        // handles all input related player controls
        rl::PlayerController* m_player_controller{ memnew(rl::PlayerController) };

    protected:
        std::vector<signal::connection> m_signal_connections{};
        // Rate of acceleration/deceleration (unit/s/s)
        double m_movement_friction{ 5.0 };
        // Rate of rotational acceleration/deceleration (unit/s/s)
        double m_rotation_speed{ 5.0 };
        // target movement speed (units/s)
        double m_movement_speed{ 500.0 };
        // target facing angle (radians)
        double m_rotation_angle{ 0.0 };
    };
}
