#pragma once

#include "util/debug.hpp"

#include <array>
#include <concepts>
#include <fmt/compile.h>
#include <fmt/format.h>
#include <type_traits>
#include <utility>

#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

namespace rl::inline utils
{

    enum class Translation
    {
        Notification
    };

    template <Translation T>
        requires(T == Translation::Notification)
    static constexpr inline std::string_view to_string(int notification)
    {
        using godot::CanvasItem;
        using godot::Control;
        using godot::Node;
        using godot::Object;
        static constexpr std::array notification_map =
            std::to_array<std::pair<int, std::string_view>>({
                { Object::NOTIFICATION_POSTINITIALIZE, "Object::POSTINITIALIZE" },
                { Object::NOTIFICATION_PREDELETE, "Object::PREDELETE" },
                { Node::NOTIFICATION_ENTER_TREE, "Node::ENTER_TREE" },
                { Node::NOTIFICATION_EXIT_TREE, "Node::EXIT_TREE" },
                { Node::NOTIFICATION_MOVED_IN_PARENT, "Node::MOVED_IN_PARENT" },
                { Node::NOTIFICATION_READY, "Node::READY" },
                { Node::NOTIFICATION_PAUSED, "Node::PAUSED" },
                { Node::NOTIFICATION_UNPAUSED, "Node::UNPAUSED" },
                { Node::NOTIFICATION_PHYSICS_PROCESS, "Node::PHYSICS_PROCESS" },
                { Node::NOTIFICATION_PROCESS, "Node::PROCESS" },
                { Node::NOTIFICATION_PARENTED, "Node::PARENTED" },
                { Node::NOTIFICATION_UNPARENTED, "Node::UNPARENTED" },
                { Node::NOTIFICATION_SCENE_INSTANTIATED, "Node::SCENE_INSTANTIATED" },
                { Node::NOTIFICATION_DRAG_BEGIN, "Node::DRAG_BEGIN" },
                { Node::NOTIFICATION_DRAG_END, "Node::DRAG_END" },
                { Node::NOTIFICATION_PATH_RENAMED, "Node::PATH_RENAMED" },
                { Node::NOTIFICATION_CHILD_ORDER_CHANGED, "Node::CHILD_ORDER_CHANGED" },
                { Node::NOTIFICATION_INTERNAL_PROCESS, "Node::INTERNAL_PROCESS" },
                { Node::NOTIFICATION_INTERNAL_PHYSICS_PROCESS, "Node::INTERNAL_PHYSICS_PROCESS" },
                { Node::NOTIFICATION_POST_ENTER_TREE, "Node::POST_ENTER_TREE" },
                { Node::NOTIFICATION_DISABLED, "Node::DISABLED" },
                { Node::NOTIFICATION_ENABLED, "Node::ENABLED" },
                { Node::NOTIFICATION_NODE_RECACHE_REQUESTED, "Node::NODE_RECACHE_REQUESTED" },
                { Node::NOTIFICATION_EDITOR_PRE_SAVE, "Node::EDITOR_PRE_SAVE" },
                { Node::NOTIFICATION_EDITOR_POST_SAVE, "Node::EDITOR_POST_SAVE" },
                { Node::NOTIFICATION_WM_MOUSE_ENTER, "Node::WM_MOUSE_ENTER" },
                { Node::NOTIFICATION_WM_MOUSE_EXIT, "Node::WM_MOUSE_EXIT" },
                { Node::NOTIFICATION_WM_WINDOW_FOCUS_IN, "Node::WM_WINDOW_FOCUS_IN" },
                { Node::NOTIFICATION_WM_WINDOW_FOCUS_OUT, "Node::WM_WINDOW_FOCUS_OUT" },
                { Node::NOTIFICATION_WM_CLOSE_REQUEST, "Node::WM_CLOSE_REQUEST" },
                { Node::NOTIFICATION_WM_GO_BACK_REQUEST, "Node::WM_GO_BACK_REQUEST" },
                { Node::NOTIFICATION_WM_SIZE_CHANGED, "Node::WM_SIZE_CHANGED" },
                { Node::NOTIFICATION_WM_DPI_CHANGE, "Node::WM_DPI_CHANGE" },
                { Node::NOTIFICATION_VP_MOUSE_ENTER, "Node::VP_MOUSE_ENTER" },
                { Node::NOTIFICATION_VP_MOUSE_EXIT, "Node::VP_MOUSE_EXIT" },
                { Node::NOTIFICATION_OS_MEMORY_WARNING, "Node::OS_MEMORY_WARNING" },
                { Node::NOTIFICATION_TRANSLATION_CHANGED, "Node::TRANSLATION_CHANGED" },
                { Node::NOTIFICATION_WM_ABOUT, "Node::WM_ABOUT" },
                { Node::NOTIFICATION_CRASH, "Node::CRASH" },
                { Node::NOTIFICATION_OS_IME_UPDATE, "Node::OS_IME_UPDATE" },
                { Node::NOTIFICATION_APPLICATION_RESUMED, "Node::APPLICATION_RESUMED" },
                { Node::NOTIFICATION_APPLICATION_PAUSED, "Node::APPLICATION_PAUSED" },
                { Node::NOTIFICATION_APPLICATION_FOCUS_IN, "Node::APPLICATION_FOCUS_IN" },
                { Node::NOTIFICATION_APPLICATION_FOCUS_OUT, "Node::APPLICATION_FOCUS_OUT" },
                { Node::NOTIFICATION_TEXT_SERVER_CHANGED, "Node::TEXT_SERVER_CHANGED" },
                { Control::NOTIFICATION_RESIZED, "Control::RESIZED" },
                { Control::NOTIFICATION_MOUSE_ENTER, "Control::MOUSE_ENTER" },
                { Control::NOTIFICATION_MOUSE_EXIT, "Control::MOUSE_EXIT" },
                { Control::NOTIFICATION_FOCUS_ENTER, "Control::FOCUS_ENTER" },
                { Control::NOTIFICATION_FOCUS_EXIT, "Control::FOCUS_EXIT" },
                { Control::NOTIFICATION_THEME_CHANGED, "Control::THEME_CHANGED" },
                { Control::NOTIFICATION_SCROLL_BEGIN, "Control::SCROLL_BEGIN" },
                { Control::NOTIFICATION_SCROLL_END, "Control::SCROLL_END" },
                { Control::NOTIFICATION_LAYOUT_DIRECTION_CHANGED,
                  "Control::LAYOUT_DIRECTION_CHANGED" },
                { CanvasItem::NOTIFICATION_TRANSFORM_CHANGED, "CanvasItem::TRANSFORM_CHANGED" },
                { CanvasItem::NOTIFICATION_DRAW, "CanvasItem::DRAW" },
                { CanvasItem::NOTIFICATION_VISIBILITY_CHANGED, "CanvasItem::VISIBILITY_CHANGED" },
                { CanvasItem::NOTIFICATION_ENTER_CANVAS, "CanvasItem::ENTER_CANVAS" },
                { CanvasItem::NOTIFICATION_EXIT_CANVAS, "CanvasItem::EXIT_CANVAS" },
                { CanvasItem::NOTIFICATION_LOCAL_TRANSFORM_CHANGED,
                  "CanvasItem::LOCAL_TRANSFORM_CHANGED" },
                { CanvasItem::NOTIFICATION_WORLD_2D_CHANGED, "CanvasItem::WORLD_2D_CHANGED" },
            });

        for (auto&& nm : notification_map)
            if (nm.first == notification)
                return nm.second;

        return fmt::to_string(fmt::format("Unknown ({})", notification));
    };

