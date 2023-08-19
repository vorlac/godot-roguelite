#pragma once

#include "util/debug.hpp"

#include <concepts>
#include <type_traits>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

namespace rl::inline utils
{

    namespace scene
    {
        template <typename TNode = godot::Node>
            requires std::derived_from<TNode, godot::Node>
        static inline godot::Node* edited_root(TNode* node)
        {
            godot::Node* edited_root{ node->get_tree()->get_edited_scene_root() };
            debug::assert(edited_root != nullptr);
            return edited_root;
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
