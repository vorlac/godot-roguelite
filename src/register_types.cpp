#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "gdexample.hpp"
#include "register_types.hpp"
#include "settings/settings.hpp"

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level)
{
    godot::UtilityFunctions::print("LOG\n", p_level);

    switch (p_level)
    {
        default:
        case MODULE_INITIALIZATION_LEVEL_CORE:
        case MODULE_INITIALIZATION_LEVEL_SERVERS:
        case MODULE_INITIALIZATION_LEVEL_MAX:
        case MODULE_INITIALIZATION_LEVEL_EDITOR:
            break;

        case MODULE_INITIALIZATION_LEVEL_SCENE:
            godot::UtilityFunctions::print("add settings\n");
            // Add our settings
            gdWui::initLibrary();
            godot::UtilityFunctions::print("add classes\n");
            ClassDB::register_class<GDExample>();
    }
}

void uninitialize_example_module(ModuleInitializationLevel p_level)
{
}

extern "C"
{
    // Initialization.
    GDExtensionBool GDE_EXPORT example_library_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library,
                                                       r_initialization);

        init_obj.register_initializer(initialize_example_module);
        init_obj.register_terminator(uninitialize_example_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_CORE);

        return init_obj.init();
    }
}
