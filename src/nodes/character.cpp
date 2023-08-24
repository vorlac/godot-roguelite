#include "nodes/character.hpp"

#include "nodes/camera.hpp"
#include "util/bind.hpp"
#include "util/utils.hpp"

#include <array>
#include <tuple>
#include <vector>

#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    Character::Character()
    {
        this->set_motion_mode(MotionMode::MOTION_MODE_FLOATING);
        this->set_scale({ 0.70, 0.70 });
        this->set_name("Player");

        godot::ResourceLoader* resource_loader{ resource::loader::get() };
        godot::Ref<godot::Resource> player_image{ resource_loader->load(path::sprite::Player) };

        m_sprite->set_texture(player_image);
        m_sprite->set_name("PlayerSprite");

        godot::Rect2 sprite_rect{ m_sprite->get_rect() };
        godot::Ref<godot::RectangleShape2D> collision_rect{ memnew(godot::RectangleShape2D) };
        collision_rect->set_size(sprite_rect.get_size());
        collision_rect->set_name("PlayerCollisionRect");

        m_collision_shape->set_shape(collision_rect);
        m_collision_shape->set_debug_color({ 255, 0, 0 });
        m_collision_shape->set_modulate(godot::Color::hex(0x8B000077));
        m_collision_shape->set_visible(true);
        m_collision_shape->set_name("PlayerCollisionShape");

        m_camera->set_name("PlayerCamera");
    }

    Character::~Character()
    {
        // if (!this->is_queued_for_deletion())
        //     this->queue_free();
    }

    void Character::_bind_methods()
    {
        Character::bind_properties();
        Character::bind_signals();
    }

    void Character::_ready()
    {
        this->add_child(m_sprite);
        this->add_child(m_collision_shape);
        this->add_child(m_camera);
    }

    void Character::_process(double delta_time)
    {
        if (engine::editor_active())
            return;

        godot::Input* const input_handler{ input::get() };
        if (input_handler != nullptr)
        {
            this->process_movement_input(input_handler, delta_time);
            this->process_rotation_input(input_handler, delta_time);
            this->process_state_input(input_handler, delta_time);

            godot::Point2 mouse_pos{ this->get_global_mouse_position() };
            if (this->get_viewport_rect().has_point(mouse_pos))
                input_handler->flush_buffered_events();

            m_elapsed_time += delta_time;
            if (m_elapsed_time > 1.0)
            {
                this->emit_signal(signal::name::position_changed, this, get_global_position());
                m_elapsed_time = 0.0;
            }
        }
    }

    void Character::process_state_input(godot::Input* const input, double delta_time)
    {
        if (input->is_action_pressed("shoot"))
            this->emit_signal(signal::name::shoot_projectile, this);
    }

    void Character::process_movement_input(godot::Input* const input, double delta_time)
    {
        using godot::Vector2;
        Vector2 velocity{ input->get_vector("move_left", "move_right", "move_up", "move_down") };
        velocity = this->get_velocity().lerp(velocity, m_movement_friction * delta_time);
        velocity = velocity.clamp({ -1.0, -1.0 }, { 1.0, 1.0 });
        this->translate(velocity * this->get_movement_speed() * delta_time);
        this->set_velocity(velocity);
        this->move_and_slide();
    }

    Character::InputMode Character::get_input_mode(godot::Input* const input)
    {
        switch (m_input_mode)
        {
            default:
            case InputMode::MouseAndKeyboard:
            {
                bool controller_input_detected{ input->is_action_pressed("controller_any") };
                if (controller_input_detected)
                    m_input_mode = InputMode::Controller;
                break;
            }
            case InputMode::Controller:
            {
                godot::Vector2 mouse_velocity{ input->get_last_mouse_velocity() };
                if (!mouse_velocity.is_zero_approx())
                    m_input_mode = InputMode::MouseAndKeyboard;
                break;
            }
        }

        return m_input_mode;
    }

    void Character::process_rotation_input(godot::Input* const input, const double delta_time)
    {
        switch (this->get_input_mode(input))
        {
            default:
                [[fallthrough]];
            case InputMode::MouseAndKeyboard:
            {
                godot::Vector2 rotation_dir{ this->get_global_mouse_position() -
                                             this->get_global_position() };

                m_rotation_angle = rotation_dir.angle() + godot::Math::deg_to_rad(90.0);
                break;
            }
            case InputMode::Controller:
            {
                godot::TypedArray<int32_t> controllers{ input->get_connected_joypads() };
                if (controllers.is_empty())
                    rl::log::warning("InputMode = Controller, but no controllers detected");
                else
                {
                    godot::Vector2 target_rotation{ input->get_vector("rotate_left", "rotate_right",
                                                                      "rotate_up", "rotate_down") };
                    if (!target_rotation.is_zero_approx())
                        m_rotation_angle = godot::Vector2{ 0, 0 }.angle_to_point(target_rotation) +
                                           godot::Math::deg_to_rad(90.0);
                }
                break;
            }
        }

        double smoothed_angle = godot::Math::lerp_angle(this->get_rotation(), m_rotation_angle,
                                                        m_rotation_speed * delta_time);
        this->set_rotation(smoothed_angle);
    }

    void Character::bind_signals()
    {
        static std::array signal_bindings{
            signal::binding<Character>{
                signal::name::position_changed,
                { godot::PropertyInfo(godot::Variant::OBJECT, "node"),
                  godot::PropertyInfo(godot::Variant::VECTOR2, "new_position") },
            },
            signal::binding<Character>{
                signal::name::shoot_projectile,
                { godot::PropertyInfo(godot::Variant::OBJECT, "node") },
            }
        };

        for (auto&& signal : signal_bindings)
            signal.add_to_classdb();
    }

    void Character::bind_properties()
    {
        const static std::array property_bindings = {
            rl::PropertyBinding{
                std::tuple{ "get_movement_speed", "set_movement_speed" },
                std::tuple{ &Character::get_movement_speed, &Character::set_movement_speed },
                std::tuple{ "movement_speed", godot::Variant::FLOAT },
            },
            rl::PropertyBinding{
                std::tuple{ "get_movement_friction", "set_movement_friction" },
                std::tuple{ &Character::get_movement_friction, &Character::set_movement_friction },
                std::tuple{ "movement_friction", godot::Variant::FLOAT },
            },
            rl::PropertyBinding{
                std::tuple{ "get_rotation_speed", "set_rotation_speed" },
                std::tuple{ &Character::get_rotation_speed, &Character::set_rotation_speed },
                std::tuple{ "rotation_speed", godot::Variant::FLOAT },
            }
        };

        for (auto&& bind : property_bindings)
        {
            godot::ClassDB::bind_method(godot::D_METHOD(bind.getter_name), bind.getter_func);
            godot::ClassDB::bind_method(godot::D_METHOD(bind.setter_name, bind.property_name),
                                        bind.setter_func);
            godot::PropertyInfo binding_prop_info{ bind.property_type, bind.property_name };
            godot::ClassDB::add_property(Character::get_class_static(), binding_prop_info,
                                         bind.setter_name, bind.getter_name);
        }
    }

    [[node_property]]
    double Character::get_movement_speed() const
    {
        return m_movement_speed;
    }

    [[node_property]]
    void Character::set_movement_speed(const double move_speed)
    {
        m_movement_speed = move_speed;
    }

    [[node_property]]
    double Character::get_movement_friction() const
    {
        return m_movement_friction;
    }

    [[node_property]]
    void Character::set_movement_friction(const double move_friction)
    {
        m_movement_friction = move_friction;
    }

    [[node_property]]
    double Character::get_rotation_speed() const
    {
        return m_rotation_speed;
    }

    [[node_property]]
    void Character::set_rotation_speed(const double rotation_speed)
    {
        m_rotation_speed = rotation_speed;
    }
}
