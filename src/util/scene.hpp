#pragma once

#include <concepts>

#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include "core/assert.hpp"
#include "util/conversions.hpp"

namespace rl
{
    class Character;
}

namespace rl::inline utils
{
    namespace scene::node
    {
        template <typename TNode>
            requires std::derived_from<TNode, godot::Node>
        static inline void set_unique_name(TNode* node, const char* name)
        {
            runtime_assert(node != nullptr);
            node->set_name(name);
            node->set_unique_name_in_owner(true);
        }
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
                using scene_t = TScene;
                using object_t = TObj;

            public:
                scene(const char* resource_path)
                {
                    auto fh{ godot::FileAccess::open(resource_path, godot::FileAccess::READ) };
                    bool file_access_success{ !fh.is_null() };
                    runtime_assert(file_access_success);

                    if (file_access_success)
                    {
                        godot::ResourceLoader* resource_loader{ resource::loader::get() };
                        m_packed_resource = resource_loader->load(resource_path);
                        initialized = m_packed_resource.is_valid();
                    }
                }

                template <typename... TArgs>
                [[nodiscard]] auto instantiate(TArgs... args) -> TObj*
                {
                    assertion(initialized, "instantiation invoked from uninitialized scene loader");
                    if (!initialized) [[unlikely]]
                        return nullptr;

                    TObj* obj{ gdcast<TObj>(m_packed_resource->instantiate()) };
                    runtime_assert(obj != nullptr);

                    if constexpr (std::is_base_of_v<Character, object_t>)
                    {
                        if (obj != nullptr)
                            obj->set_controller(args...);
                    }

                    return obj;
                }

            private:
                godot::Ref<scene_t> m_packed_resource{};
                bool initialized{ false };
            };
        }
    }
}
