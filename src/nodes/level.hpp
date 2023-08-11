#pragma once

#include "nodes/character.hpp"
#include "util/signals.hpp"
#include "util/utils.hpp"

#include <array>
#include <tuple>
#include <utility>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot
{
    class Input;
    class Object;
    class Vector2;
    class InputEvent;

    class Level : public Node2D
    {
        GDCLASS(Level, Node2D);

    public:
        Level();
        ~Level();

        void _ready() override;
        void _input(const Ref<InputEvent>& event) override;

    protected:
        [[signal_callback]]
        void on_character_position_changed(const Object* node, Vector2 location) const;

        static void _bind_methods()
        {
            ClassDB::bind_method(D_METHOD("on_character_position_changed"),
                                 &Level::on_character_position_changed);
        }

    private:
        Character* m_player{ memnew(Character) };
    };
}
