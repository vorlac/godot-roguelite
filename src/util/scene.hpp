#pragma once

#include <array>
#include <concepts>
#include <fmt/format.h>
#include <string_view>
#include <type_traits>
#include <utility>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

namespace rl::inline utils
{
    template <typename TOut>
    static inline constexpr TOut* as(godot::Object* obj)
    {
        return godot::Object::cast_to<TOut>(obj);
    }

    const static inline auto str_map =
        std::to_array<typename std::type_identity<std::pair<int, std::string_view>>::type>({
            { godot::Object::NOTIFICATION_POSTINITIALIZE, "Object::POSTINITIALIZE" },
            { godot::Object::NOTIFICATION_PREDELETE, "Object::PREDELETE" },
            { godot::Node::NOTIFICATION_ENTER_TREE, "Node::ENTER_TREE" },
            { godot::Node::NOTIFICATION_EXIT_TREE, "Node::EXIT_TREE" },
            { godot::Node::NOTIFICATION_MOVED_IN_PARENT, "Node::MOVED_IN_PARENT" },
            { godot::Node::NOTIFICATION_READY, "Node::READY" },
            { godot::Node::NOTIFICATION_PAUSED, "Node::PAUSED" },
            { godot::Node::NOTIFICATION_UNPAUSED, "Node::UNPAUSED" },
            { godot::Node::NOTIFICATION_PHYSICS_PROCESS, "Node::PHYSICS_PROCESS" },
            { godot::Node::NOTIFICATION_PROCESS, "Node::PROCESS" },
            { godot::Node::NOTIFICATION_PARENTED, "Node::PARENTED" },
            { godot::Node::NOTIFICATION_UNPARENTED, "Node::UNPARENTED" },
            { godot::Node::NOTIFICATION_SCENE_INSTANTIATED, "Node::SCENE_INSTANTIATED" },
            { godot::Node::NOTIFICATION_DRAG_BEGIN, "Node::DRAG_BEGIN" },
            { godot::Node::NOTIFICATION_DRAG_END, "Node::DRAG_END" },
            { godot::Node::NOTIFICATION_PATH_RENAMED, "Node::PATH_RENAMED" },
            { godot::Node::NOTIFICATION_CHILD_ORDER_CHANGED, "Node::CHILD_ORDER_CHANGED" },
            { godot::Node::NOTIFICATION_INTERNAL_PROCESS, "Node::INTERNAL_PROCESS" },
            { godot::Node::NOTIFICATION_INTERNAL_PHYSICS_PROCESS,
              "Node::INTERNAL_PHYSICS_PROCESS" },
            { godot::Node::NOTIFICATION_POST_ENTER_TREE, "Node::POST_ENTER_TREE" },
            { godot::Node::NOTIFICATION_DISABLED, "Node::DISABLED" },
            { godot::Node::NOTIFICATION_ENABLED, "Node::ENABLED" },
            { godot::Node::NOTIFICATION_NODE_RECACHE_REQUESTED, "Node::NODE_RECACHE_REQUESTED" },
            { godot::Node::NOTIFICATION_EDITOR_PRE_SAVE, "Node::EDITOR_PRE_SAVE" },
            { godot::Node::NOTIFICATION_EDITOR_POST_SAVE, "Node::EDITOR_POST_SAVE" },
            { godot::Node::NOTIFICATION_WM_MOUSE_ENTER, "Node::WM_MOUSE_ENTER" },
            { godot::Node::NOTIFICATION_WM_MOUSE_EXIT, "Node::WM_MOUSE_EXIT" },
            { godot::Node::NOTIFICATION_WM_WINDOW_FOCUS_IN, "Node::WM_WINDOW_FOCUS_IN" },
            { godot::Node::NOTIFICATION_WM_WINDOW_FOCUS_OUT, "Node::WM_WINDOW_FOCUS_OUT" },
            { godot::Node::NOTIFICATION_WM_CLOSE_REQUEST, "Node::WM_CLOSE_REQUEST" },
            { godot::Node::NOTIFICATION_WM_GO_BACK_REQUEST, "Node::WM_GO_BACK_REQUEST" },
            { godot::Node::NOTIFICATION_WM_SIZE_CHANGED, "Node::WM_SIZE_CHANGED" },
            { godot::Node::NOTIFICATION_WM_DPI_CHANGE, "Node::WM_DPI_CHANGE" },
            { godot::Node::NOTIFICATION_VP_MOUSE_ENTER, "Node::VP_MOUSE_ENTER" },
            { godot::Node::NOTIFICATION_VP_MOUSE_EXIT, "Node::VP_MOUSE_EXIT" },
            { godot::Node::NOTIFICATION_OS_MEMORY_WARNING, "Node::OS_MEMORY_WARNING" },
            { godot::Node::NOTIFICATION_TRANSLATION_CHANGED, "Node::TRANSLATION_CHANGED" },
            { godot::Node::NOTIFICATION_WM_ABOUT, "Node::WM_ABOUT" },
            { godot::Node::NOTIFICATION_CRASH, "Node::CRASH" },
            { godot::Node::NOTIFICATION_OS_IME_UPDATE, "Node::OS_IME_UPDATE" },
            { godot::Node::NOTIFICATION_APPLICATION_RESUMED, "Node::APPLICATION_RESUMED" },
            { godot::Node::NOTIFICATION_APPLICATION_PAUSED, "Node::APPLICATION_PAUSED" },
            { godot::Node::NOTIFICATION_APPLICATION_FOCUS_IN, "Node::APPLICATION_FOCUS_IN" },
            { godot::Node::NOTIFICATION_APPLICATION_FOCUS_OUT, "Node::APPLICATION_FOCUS_OUT" },
            { godot::Node::NOTIFICATION_TEXT_SERVER_CHANGED, "Node::TEXT_SERVER_CHANGED" },
            { godot::Control::NOTIFICATION_RESIZED, "Control::RESIZED" },
            { godot::Control::NOTIFICATION_MOUSE_ENTER, "Control::MOUSE_ENTER" },
            { godot::Control::NOTIFICATION_MOUSE_EXIT, "Control::MOUSE_EXIT" },
            { godot::Control::NOTIFICATION_FOCUS_ENTER, "Control::FOCUS_ENTER" },
            { godot::Control::NOTIFICATION_FOCUS_EXIT, "Control::FOCUS_EXIT" },
            { godot::Control::NOTIFICATION_THEME_CHANGED, "Control::THEME_CHANGED" },
            { godot::Control::NOTIFICATION_SCROLL_BEGIN, "Control::SCROLL_BEGIN" },
            { godot::Control::NOTIFICATION_SCROLL_END, "Control::SCROLL_END" },
            { godot::Control::NOTIFICATION_LAYOUT_DIRECTION_CHANGED,
              "Control::LAYOUT_DIRECTION_CHANGED" },
            { godot::CanvasItem::NOTIFICATION_TRANSFORM_CHANGED, "CanvasItem::TRANSFORM_CHANGED" },
            { godot::CanvasItem::NOTIFICATION_DRAW, "CanvasItem::DRAW" },
            { godot::CanvasItem::NOTIFICATION_VISIBILITY_CHANGED,
              "CanvasItem::VISIBILITY_CHANGED" },
            { godot::CanvasItem::NOTIFICATION_ENTER_CANVAS, "CanvasItem::ENTER_CANVAS" },
            { godot::CanvasItem::NOTIFICATION_EXIT_CANVAS, "CanvasItem::EXIT_CANVAS" },
            { godot::CanvasItem::NOTIFICATION_LOCAL_TRANSFORM_CHANGED,
              "CanvasItem::LOCAL_TRANSFORM_CHANGED" },
            { godot::CanvasItem::NOTIFICATION_WORLD_2D_CHANGED, "CanvasItem::WORLD_2D_CHANGED" },
        });

