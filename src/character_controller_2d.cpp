#include "character_controller_2d.hpp"

#include <cstdio>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    void CharacterController2D::_bind_methods()
    {
        UtilityFunctions::print("Binding methods");

        ClassDB::bind_method(D_METHOD("get_speed"), &CharacterController2D::get_speed);
        ClassDB::bind_method(D_METHOD("set_speed", "speed"), &CharacterController2D::set_speed);

        ClassDB::bind_method(D_METHOD("get_max_speed"), &CharacterController2D::get_max_speed);
        ClassDB::bind_method(D_METHOD("set_max_speed", "max_speed"), &CharacterController2D::set_max_speed);

        ClassDB::bind_method(D_METHOD("get_acceleration"), &CharacterController2D::get_acceleration);
        ClassDB::bind_method(D_METHOD("set_acceleration", "acceleration"), &CharacterController2D::set_acceleration);

        ClassDB::bind_method(D_METHOD("get_friction"), &CharacterController2D::get_friction);
        ClassDB::bind_method(D_METHOD("set_friction", "friction"), &CharacterController2D::set_friction);

        ADD_GROUP("Movement", "movement_");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "current_speed", PROPERTY_HINT_RANGE, "0,2000,1"), "set_speed",
                     "get_speed");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "max_speed", PROPERTY_HINT_RANGE, "0,2000,1"), "set_max_speed",
                     "get_max_speed");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "acceleration", PROPERTY_HINT_RANGE, "0,2000,1"),
                     "set_acceleration", "get_acceleration");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "friction", PROPERTY_HINT_RANGE, "0,2000,1"), "set_friction",
                     "get_friction");
    }

    void CharacterController2D::_ready()
    {
        this->set_process_input(true);
    }

    void CharacterController2D::_physics_process(double delta_time)
    {
        Node::_physics_process(delta_time);
    }

    void CharacterController2D::_process(double delta_time)
    {
        Node::_process(delta_time);
        process_movement(delta_time);
    }

    void CharacterController2D::process_movement(double delta_time)
    {
        const Vector2 movement{ Input::get_singleton()->get_vector("ui_left", "ui_right", "ui_up", "ui_down") };

        m_velocity += movement.is_zero_approx() ? -m_friction : movement * m_acceleration;
        m_velocity *= m_target_speed * delta_time;

        auto clamped_velocity = m_velocity.clamp(m_min_speed, m_max_speed);
        this->set_position(this->get_position() + clamped_velocity);
    }
}
