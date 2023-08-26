#pragma once

namespace godot
{
    class InputMap;
    class Input;
}

namespace rl::inline utils
{
    struct input
    {
        struct map
        {
            static godot::InputMap* get();
        };

        static godot::Input* get();

        static void hide_cursor();
        static void show_cursor();
        static bool cursor_visible();
        static void load_project_inputs();
        static void use_accumulated_inputs(bool enable);
    };
}
