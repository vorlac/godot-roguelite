#include "register_types.hpp"

#include "nodes/character.hpp"
#include "nodes/game.hpp"
#include "nodes/level.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

namespace godot
{
    void initialize_roguelite_module(ModuleInitializationLevel p_level)
    {
        if (p_level != ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE)
            return;

        ClassDB::register_class<Game>();
        ClassDB::register_class<Level>();
        ClassDB::register_class<Character>();
    }

    void uninitialize_roguelite_module(ModuleInitializationLevel p_level)
    {
        if (p_level != ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE)
            return;
    }

    extern "C"
    {
        // GDExtension init
        GDExtensionBool GDE_EXPORT roguelite_library_init(GDExtensionInterfaceGetProcAddress addr,
                                                          GDExtensionClassLibraryPtr lib,
                                                          GDExtensionInitialization* init)
        {
            GDExtensionBinding::InitObject init_obj(addr, lib, init);
            init_obj.register_initializer(initialize_roguelite_module);
            init_obj.register_terminator(uninitialize_roguelite_module);
            init_obj.set_minimum_library_initialization_level(
                ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

            return init_obj.init();
        }
    }
}
