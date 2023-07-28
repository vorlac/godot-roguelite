#include "character.hpp"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    void Character::_bind_methods()
    {
        UtilityFunctions::print(FUNCTION_STR);

        ClassDB::bind_method(D_METHOD("get_speed"), &Character::get_speed);
        ClassDB::bind_method(D_METHOD("set_speed", "speed"), &Character::set_speed);
        ClassDB::bind_method(D_METHOD("get_acceleration"), &Character::get_acceleration);
        ClassDB::bind_method(D_METHOD("set_acceleration", "acceleration"), &Character::set_acceleration);
        ClassDB::bind_method(D_METHOD("get_friction"), &Character::get_friction);
        ClassDB::bind_method(D_METHOD("set_friction", "friction"), &Character::set_friction);
        ClassDB::bind_method(D_METHOD("get_rotation_speed"), &Character::get_rotation_speed);
        ClassDB::bind_method(D_METHOD("set_rotation_speed", "rotation_speed"), &Character::set_rotation_speed);

        PropertyInfo speed_property_info{ Variant::VECTOR2, "speed", PROPERTY_HINT_RANGE, "0,2000,1" };
        PropertyInfo accel_property_info{ Variant::VECTOR2, "acceleration", PROPERTY_HINT_RANGE, "0,2000,1" };
        PropertyInfo friction_property_info{ Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0,1,0.01" };
        PropertyInfo rot_speed_property_info{ Variant::FLOAT, "rotation_speed", PROPERTY_HINT_RANGE, "0,50,0.01" };

        ADD_PROPERTY(speed_property_info, "set_speed", "get_speed");
        ADD_PROPERTY(accel_property_info, "set_acceleration", "get_acceleration");
        ADD_PROPERTY(friction_property_info, "set_friction", "get_friction");
        ADD_PROPERTY(rot_speed_property_info, "set_rotation_speed", "get_rotation_speed");
    }

    void Character::_ready()
    {
        UtilityFunctions::print(FUNCTION_STR);
        m_velocity = { 0.0, 0.0 };
    }

    void Character::_enter_tree()
    {
        UtilityFunctions::print(FUNCTION_STR);
    }

    void Character::_exit_tree()
    {
        // can't use UtilityFunctions::print here, crashes
    }

    void Character::_input(const Ref<InputEvent>& event)
    {
        //
    }

    void Character::_shortcut_input(const Ref<InputEvent>& event)
    {
        //
    }

    void Character::_unhandled_key_input(const Ref<InputEvent>& event)
    {
        //
    }

    void Character::_unhandled_input(const Ref<InputEvent>& event)
    {
        // TODO: fix... doesn't handle deadzones properly
        const String event_type{ event->get_class() };
        switch (m_input_mode)
        {
            default:
                [[fallthrough]];
            case InputMode::MouseAndKeyboard:
            {
                // InputEventJoypadMotion, InputEventJoypadButton
                if (event_type.begins_with("InputEventJoypad"))
                    m_input_mode = InputMode::Controller;
                break;
            }
            case InputMode::Controller:
            {
                // InputEventMouseMotion, InputEventMouseButton, InputEventKey
                if (event_type.begins_with("InputEventMouse") || event_type.begins_with("InputEventKey"))
                    m_input_mode = InputMode::MouseAndKeyboard;
                break;
            }
        }
    }

    void Character::_physics_process(double delta_time)
    {
        // called in a fixed time step interval
    }

    void Character::_process(double delta_time)
    {
        // called every frame
        Input* const input{ Input::get_singleton() };
        this->process_movement_input(input, delta_time);
        this->process_rotation_input(input, delta_time);
    }

    void Character::process_movement_input(Input* const input, double delta_time)
    {
        Vector2 raw_movement_input{ input->get_vector("ui_left", "ui_right", "ui_up", "ui_down") };
        Vector2 target_velocity = raw_movement_input.is_zero_approx() ? Vector2{ 0, 0 } : raw_movement_input;

        m_velocity = m_velocity.lerp(target_velocity, m_friction);
        m_velocity = m_velocity.clamp({ -1.0, -1.0 }, { 1.0, 1.0 });

        auto movement_offset{ m_velocity * m_target_speed * delta_time };
        this->set_position(this->get_position() + movement_offset);
    }

    Character::InputMode Character::get_input_mode(Input* const input)
    {
        switch (m_input_mode)
        {
            default:
                [[fallthrough]];
            case InputMode::MouseAndKeyboard:
            {
                TypedArray<int32_t> controllers{ input->get_connected_joypads() };
                if (!controllers.is_empty())
                {
                    uint32_t controller_id = controllers.front();

                    float rotation_x_axis = input->get_joy_axis(controller_id, JoyAxis::JOY_AXIS_RIGHT_X);
                    float rotation_y_axis = input->get_joy_axis(controller_id, JoyAxis::JOY_AXIS_RIGHT_Y);
                    float movement_x_axis = input->get_joy_axis(controller_id, JoyAxis::JOY_AXIS_LEFT_X);
                    float movement_y_axis = input->get_joy_axis(controller_id, JoyAxis::JOY_AXIS_LEFT_Y);

                    auto deadzone = 0.15;
                    rotation_x_axis = Math::abs(rotation_x_axis) < deadzone ? 0.0 : rotation_x_axis;
                    rotation_y_axis = Math::abs(rotation_y_axis) < deadzone ? 0.0 : rotation_y_axis;
                    movement_x_axis = Math::abs(rotation_x_axis) < deadzone ? 0.0 : rotation_x_axis;
                    movement_y_axis = Math::abs(rotation_y_axis) < deadzone ? 0.0 : rotation_y_axis;

                    if (!Math::is_zero_approx(rotation_x_axis) || !Math::is_zero_approx(rotation_y_axis) ||
                        !Math::is_zero_approx(movement_x_axis) || !Math::is_zero_approx(movement_y_axis))
                        m_input_mode = InputMode::Controller;
                }
                break;
            }
            case InputMode::Controller:
            {
                if (!input->get_last_mouse_velocity().is_zero_approx())
                    m_input_mode = InputMode::MouseAndKeyboard;
                break;
            }
        }

        return m_input_mode;
    }

    void Character::process_rotation_input(Input* const input, const double delta_time)
    {
        switch (this->get_input_mode(input))
        {
            default:
                [[fallthrough]];
            case InputMode::MouseAndKeyboard:
            {
                Point2 mouse_position{ this->get_viewport()->get_mouse_position() };
                m_rotation_angle = this->get_position().angle_to_point(this->get_global_mouse_position()) +
                                   Math::deg_to_rad(90.0);
                break;
            }
            case InputMode::Controller:
            {
                TypedArray<int32_t> controllers{ input->get_connected_joypads() };
                if (controllers.is_empty())
                    UtilityFunctions::printerr("Input Mode = Controller, but no controllers detected");
                else
                {
                    uint32_t controller_id = controllers.front();

                    float x_axis = input->get_joy_axis(controller_id, JoyAxis::JOY_AXIS_RIGHT_X);
                    float y_axis = input->get_joy_axis(controller_id, JoyAxis::JOY_AXIS_RIGHT_Y);

                    auto deadzone = 0.15;
                    x_axis = Math::abs(x_axis) < deadzone ? 0.0 : x_axis;
                    y_axis = Math::abs(y_axis) < deadzone ? 0.0 : y_axis;

                    if (!Math::is_zero_approx(x_axis) || !Math::is_zero_approx(y_axis))
                    {
                        const Vector2 target_rotation{ -y_axis, x_axis };
                        m_rotation_angle = Vector2{ 0, 0 }.angle_to_point(target_rotation);
                    }
                }

                break;
            }
        }

        double rotation{ Math::lerp_angle(this->get_rotation(), m_rotation_angle, m_rotation_speed * delta_time) };
        this->set_rotation(rotation);
    }
}
