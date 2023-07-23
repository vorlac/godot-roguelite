#include "character_controller_2d.hpp"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    void CharacterController2D::_bind_methods()
    {
        UtilityFunctions::print("Binding methods");

        ClassDB::bind_method(D_METHOD("get_speed"), &CharacterController2D::get_speed);
        ClassDB::bind_method(D_METHOD("set_speed", "speed"), &CharacterController2D::set_speed);
        PropertyInfo speed_property_info{ Variant::VECTOR2, "speed", PROPERTY_HINT_RANGE, "0,2000,1" };
        ADD_PROPERTY(speed_property_info, "set_speed", "get_speed");

        ClassDB::bind_method(D_METHOD("get_acceleration"), &CharacterController2D::get_acceleration);
        ClassDB::bind_method(D_METHOD("set_acceleration", "acceleration"), &CharacterController2D::set_acceleration);
        PropertyInfo accel_property_info{ Variant::VECTOR2, "acceleration", PROPERTY_HINT_RANGE, "0,2000,1" };
        ADD_PROPERTY(accel_property_info, "set_acceleration", "get_acceleration");

        ClassDB::bind_method(D_METHOD("get_friction"), &CharacterController2D::get_friction);
        ClassDB::bind_method(D_METHOD("set_friction", "friction"), &CharacterController2D::set_friction);
        PropertyInfo friction_property_info{ Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0,1,0.01" };
        ADD_PROPERTY(friction_property_info, "set_friction", "get_friction");
    }

    void CharacterController2D::_ready()
    {
        m_velocity = { 0.0, 0.0 };
    }

    void CharacterController2D::_unhandled_input(const Ref<InputEvent>& event)
    {
        Input* input = Input::get_singleton();
        real_t vert_input = input->get_axis("ui_down", "ui_up");
        real_t horiz_input = input->get_axis("ui_left", "ui_right");

        Vector2 target_velocity{ Math::is_zero_approx(horiz_input) ? 0.0f : horiz_input,
                                 Math::is_zero_approx(vert_input) ? 0.0f : vert_input };

        Vector2 temp1{ m_velocity.lerp(target_velocity, m_friction) };
        Vector2 temp2{ temp1.clamp({ -1.0, -1.0 }, { 1.0, 1.0 }) };
        m_velocity = temp2;
    }

    void CharacterController2D::_physics_process(double delta_time)
    {
        auto movement_offst{ m_velocity * m_target_speed * delta_time };
        this->set_position(this->get_position() + movement_offst);

        /*Node::_physics_process(delta_time);
        Input* input = Input::get_singleton();*/
    }

    void CharacterController2D::_process(double delta_time)
    {
        // Node::_process(delta_time);
        // process_movement(delta_time);
    }

    void CharacterController2D::process_movement(double delta_time)
    {
        /*const Vector2 movement{ Input::get_singleton()->get_vector("ui_left", "ui_right", "ui_up", "ui_down") };

        m_velocity += movement.is_zero_approx() ? -m_friction : movement * m_acceleration;
        m_velocity *= m_target_speed * delta_time;

        auto clamped_velocity = m_velocity.clamp(m_min_speed, m_max_speed);
        this->set_position(this->get_position() + clamped_velocity);*/
    }
}
