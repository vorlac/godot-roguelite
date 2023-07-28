#include "register_types.hpp"

#include "character.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

namespace godot
{
    void initialize_roguelite_module(ModuleInitializationLevel p_level)
    {
        if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
            return;

        ClassDB::register_class<Character>();
    }

    void uninitialize_roguelite_module(ModuleInitializationLevel p_level)
    {
        if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
            return;
    }

    extern "C"
    {
        // GDExtension init
        GDExtensionBool GDE_EXPORT roguelite_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                                          GDExtensionClassLibraryPtr p_library,
                                                          GDExtensionInitialization* r_initialization)
        {
            GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

            init_obj.register_initializer(initialize_roguelite_module);
            init_obj.register_terminator(uninitialize_roguelite_module);
            init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

            return init_obj.init();
        }
    }
}