    constexpr std::string_view to_string(int notification)
    {
        for (auto&& nm : str_map)
            if (nm.first == notification)
                return nm.second;

        return std::string_view(std::move(fmt::format("Unknown ({})", notification)));
    }

    namespace scene::tree
    {
        template <typename TNode>
            requires std::derived_from<TNode, godot::Node>
        static inline godot::SceneTree* get(TNode* node)
        {
            godot::SceneTree* scene_tree{ node->get_tree() };
            return scene_tree;
        }

        template <typename TNode>
            requires std::derived_from<TNode, godot::Node>
        static inline godot::Node* edited_root(TNode* node)
        {
            godot::Node* edited_root{ node->get_tree()->get_edited_scene_root() };
            return edited_root;
        }

        template <typename TNode>
            requires std::derived_from<TNode, godot::Node>
        static inline godot::Node* root_node(TNode* node)
        {
            godot::SceneTree* scene_tree{ tree::get(node) };
            godot::Window* root_window{ scene_tree->get_root() };
            godot::Node* root_node{ rl::as<godot::Node>(root_window) };
            return root_node;
        }
    }

    namespace resource
    {
        namespace loader
        {
            static inline godot::ResourceLoader* get()
            {
                return godot::ResourceLoader::get_singleton();
            }
        }

        namespace preload
        {
            template <typename TObj, typename TScene = godot::PackedScene>
                requires std::derived_from<TScene, godot::Resource> &&
                         std::convertible_to<TObj, godot::Object>
            class scene
            {
            public:
                using scene_t = typename std::type_identity_t<TScene>;
                using object_t = typename std::type_identity_t<TObj>;

            public:
                scene(const char* const resource_path)
                {
                    godot::ResourceLoader* resource_loader{ resource::loader::get() };
                    m_packed_resource = resource_loader->load(resource_path);
                }

                [[nodiscard]]
                auto instantiate() -> TObj*
                {
                    TObj* ret{ rl::as<TObj>(m_packed_resource->instantiate()) };
                    return ret;
                }

            private:
                godot::Ref<TScene> m_packed_resource{};
            };
        }
    }
}
