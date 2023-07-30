#include "character.hpp"

#include "utils/utils.hpp"

#include <array>
#include <gdextension_interface.h>
#include <tuple>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>

namespace godot
{
    void Character::_bind_methods()
    {
        rl::utils::print(FUNCTION_STR);

        // {
        //     { getter function name, getter function pointer }
        //     { setter function name, setter function pointer }
        //     { property name / setter param, property type }
        // }

        constexpr std::array METHOD_BINDINGS = {
            std::tuple(
                std::pair("get_movement_speed", &Character::get_movement_speed),
                std::pair("set_movement_speed", &Character::set_movement_speed),
                std::pair("move_speed", Variant::FLOAT)
            ),
            std::tuple(
                std::pair("get_movement_friction", &Character::get_movement_friction),
                std::pair("set_movement_friction", &Character::set_movement_friction),
                std::pair("move_friction", Variant::FLOAT)
            ),
            std::tuple(
                std::pair("get_rotation_speed", &Character::get_rotation_speed),
                std::pair("set_rotation_speed", &Character::set_rotation_speed),
                std::pair("rotation_speed", Variant::FLOAT)
            )
        };

        for (auto&& binding : METHOD_BINDINGS)
        {
            auto&& [getter_info, setter_info, properties] = std::move(binding);
            auto&& [getter_method_name, getter_callable] = std::move(getter_info);
            auto&& [setter_method_name, setter_callable] = std::move(setter_info);
            auto&& [property_name, property_type] = std::move(properties);

            ClassDB::bind_method(D_METHOD(getter_method_name), getter_callable);
            ClassDB::bind_method(D_METHOD(setter_method_name, property_name), setter_callable);

            PropertyInfo binding_prop_info{ property_type, property_name };
            ADD_PROPERTY(binding_prop_info, setter_method_name, getter_method_name);
        }
    }

    void Character::_ready()
    {
        this->set_motion_mode(MotionMode::MOTION_MODE_FLOATING);
        InputMap* const input_map{ InputMap::get_singleton() };
        input_map->load_from_project_settings();
    }

    void Character::_enter_tree()
    {
        rl::utils::print(FUNCTION_STR);
    }

    void Character::_exit_tree()
    {
        //
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
        //
    }

    double Character::get_movement_speed() const
    {
        return m_movement_speed;
    }

    double Character::get_movement_friction() const
    {
        return m_movement_friction;
    }

    double Character::get_rotation_speed() const
    {
        return m_rotation_speed;
    }

    void Character::set_movement_speed(const double move_speed)
    {
        m_movement_speed = move_speed;
    }

    void Character::set_movement_friction(const double move_friction)
    {
        m_movement_friction = move_friction;
    }

    void Character::set_rotation_speed(const double rotation_speed)
    {
        m_rotation_speed = rotation_speed;
    }

    void Character::_physics_process(double delta_time)
    {
        // called in a fixed time step interval
    }

    void Character::_process(double delta_time)
    {
        Input* const input{ Input::get_singleton() };
        if (input != nullptr)
        {
            m_elapsed_time += delta_time;
            if constexpr (RL_DEBUG)
                input->set_block_signals(false);

            this->process_movement_input(input, delta_time);
            this->process_rotation_input(input, delta_time);

            Point2 mouse_pos{ this->get_global_mouse_position() };
            if (this->get_viewport_rect().has_point(mouse_pos))
                input->flush_buffered_events();

            if constexpr (RL_DEBUG)
                input->set_block_signals(true);
        }
    }

    void Character::process_movement_input(Input* const input, double delta_time)
    {
        Vector2 velocity{ input->get_vector("move_left", "move_right", "move_up", "move_down") };
        velocity = this->get_velocity().lerp(velocity, m_movement_friction * delta_time);
        velocity = velocity.clamp({ -1.0, -1.0 }, { 1.0, 1.0 });
        this->translate(velocity * this->get_movement_speed() * delta_time);
        this->set_velocity(velocity);
        this->move_and_slide();
    }

    Character::InputMode Character::get_input_mode(Input* const input)
    {
        switch (m_input_mode)
        {
            default:
                [[fallthrough]];
            case InputMode::MouseAndKeyboard:
            {
                const bool controller_input_detected{ input->is_action_pressed("controller_any") };
                if (controller_input_detected)
                    m_input_mode = InputMode::Controller;
                break;
            }
            case InputMode::Controller:
            {
                const auto&& mouse_velocity{ input->get_last_mouse_velocity() };
                if (!mouse_velocity.is_zero_approx())
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
                Vector2 rotation_direction{ this->get_global_mouse_position() -
                                            this->get_global_position() };
                m_rotation_angle = rotation_direction.angle() + Math::deg_to_rad(90.0);
                break;
            }
            case InputMode::Controller:
            {
                TypedArray<int32_t> controllers{ std::move(input->get_connected_joypads()) };
                if (controllers.is_empty())
                    rl::utils::printerr("InputMode = Controller, but no controllers detected");
                else
                {
                    const uint32_t controller_id = controllers.front();
                    const Vector2 target_rotation = input->get_vector(
                        "rotate_left", "rotate_right", "rotate_up", "rotate_down"
                    );
                    if (!target_rotation.is_zero_approx())
                        m_rotation_angle = Vector2{ 0, 0 }.angle_to_point(target_rotation) +
                                           Math::deg_to_rad(90.0);
                }
                break;
            }
        }

        const double smoothed_rotation_angle =
            Math::lerp_angle(this->get_rotation(), m_rotation_angle, m_rotation_speed * delta_time);

        this->set_rotation(smoothed_rotation_angle);
    }
}
