#include "api/module_interface.hpp"

#include "core/level.hpp"
#include "core/projectile_spawner.hpp"
#include "main.hpp"
#include "nodes/camera.hpp"
#include "nodes/character.hpp"
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

    void initialize_singletons()
    {
        console_singleton = memnew(rl::Console<RichTextLabel>);
        rl::engine::get()->register_singleton("Console", rl::Console<RichTextLabel>::get());
    }

    void teardown_singletons()
    {
        rl::engine::get()->unregister_singleton("Console");
        memdelete(console_singleton);
    }

    void initialize_extension_module(ModuleInitializationLevel init_level)
    {
        if (init_level != MODULE_INITIALIZATION_LEVEL_SCENE)
            return;

        ClassDB::register_class<rl::MainDialog>();
        ClassDB::register_class<rl::Camera>();
        ClassDB::register_class<rl::Projectile>();
        ClassDB::register_class<rl::ProjectileSpawner>();
        ClassDB::register_class<rl::Character>();
        ClassDB::register_class<rl::Level>();
        ClassDB::register_class<rl::Main>();
        ClassDB::register_class<rl::Console<RichTextLabel>>();

        initialize_singletons();
    }

    void uninitialize_extension_module(ModuleInitializationLevel init_level)
    {
        if (init_level != MODULE_INITIALIZATION_LEVEL_SCENE)
            return;

        teardown_singletons();
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
