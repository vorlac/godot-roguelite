#include "api/extension_interface.hpp"

#include "main.hpp"
#include "nodes/camera.hpp"
#include "nodes/character.hpp"
#include "nodes/level.hpp"
#include "nodes/player.hpp"
#include "nodes/player_controller.hpp"
#include "nodes/projectile_spawner.hpp"
#include "singletons/console.hpp"
#include "ui/main_dialog.hpp"
#include "util/engine.hpp"

#include <type_traits>

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/variant/string_name.hpp>

namespace godot
{
    static inline rl::Console<RichTextLabel>* console_singleton{ nullptr };

    void initialize_static_objects()
    {
        console_singleton = memnew(rl::Console<RichTextLabel>);
        rl::engine::get()->register_singleton("Console", rl::Console<RichTextLabel>::get());
    }

    void teardown_static_objects()
    {
        rl::engine::get()->unregister_singleton("Console");
        memdelete(console_singleton);
    }

    void initialize_extension_module(ModuleInitializationLevel init_level)
    {
        if (init_level != MODULE_INITIALIZATION_LEVEL_SCENE)
            return;

        ClassDB::register_class<rl::Console<RichTextLabel>>();
        ClassDB::register_class<rl::Projectile>();
        ClassDB::register_class<rl::ProjectileSpawner>();
        ClassDB::register_class<rl::PlayerController>();
        ClassDB::register_class<rl::Camera>();
        ClassDB::register_class<rl::Character>();
        ClassDB::register_class<rl::Player>();
        ClassDB::register_class<rl::Level>();
        ClassDB::register_class<rl::MainDialog>();
        ClassDB::register_class<rl::Main>();

        initialize_static_objects();
    }

    void uninitialize_extension_module(ModuleInitializationLevel init_level)
    {
        if (init_level != MODULE_INITIALIZATION_LEVEL_SCENE)
            return;

        teardown_static_objects();
    }

    extern "C"
    {
        GDExtensionBool GDE_EXPORT extension_library_init(GDExtensionInterfaceGetProcAddress addr,
                                                          GDExtensionClassLibraryPtr lib,
                                                          GDExtensionInitialization* init)
        {
            const auto init_level = MODULE_INITIALIZATION_LEVEL_SCENE;
            godot::GDExtensionBinding::InitObject init_obj(addr, lib, init);

            init_obj.register_initializer(initialize_extension_module);
            init_obj.register_terminator(uninitialize_extension_module);
            init_obj.set_minimum_library_initialization_level(init_level);

            return init_obj.init();
        }
    }
}
