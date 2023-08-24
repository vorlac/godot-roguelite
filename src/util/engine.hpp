#pragma once

#include "util/debug.hpp"
#include "util/io.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

namespace rl::inline utils
{
    struct engine
    {
        static inline godot::Engine* const get()
        {
            godot::Engine* const engine{ godot::Engine::get_singleton() };
            debug::assert(engine != nullptr);
            return engine;
        }

        static inline godot::MainLoop* const main_loop()
        {
            auto engine{ engine::get() };
            if (engine != nullptr) [[likely]]
                return engine->get_main_loop();
            return nullptr;
        }

        static inline godot::SceneTree* const scene_tree()
        {
            auto loop{ engine::main_loop() };
            if (loop != nullptr) [[likely]]
                return godot::Object::cast_to<godot::SceneTree>(loop);
            return nullptr;
        }

        static inline godot::Window* const root_window()
        {
            auto tree{ engine::scene_tree() };
            if (tree != nullptr) [[likely]]
                return tree->get_root();
            return nullptr;
        }

        static inline godot::Node* const root_node()
        {
            return godot::Object::cast_to<godot::Node>(engine::root_window());
        }

        const static inline int32_t max_fps()
        {
            auto engine{ engine::get() };
            if (engine != nullptr) [[likely]]
                return engine->get_max_fps();

            log::error("Failed to get max fps");
        }

        static inline void set_fps(const uint32_t fps)
        {
            auto engine{ engine::get() };
            if (engine != nullptr) [[likely]]
                return engine->set_max_fps(fps);

            log::error("Failed to set max fps");
        }

        const static inline bool editor_active()
        {
            auto engine{ engine::get() };
            if (engine != nullptr) [[likely]]
                return engine->is_editor_hint();
            return false;
        }
    };

}