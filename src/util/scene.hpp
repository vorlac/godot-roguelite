#pragma once

#include "util/conversions.hpp"

#include <concepts>

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

namespace rl::inline utils
{
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
            godot::Node* root_node{ gdcast<godot::Node>(root_window) };
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
                    TObj* ret{ gdcast<TObj>(m_packed_resource->instantiate()) };
                    return ret;
                }

            private:
                godot::Ref<TScene> m_packed_resource{};
            };
        }
    }
}
