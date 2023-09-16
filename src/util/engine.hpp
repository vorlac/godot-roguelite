#pragma once

#include <cstdint>

namespace godot
{
    class Engine;
    class MainLoop;
    class Node;
    class SceneTree;
    class Window;
}

namespace rl::inline utils
{
    struct engine
    {
        static godot::Engine* get();
        static godot::MainLoop* main_loop();
        static godot::SceneTree* scene_tree();
        static godot::Window* root_window();
        static godot::Node* root_node();
        static void set_fps(const uint32_t fps);
        static int32_t max_fps();
        static bool editor_active();
    };
}
