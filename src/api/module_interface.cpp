#include "api/module_interface.hpp"

#include "core/level_loader.hpp"
#include "core/projectile_spawner.hpp"
#include "main.hpp"
#include "nodes/camera.hpp"
#include "nodes/character.hpp"
#include "ui/main_interface_diag.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

namespace godot
{
    void initialize_extension_module(ModuleInitializationLevel init_level)
    {
        if (init_level != MODULE_INITIALIZATION_LEVEL_SCENE)
            return;

        ClassDB::register_class<rl::MainInterfaceDiag>();
        ClassDB::register_class<rl::Camera>();
        ClassDB::register_class<rl::Projectile>();
        ClassDB::register_class<rl::ProjectileSpawner>();
        ClassDB::register_class<rl::Character>();
        ClassDB::register_class<rl::LevelLoader>();
        ClassDB::register_class<rl::Main>();
    }

    void uninitialize_extension_module(ModuleInitializationLevel init_level)
    {
        if (init_level != MODULE_INITIALIZATION_LEVEL_SCENE)
            return;
    }

    extern "C"
    {
        GDExtensionBool GDE_EXPORT extension_library_init(GDExtensionInterfaceGetProcAddress addr,
                                                          GDExtensionClassLibraryPtr lib,
                                                          GDExtensionInitialization* init)
        {
            auto init_level{ MODULE_INITIALIZATION_LEVEL_SCENE };
            GDExtensionBinding::InitObject init_obj(addr, lib, init);

            init_obj.register_initializer(initialize_extension_module);
            init_obj.register_terminator(uninitialize_extension_module);
            init_obj.set_minimum_library_initialization_level(init_level);

            return init_obj.init();
        }
    }
}
