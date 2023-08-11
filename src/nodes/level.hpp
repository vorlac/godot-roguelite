#pragma once

#include "nodes/character.hpp"
#include "util/signals.hpp"
#include "util/utils.hpp"

#include <utility>
#include <vector>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    class godot::Input;
    class godot::Object;
    class godot::InputEvent;

    class Level : public godot::Node2D
    {
        GDCLASS(Level, godot::Node2D);

    public:
        Level();
        ~Level();

        void _ready() override;
        void _input(const godot::Ref<godot::InputEvent>& event) override;

    protected:
        [[signal_callback]]
        void on_character_position_changed(const godot::Object* node,
                                           godot::Vector2 location) const;

        static void _bind_methods()
        {
            godot::ClassDB::bind_method(godot::D_METHOD("on_character_position_changed"),
                                        &rl::Level::on_character_position_changed);
        }

    private:
        using callback_connection = std::pair<godot::String, godot::Callable>;
        std::vector<callback_connection> m_signal_connections{};
        rl::Character* m_player{ memnew(rl::Character) };
    };
}
