#pragma once

#include "util/input.hpp"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/wrapped.hpp>

namespace rl
{
    struct Action
    {
        enum class UI
        {
            NavigateUp,
            NavigateDown,
            NavigateLeft,
            NavigateRight,
            NavigateBack,
            NavigateForward,

            Apply,
            Cancel,

            Next,
            Previous
        };

        enum class Game
        {
            MoveUp,
            MoveDown,
            MoveLeft,
            MoveRight,
            Dodge,

            RotateUp,
            RotateDown,
            RotateLeft,
            RotateRight,

            Shoot,
            NextWeapon,
            PreviousWeapon,

            UseItem,
            UseSpecial,

            Pause,
            Menu,

            // etc
        };
    };

    class InputMapping : public godot::Node
    {
        GDCLASS(InputMapping, godot::Node);

    public:
        InputMapping()
        {
        }

        ~InputMapping()
        {
        }

        void _unhandled_input(const godot::Ref<godot::InputEvent>& event)
        {
        }

    protected:
        static void _bind_methods()
        {
        }

    private:
        static inline godot::InputMap* const m_input_map{ nullptr };
    };

}
