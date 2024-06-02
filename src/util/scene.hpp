#pragma once

#include <concepts>

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include "core/assert.hpp"
#include "util/conversions.hpp"

namespace rl::inline utils
{
    namespace scene
    {
        namespace node
        {
            template <typename TNode>
                requires std::derived_from<TNode, godot::Node>
            static inline void set_unique_name(TNode* node, const char* name)
            {
                runtime_assert(node != nullptr);
                node->set_name(name);
                node->set_unique_name_in_owner(true);
            }

            /** Sets the owner of a node and all it's children. */
            template <typename TNodeA, typename TNodeB>
                requires std::derived_from<TNodeB, godot::Node> &&
                         std::derived_from<TNodeA, godot::Node>
            static inline void set_owner(TNodeA* node, TNodeB* owner)
            {
                runtime_assert(node != nullptr && owner != nullptr);
                const int node_child_count = node->get_child_count();
                for (int i = 0; i < node_child_count; ++i)
                {
                    auto child = node->get_child(i);
                    child->set_owner(owner);
                    set_owner(child, owner);
                }
            }
        }

        namespace tree
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

        namespace packer
        {
            /**
             @return PackedScene from godot::Node parameter.
             */
            template <typename TNode>
                requires std::derived_from<TNode, godot::Node>
            static inline godot::PackedScene* pack(TNode* node)
            {
                node::set_owner<TNode, TNode>(node, node);
                godot::PackedScene* package = memnew(godot::PackedScene);
                package->pack(node);
                return package;
            }
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

        namespace saver
        {
            static inline godot::ResourceSaver* get()
            {
                return godot::ResourceSaver::get_singleton();
            }
        }

        namespace preload
        {
            template <typename TObj, typename TScene = godot::PackedScene>
                requires std::derived_from<TScene, godot::Resource> &&
                         std::convertible_to<TObj, godot::Object>
            class packed_scene
            {
            public:
                using scene_t = TScene;
                using object_t = TObj;

                /* Pack as preload from path. */
                packed_scene(godot::String resource_load_path)
                {
                    godot::ResourceLoader* resource_loader{ resource::loader::get() };

                    bool file_access_success{ resource_loader->exists(resource_load_path) };
                    runtime_assert(file_access_success);

                    if (file_access_success)
                    {
                        m_packed_resource = resource_loader->load(resource_load_path);
                        initialized = m_packed_resource.is_valid();
                    }
                }

                /* Pack from existing instance. */
                packed_scene(godot::Node* node)
                {
                    m_packed_resource = scene::packer::pack(node);
                    initialized = m_packed_resource.is_valid();
                }

                [[nodiscard]] auto instantiate() -> object_t*
                {
                    assertion(initialized,
                              "Resource instantiation invoked from uninitialized scene loader.");
                    if (!initialized) [[unlikely]]
                        return nullptr;

                    object_t* obj{ gdcast<object_t>(m_packed_resource->instantiate()) };
                    runtime_assert(obj != nullptr);

                    return obj;
                }

                /** Save this resource to specified path. */
                void save(godot::String& resource_save_path)
                {
                    if (initialized)
                    {
                        auto error = saver::get()->save(m_packed_resource, resource_save_path);
                        assertion(error != godot::Error::OK, "Packed resource save failed.");
                    }
                }

            private:
                godot::Ref<scene_t> m_packed_resource{};
                bool initialized{ false };
            };
        }
    }
}