    template <typename TOut, typename TIn = godot::Node>
        requires std::derived_from<TOut, TIn>
    static inline constexpr TOut* const convert(TIn* const node)
    {
        debug::assert(node != nullptr);
        TOut* const ret{ godot::Object::cast_to<TOut>(node) };
        debug::assert(ret != nullptr);
        return ret;
    }

    namespace scene::tree
    {
        template <typename TNode>
            requires std::derived_from<TNode, godot::Node>
        static inline godot::SceneTree* get(TNode* node)
        {
            debug::assert(node != nullptr);
            godot::SceneTree* scene_tree{ node->get_tree() };
            debug::assert(scene_tree != nullptr);
            return scene_tree;
        }

        template <typename TNode>
            requires std::derived_from<TNode, godot::Node>
        static inline godot::Node* edited_root(TNode* node)
        {
            debug::assert(node != nullptr);
            godot::Node* edited_root{ node->get_tree()->get_edited_scene_root() };
            debug::assert(edited_root != nullptr);
            return edited_root;
        }

        template <typename TNode, typename TRootNode = godot::Node>
            requires std::derived_from<TNode, godot::Node>
        static inline TRootNode* const root_node(TNode* node)
        {
            godot::SceneTree* scene_tree{ tree::get(node) };
            debug::assert(scene_tree != nullptr);

            godot::Window* root_window{ scene_tree->get_root() };
            debug::assert(root_window != nullptr);

            TRootNode* root_node{ godot::Object::cast_to<TRootNode>(root_window) };
            debug::assert(root_node != nullptr);
            return root_node;
        }
    }

    namespace path
    {
        namespace sprite
        {
            inline constexpr auto Player{ "res://assets/art/placeholder/player.svg" };
            inline constexpr auto Enemy{ "res://assets/art/placeholder/enemy.svg" };
            inline constexpr auto Bullet{ "res://assets/art/placeholder/bullet.svg" };
            inline constexpr auto Background{ "res://assets/art/placeholder/background.png" };
        }

        namespace scene
        {
            inline constexpr auto Bullet{ "res://assets/scenes/projectiles/bullet.tscn" };
        }
    }

    namespace resource
    {
        namespace loader
        {
            static inline godot::ResourceLoader* const get()
            {
                return godot::ResourceLoader::get_singleton();
            }
        }

        namespace preload
        {
            template <typename TObj, typename TScene = godot::PackedScene>
                requires std::derived_from<TScene, godot::Resource> &&
                         std::convertible_to<TObj, godot::Node>
            struct scene
            {
                using scene_t = typename std::type_identity_t<TScene>;
                using object_t = typename std::type_identity_t<TObj>;

                scene(const char* const resource_path)
                {
                    godot::ResourceLoader* const resource_loader{ resource::loader::get() };
                    debug::assert(resource_loader != nullptr);
                    m_packed_resource = resource_loader->load(resource_path);
                }

                [[nodiscard]]
                inline auto instantiate() -> TObj*
                {
                    debug::assert(m_packed_resource.is_valid());
                    TObj* ret{ godot::Object::cast_to<TObj>(m_packed_resource->instantiate()) };
                    debug::assert(ret != nullptr);
                    return ret;
                }

                godot::Ref<TScene> m_packed_resource{};
            };
        }
    }
}
