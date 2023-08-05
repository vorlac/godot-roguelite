#include "nodes/character.hpp"

#include "nodes/camera.hpp"
#include "util/bindings.hpp"
#include "util/utils.hpp"

#include <array>
#include <tuple>

#include <gdextension_interface.h>
#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/variant.hpp>

/**
 * @brief behavior
 * */
namespace godot
{
    void Character::_ready()
    {
        gdutils::print(FUNCTION_STR);
        this->set_motion_mode(MotionMode::MOTION_MODE_FLOATING);
        this->set_scale({ 0.70, 0.70 });
    }

    void Character::_enter_tree()
    {
        gdutils::print(FUNCTION_STR);
        Camera* player_camera{ memnew(Camera) };
        if (player_camera != nullptr)
        {
            this->add_child(player_camera);
            player_camera->set_owner(this);
        }
    }

    void Character::_exit_tree()
    {
        // TODO: revisit, delete all children? or does something else do that?
        const auto& children{ this->get_children() };
        if (not children.is_empty())
        {
            for (auto idx = 0; idx < children.size(); ++idx)
            {
                auto camera = Object::cast_to<Camera>(children[idx]);
                if (camera != nullptr)
                    this->remove_child(camera);
            }
        }
    }

    void Character::_physics_process(double delta_time)
    {
        auto camera{ this->get_node<Camera>("PlayerCamera") };
        if (camera != nullptr)
            camera->align();

        // called every 1/60th sec
        if (rl::editor::active())
            return;
    }

    void Character::_input(const Ref<InputEvent>& event)
    {
        // called when input is detected
        if (rl::editor::active())
            return;
    }

    void Character::_process(double delta_time)
    {
        if (rl::editor::active())
            return;

        Input* const input{ Input::get_singleton() };
        if (input != nullptr)
        {
            this->process_movement_input(input, delta_time);
            this->process_rotation_input(input, delta_time);

            Point2 mouse_pos{ this->get_global_mouse_position() };
            if (this->get_viewport_rect().has_point(mouse_pos))
                input->flush_buffered_events();

            m_elapsed_time += delta_time;
            if (m_elapsed_time > 1.0)
            {
                this->emit_signal(Signals::PositionChanged, this, this->get_global_position());
                m_elapsed_time = 0.0;
            }
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
                    gdutils::printerr("InputMode = Controller, but no controllers detected");
                else
                {
                    const uint32_t controller_id = controllers.front();
                    const Vector2 target_rotation = input->get_vector("rotate_left", "rotate_right",
                                                                      "rotate_up", "rotate_down");
                    if (!target_rotation.is_zero_approx())
                        m_rotation_angle = Vector2{ 0, 0 }.angle_to_point(target_rotation) +
                                           Math::deg_to_rad(90.0);
                }
                break;
            }
        }

        const double smoothed_rotation_angle = Math::lerp_angle(
            this->get_rotation(), m_rotation_angle, m_rotation_speed * delta_time);

        this->set_rotation(smoothed_rotation_angle);
    }
}

/**
 * @brief properties
 * */
namespace godot
{
    [[nodiscard]] double Character::get_movement_speed() const
    {
        return m_movement_speed;
    }

    void Character::set_movement_speed(const double move_speed)
    {
        m_movement_speed = move_speed;
    }

    [[nodiscard]] double Character::get_movement_friction() const
    {
        return m_movement_friction;
    }

    void Character::set_movement_friction(const double move_friction)
    {
        m_movement_friction = move_friction;
    }

    [[nodiscard]] double Character::get_rotation_speed() const
    {
        return m_rotation_speed;
    }

    void Character::set_rotation_speed(const double rotation_speed)
    {
        m_rotation_speed = rotation_speed;
    }
}

/**
 * @brief property/signal bindings
 * */
namespace godot
{
    void Character::bind_signals()
    {
        const static std::array signal_bindings = {
            rl::SignalBinding{
                Signals::PositionChanged,
                PropertyInfo{ Variant::OBJECT, "node" },
                PropertyInfo{ Variant::VECTOR2, "new_position" },
            },
        };

        for (const auto& signal : signal_bindings)
        {
            godot::ClassDB::add_signal(
                Character::get_class_static(),
                MethodInfo(signal.name, signal.receiver_info, signal.sender_info));
        }
    }

    void Character::bind_properties()
    {
        const static std::array property_bindings = {
            rl::PropertyBinding{
                std::tuple{ "get_movement_speed", "set_movement_speed" },
                std::tuple{ &Character::get_movement_speed, &Character::set_movement_speed },
                std::tuple{ "movement_speed", Variant::FLOAT },
            },
            rl::PropertyBinding{
                std::tuple{ "get_movement_friction", "set_movement_friction" },
                std::tuple{ &Character::get_movement_friction, &Character::set_movement_friction },
                std::tuple{ "movement_friction", Variant::FLOAT },
            },
            rl::PropertyBinding{
                std::tuple{ "get_rotation_speed", "set_rotation_speed" },
                std::tuple{ &Character::get_rotation_speed, &Character::set_rotation_speed },
                std::tuple{ "rotation_speed", Variant::FLOAT },
            }
        };

        for (const auto& bind : property_bindings)
        {
            ClassDB::bind_method(D_METHOD(bind.getter_name), bind.getter_func);
            ClassDB::bind_method(D_METHOD(bind.setter_name, bind.property_name), bind.setter_func);
            PropertyInfo binding_prop_info{ bind.property_type, bind.property_name };
            godot::ClassDB::add_property(Character::get_class_static(), binding_prop_info,
                                         bind.setter_name, bind.getter_name);
        }
    }

    void Character::_bind_methods()
    {
        Character::bind_properties();
        Character::bind_signals();
    }
}
