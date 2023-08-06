#pragma once

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

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

    namespace debug::io
    {
        // [UtilityFunctions::print_verbose] print verbose messages
        [[msvc::flatten]]
        const static inline void verbose(const char* const msg)
        {
            godot::UtilityFunctions::print_verbose(msg);
        }

        // [UtilityFunctions::print] print info messages
        [[msvc::flatten]]
        const static inline void info(const char* const msg)
        {
            godot::UtilityFunctions::print(msg);
        }

        // [WARN_PRINT_ED]: print warning messages
        [[msvc::flatten]]
        const static inline void warning(const char* const msg)
        {
            WARN_PRINT_ED(msg);
        }

        // [ERR_PRINT_ED]: print error message
        [[msvc::flatten]]
        const static inline void error(const char* const msg)
        {
            ERR_PRINT_ED(msg);
        }

        // [DEV_ASSERT] print condition and break debugger when cond is false.
        // calls to this function and the consitions are replaced with (void(0))
        // when building in release mode.
        [[msvc::flatten]]
        const static inline void assert(const bool cond)
        {
            DEV_ASSERT(cond);
        }
    }

    namespace project
    {
        constexpr static inline auto project_path(const String& rel_path) -> String&&
        {
            return std::move("res://" + rel_path);
        }

        constexpr static inline auto user_data_path(const String& rel_path) -> String&&
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
            DEV_ASSERT(edited_root != nullptr);
            return edited_root;
        }
    }

    namespace input
    {
        const static inline void load_project_inputs()
        {
            InputMap* const input_map{ InputMap::get_singleton() };
            input_map->load_from_project_settings();
        }

    }

    namespace editor
    {
        const static inline bool active()
        {
            return Engine::get_singleton()->is_editor_hint();
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
            static Engine* loop{ Engine::get_singleton() };
            if (loop != nullptr) [[likely]]
                return loop->get_main_loop();
            return nullptr;
        }

        static inline SceneTree* scene_tree()
        {
            static MainLoop* loop{ engine::main_loop() };
            if (loop != nullptr) [[likely]]
                return Object::cast_to<SceneTree>(loop);
            return nullptr;
        }

        static inline Window* root_window()
        {
            static SceneTree* tree{ engine::scene_tree() };
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
            return Engine::get_singleton()->get_max_fps();
        }

        static inline void set_fps(const uint32_t fps)
        {
            return Engine::get_singleton()->set_max_fps(fps);
        }
    }
}
