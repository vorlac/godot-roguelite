#pragma once

#include <array>
#include <concepts>
#include <functional>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>

namespace godot
{
    class Input;
    class InputMap;

    template <typename TGet, typename TSet>
    struct PropertyBinding
    {
        PropertyBinding(
            std::tuple<String, String> func_names, std::tuple<TGet, TSet> func_callables,
            std::tuple<String, Variant::Type> prop_desc,
            std::optional<std::tuple<String, PropertyInfo, PropertyInfo>> signal_info = std::nullopt
        )
            : getter_name{ std::move(std::get<0>(func_names)) }
            , setter_name{ std::move(std::get<1>(func_names)) }
            , getter_func{ std::get<0>(func_callables) }
            , setter_func{ std::get<1>(func_callables) }
            , property_name{ std::move(std::get<0>(prop_desc)) }
            , property_type{ std::move(std::get<1>(prop_desc)) }
        {
        }

        String getter_name{};
        String setter_name{};
        String property_name{};
        TGet getter_func{ nullptr };
        TSet setter_func{ nullptr };
        Variant::Type property_type{};
    };

    struct SignalBinding
    {
        SignalBinding(String&& name, PropertyInfo&& recv_info, PropertyInfo&& send_info)
            : name{ std::move(name) }
            , sender_info{ std::move(send_info) }
            , receiver_info{ std::move(recv_info) }
        {
        }

        String name{};
        PropertyInfo sender_info{};
        PropertyInfo receiver_info{};
    };

    class Character : public CharacterBody2D
    {
        GDCLASS(Character, CharacterBody2D);

    private:
        enum class InputMode
        {
            MouseAndKeyboard,
            Controller
        };

    public:
        void _ready() override;
        void _enter_tree() override;
        void _exit_tree() override;
        void _process(double delta_time) override;
        void _physics_process(double delta_time) override;
        void _input(const Ref<InputEvent>& event) override;
        void _unhandled_input(const Ref<InputEvent>& event) override;
        void _unhandled_key_input(const Ref<InputEvent>& event) override;
        void _shortcut_input(const Ref<InputEvent>& event) override;
        static void _bind_methods();

        double get_movement_speed() const;
        double get_movement_friction() const;
        double get_rotation_speed() const;
        void set_movement_speed(const double move_speed);
        void set_movement_friction(const double move_friction);
        void set_rotation_speed(const double rotation_speed);

    protected:
        InputMode get_input_mode(Input* const input);
        void process_movement_input(Input* input, double delta_time);
        void process_rotation_input(Input* input, double delta_time);

    private:
        // active input mode for character controls
        InputMode m_input_mode{ InputMode::MouseAndKeyboard };
        // Rate of acceleration/deceleration (unit/s/s)
        double m_movement_friction{ 5.0 };
        // Rate of acceleration/deceleration (unit/s/s)
        double m_rotation_speed{ 5.0 };
        // target movement speed (units/s)
        double m_movement_speed{ 500.0 };
        // target facing angle (radians)
        double m_rotation_angle{ 0.0 };
        // elapsed runtime (seconds)
        double m_elapsed_time{ 0.0 };
        // TODO: break this out into an editor tool
        constexpr static inline bool RL_DEBUG{ true };
    };
}
