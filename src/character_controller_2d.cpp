#include "character_controller_2d.hpp"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    void CharacterController2D::_bind_methods()
    {
        UtilityFunctions::print("Binding methods");

        ClassDB::bind_method(D_METHOD("get_speed"), &CharacterController2D::get_speed);
        ClassDB::bind_method(D_METHOD("set_speed", "speed"), &CharacterController2D::set_speed);
        ClassDB::bind_method(D_METHOD("get_acceleration"), &CharacterController2D::get_acceleration);
        ClassDB::bind_method(D_METHOD("set_acceleration", "acceleration"), &CharacterController2D::set_acceleration);
        ClassDB::bind_method(D_METHOD("get_friction"), &CharacterController2D::get_friction);
        ClassDB::bind_method(D_METHOD("set_friction", "friction"), &CharacterController2D::set_friction);

        PropertyInfo speed_property_info{ Variant::VECTOR2, "speed", PROPERTY_HINT_RANGE, "0,2000,1" };
        PropertyInfo accel_property_info{ Variant::VECTOR2, "acceleration", PROPERTY_HINT_RANGE, "0,2000,1" };
        PropertyInfo friction_property_info{ Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0,1,0.01" };

        ADD_PROPERTY(speed_property_info, "set_speed", "get_speed");
        ADD_PROPERTY(accel_property_info, "set_acceleration", "get_acceleration");
        ADD_PROPERTY(friction_property_info, "set_friction", "get_friction");
    }

    void CharacterController2D::_ready()
    {
        m_velocity = { 0.0, 0.0 };
    }

    void CharacterController2D::_physics_process(double delta_time)
    {
        Input* input = Input::get_singleton();

        {
            // movement
            Vector2 test{ std::move(input->get_vector("ui_left", "ui_right", "ui_up", "ui_down")) };
            Vector2 target_velocity = test.is_zero_approx() ? Vector2{ 0, 0 } : test;

            m_velocity = { m_velocity.lerp(target_velocity, m_friction) };
            m_velocity = { m_velocity.clamp({ -1.0, -1.0 }, { 1.0, 1.0 }) };

            auto movement_offset{ m_velocity * m_target_speed * delta_time };
            this->set_position(this->get_position() + movement_offset);
        }

        {
            // rotation
            TypedArray<int32_t> controllers{ input->get_connected_joypads() };
            if (controllers.is_empty())
            {
                Point2 mouse_position{ this->get_viewport()->get_mouse_position() };
                Vector2 mouse_velocity{ input->get_last_mouse_velocity() };
            }
            else
            {
                uint32_t controller_id = controllers.front();

                float x_axis = input->get_joy_axis(controller_id, JoyAxis::JOY_AXIS_RIGHT_X);
                float y_axis = input->get_joy_axis(controller_id, JoyAxis::JOY_AXIS_RIGHT_Y);

                auto deadzone = 0.15;
                x_axis = Math::abs(x_axis) < deadzone ? 0.0 : x_axis;
                y_axis = Math::abs(y_axis) < deadzone ? 0.0 : y_axis;

                if (!Math::is_zero_approx(x_axis) || !Math::is_zero_approx(y_axis))
                    m_target_rotation = { -y_axis, x_axis };

                if (!m_target_rotation.is_equal_approx(m_prev_rotation))
                {
                    const auto smoothed_rot{ m_prev_rotation.lerp(m_target_rotation, m_friction) };
                    const auto angle{ Vector2{ 0, 0 }.angle_to_point(smoothed_rot) };
                    this->set_rotation(angle);

                    UtilityFunctions::print("raw input:{" + m_target_rotation + "}");
                    m_prev_rotation = m_target_rotation;
                }
            }
        }
    }

    void CharacterController2D::_process(double delta_time)
    {
        Node::_process(delta_time);
        // process_movement(delta_time);
    }
}
