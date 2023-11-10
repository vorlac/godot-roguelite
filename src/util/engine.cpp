#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include "util/engine.hpp"

namespace rl::inline utils
{
    godot::Engine* engine::get()
    {
        godot::Engine* engine{ godot::Engine::get_singleton() };
        return engine;
    }

    godot::MainLoop* engine::main_loop()
    {
        auto engine{ engine::get() };
        return engine->get_main_loop();
    }

    godot::SceneTree* engine::scene_tree()
    {
        auto loop{ engine::main_loop() };
        return ::godot::Object::cast_to<godot::SceneTree>(loop);
    }

    godot::Window* engine::root_window()
    {
        auto tree{ engine::scene_tree() };
        return tree->get_root();
    }

    godot::Node* engine::root_node()
    {
        return godot::Object::cast_to<godot::Node>(engine::root_window());
    }

    void engine::set_fps(const uint32_t fps)
    {
        auto engine{ engine::get() };
        return engine->set_max_fps(fps);
    }

    int32_t engine::max_fps()
    {
        auto engine{ engine::get() };
        return engine->get_max_fps();
    }

    bool engine::editor_active()
    {
        auto engine{ engine::get() };
        return engine->is_editor_hint();
    }
}
