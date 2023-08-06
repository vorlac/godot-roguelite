#pragma once
#include "nodes/character.hpp"
#include "util/signals.hpp"
#include "util/utils.hpp"

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>

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

        void _ready() override;
        void _enter_tree() override;
        void _exit_tree() override;
        void _input(const Ref<InputEvent>& event) override;

        static void _bind_methods();
        static void bind_signals();
        static void bind_properties();

    protected:
        [[signal_callback]]
        void on_character_position_changed(const Object* node, Vector2 location) const;

    private:
        Character* m_player{ memnew(Character) };
    };
}
