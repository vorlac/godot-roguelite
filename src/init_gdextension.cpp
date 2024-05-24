/*************************************************************************/
/* Copyright (c) 2021-2022 David Snopek                                  */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "internal/sg_world_2d_internal.h"
#include "math/sg_fixed_rect2.h"
#include "math/sg_fixed_singleton.h"
#include "math/sg_fixed_transform_2d.h"
#include "math/sg_fixed_vector2.h"
#include "scene/2d/sg_area_2d.h"
#include "scene/2d/sg_character_body_2d.h"
#include "scene/2d/sg_collision_polygon_2d.h"
#include "scene/2d/sg_collision_shape_2d.h"
#include "scene/2d/sg_fixed_position_2d.h"
#include "scene/2d/sg_ray_cast_2d.h"
#include "scene/2d/sg_static_body_2d.h"
#include "scene/resources/sg_shapes_2d.h"
#include "servers/sg_physics_2d_server.h"

#if defined(TOOLS_ENABLED) || defined(DEBUG_ENABLED)
  #include "./editor/sg_collision_polygon_2d_editor_plugin.h"
  #include "./editor/sg_collision_shape_2d_editor_plugin.h"
#endif

using namespace godot;

static SGFixed* fixed_singleton;
static SGPhysics2DServer* server_singleton;

void register_sg_physics_2d_extension_types(ModuleInitializationLevel p_level)
{
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        GDREGISTER_CLASS(SGFixed);
        GDREGISTER_CLASS(SGFixedVector2);
        GDREGISTER_CLASS(SGFixedRect2);
        GDREGISTER_CLASS(SGFixedTransform2D);

        GDREGISTER_CLASS(SGFixedNode2D);
        GDREGISTER_CLASS(SGFixedPosition2D);
        GDREGISTER_CLASS(SGPhysics2DServer);
        GDREGISTER_VIRTUAL_CLASS(SGCollisionObject2D);
        GDREGISTER_CLASS(SGArea2D);
        GDREGISTER_CLASS(SGAreaCollision2D);
        GDREGISTER_CLASS(SGPhysicsBody2D);
        GDREGISTER_CLASS(SGStaticBody2D);
        GDREGISTER_CLASS(SGCharacterBody2D);
        GDREGISTER_CLASS(SGKinematicCollision2D);
        GDREGISTER_CLASS(SGRayCast2D);
        GDREGISTER_CLASS(SGRayCastCollision2D);

        GDREGISTER_CLASS(SGCollisionShape2D);
        GDREGISTER_CLASS(SGCollisionPolygon2D);

        GDREGISTER_VIRTUAL_CLASS(SGShape2D);
        GDREGISTER_CLASS(SGRectangleShape2D);
        GDREGISTER_CLASS(SGCircleShape2D);
        GDREGISTER_CLASS(SGCapsuleShape2D);

        fixed_singleton = memnew(SGFixed);
        Engine::get_singleton()->register_singleton("SGFixed", SGFixed::get_singleton());

        server_singleton = memnew(SGPhysics2DServer);
        Engine::get_singleton()->register_singleton("SGPhysics2DServer",
                                                    SGPhysics2DServer::get_singleton());
    }

#if defined(TOOLS_ENABLED) || defined(DEBUG_ENABLED)
    if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
    {
        GDREGISTER_CLASS(SGCollisionShape2DEditorPlugin);
        GDREGISTER_CLASS(SGCollisionPolygon2DEditor);
        GDREGISTER_CLASS(SGCollisionPolygon2DEditorPlugin);

        EditorPlugins::add_by_type<SGCollisionShape2DEditorPlugin>();
        EditorPlugins::add_by_type<SGCollisionPolygon2DEditorPlugin>();
    }
#endif
}

void unregister_sg_physics_2d_extension_types(ModuleInitializationLevel p_level)
{
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        Engine::get_singleton()->unregister_singleton("SGFixed");
        memdelete(fixed_singleton);

        Engine::get_singleton()->unregister_singleton("SGPhysics2DServer");
        memdelete(server_singleton);
    }

#if defined(TOOLS_ENABLED) || defined(DEBUG_ENABLED)
    if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
        EditorPlugins::remove_by_type<SGCollisionShape2DEditorPlugin>();
#endif
}

extern "C"
{
    GDExtensionBool GDE_EXPORT sg_physics_2d_extension_init(
        GDExtensionInterfaceGetProcAddress p_interface, const GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization* r_initialization)
    {
        GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

        init_obj.register_initializer(register_sg_physics_2d_extension_types);
        init_obj.register_terminator(unregister_sg_physics_2d_extension_types);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
