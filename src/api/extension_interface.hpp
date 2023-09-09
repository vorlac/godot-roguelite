#pragma once

#include <godot_cpp/core/class_db.hpp>

namespace godot
{
    void initialize_static_objects();
    void teardown_static_objects();
    void initialize_extension_module(ModuleInitializationLevel init_level);
    void uninitialize_extension_module(ModuleInitializationLevel init_level);

    extern "C"
    {
        GDExtensionBool GDE_EXPORT extension_library_init(GDExtensionInterfaceGetProcAddress addr,
                                                          GDExtensionClassLibraryPtr lib,
                                                          GDExtensionInitialization* init);
    }
}
