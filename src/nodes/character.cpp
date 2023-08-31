#include "nodes/character.hpp"

#include "nodes/camera.hpp"
#include "nodes/player_controller.hpp"
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
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    Character::Character()
    {
        this->set_name("Player");
        this->set_motion_mode(MotionMode::MOTION_MODE_FLOATING);
        this->set_scale({ 0.70, 0.70 });

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
    }

    void Character::_ready()
    {
        this->add_child(m_camera);
        this->add_child(m_sprite);
        this->add_child(m_collision_shape);
        this->add_child(m_player_controller);

        m_signal_connections = {
            SignalConnection(m_player_controller, signal::name::player_move, this, on_player_move),
            SignalConnection(m_player_controller, signal::name::player_rotate, this,
                             on_player_rotate),
            SignalConnection(m_player_controller, signal::name::player_shoot, this,
                             on_player_shoot),
        };

        for (const auto& conn : m_signal_connections)
        {
            auto& [signal_owner, sig, slot] = conn;
            signal_owner->connect(sig, slot);
        }
    }

    void Character::_process(double delta_time)
    {
    }

    [[signal_callback]]
    void Character::on_player_move(godot::Vector2 movement_velocity, double delta_time)
    {
        double increment = m_movement_friction * delta_time;
        godot::Vector2 velocity{ this->get_velocity().lerp(movement_velocity, increment) };
        velocity = velocity.clamp({ -1.0, -1.0 }, { 1.0, 1.0 });
        this->translate(velocity * this->get_movement_speed() * delta_time);
        this->set_velocity(velocity);
        this->move_and_slide();
    }

    [[signal_callback]]
    void Character::on_player_rotate(double rotation_angle, double delta_time)
    {
        double smoothed_angle = godot::Math::lerp_angle(this->get_rotation(), rotation_angle,
                                                        m_rotation_speed * delta_time);
        this->set_rotation(smoothed_angle);
    }

    [[signal_callback]]
    void Character::on_player_shoot()
    {
        // TODO: fix this
        this->emit_signal(signal::name::spawn_projectile, this);
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

    void Character::_bind_methods()
    {
        add_method_binding(Character, on_player_move);
        add_method_binding(Character, on_player_rotate);
        add_method_binding(Character, on_player_shoot);

        Character::bind_properties();
        Character::bind_signals();
    }

    void Character::bind_signals()
    {
        signal::bindings signals = {
            {
                signal::name::position_changed,
                { godot::PropertyInfo(godot::Variant::OBJECT, "node"),
                  godot::PropertyInfo(godot::Variant::VECTOR2, "new_position") },
            },
            {
                signal::name::spawn_projectile,
                { godot::PropertyInfo(godot::Variant::OBJECT, "node") },
            },
        };

        BindSignals(signals);
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
}
