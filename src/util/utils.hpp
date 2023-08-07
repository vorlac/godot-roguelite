#pragma once

#include "util/io.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/string.hpp>

using gdutils = godot::UtilityFunctions;

namespace rl
{
    using godot::Engine;
    using godot::InputMap;
    using godot::MainLoop;
    using godot::Node;
    using godot::Object;
    using godot::SceneTree;
    using godot::String;
    using godot::Window;

    namespace project
    {
        constexpr inline String&& project_path(const String& rel_path)
        {
            return std::move("res://" + rel_path);
        }

        constexpr inline String&& user_data_path(const String& rel_path)
        {
            return std::move("user://" + rel_path);
        }
    }

    namespace scene
    {
        template <typename TNode = Node>
        inline Node* edited_root(TNode* node)
        {
            Node* edited_root{ node->get_tree()->get_edited_scene_root() };
            rl::log::assert(edited_root != nullptr);
            return edited_root;
        }
    }

    namespace input
    {
        const static inline void load_project_inputs()
        {
            auto input_map{ InputMap::get_singleton() };
            if (input_map != nullptr) [[likely]]
                input_map->load_from_project_settings();
        }

    }

    namespace editor
    {
        const static inline bool active()
        {
            auto editor{ Engine::get_singleton() };
            if (editor != nullptr) [[likely]]
                return editor->is_editor_hint();
            return false;
        }
    }

    namespace game
    {
        const static inline bool active()
        {
            return !editor::active();
        }
    }

    namespace engine
    {
        static inline MainLoop* main_loop()
        {
            auto engine{ Engine::get_singleton() };
            if (engine != nullptr) [[likely]]
                return engine->get_main_loop();
            return nullptr;
        }

        static inline SceneTree* scene_tree()
        {
            auto loop{ engine::main_loop() };
            if (loop != nullptr) [[likely]]
                return Object::cast_to<SceneTree>(loop);
            return nullptr;
        }

        static inline Window* root_window()
        {
            auto tree{ engine::scene_tree() };
            if (tree != nullptr) [[likely]]
                return tree->get_root();
            return nullptr;
        }

        static inline Node* root_node()
        {
            return Object::cast_to<Node>(engine::root_window());
        }

        const static inline int32_t max_fps()
        {
            auto engine{ Engine::get_singleton() };
            if (engine != nullptr) [[likely]]
                return engine->get_max_fps();

            log::error("Failed to get max fps");
        }

        static inline void set_fps(const uint32_t fps)
        {
            auto engine{ Engine::get_singleton() };
            if (engine != nullptr) [[likely]]
                return engine->set_max_fps(fps);

            log::error("Failed to set max fps");
        }
    }
}
