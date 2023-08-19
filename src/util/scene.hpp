#pragma once

#include "util/debug.hpp"

#include <concepts>
#include <type_traits>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

namespace rl::inline utils
{
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
