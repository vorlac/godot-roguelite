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

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "servers/sg_physics_2d_server.h"
#include "sg_collision_shape_2d.h"

void SGCollisionShape2D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_shape", "shape"), &SGCollisionShape2D::set_shape);
    ClassDB::bind_method(D_METHOD("get_shape"), &SGCollisionShape2D::get_shape);

    ClassDB::bind_method(D_METHOD("set_disabled", "disabled"), &SGCollisionShape2D::set_disabled);
    ClassDB::bind_method(D_METHOD("get_disabled"), &SGCollisionShape2D::get_disabled);

    ClassDB::bind_method(D_METHOD("set_debug_color", "debug_color"),
                         &SGCollisionShape2D::set_debug_color);
    ClassDB::bind_method(D_METHOD("get_debug_color"), &SGCollisionShape2D::get_debug_color);

    ClassDB::bind_method(D_METHOD("get_rid"), &SGCollisionShape2D::get_rid);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "SGShape2D"),
                 "set_shape", "get_shape");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "disabled"), "set_disabled", "get_disabled");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "debug_color"), "set_debug_color", "get_debug_color");

    ClassDB::bind_method(D_METHOD("_shape_changed"), &SGCollisionShape2D::_shape_changed);
}

void SGCollisionShape2D::_notification(int p_what)
{
    switch (p_what)
    {
        case NOTIFICATION_DRAW:
        {
            if (!Engine::get_singleton()->is_editor_hint() &&
                !get_tree()->is_debugging_collisions_hint())
            {
                break;
            }

            if (!shape.is_valid())
                break;

            // @todo Get access to the debug collision color from GDExtension
            // Color draw_col = get_tree()->get_debug_collisions_color();
            Color draw_col = debug_color;
            if (disabled)
            {
                float g = draw_col.get_v();
                draw_col.r = g;
                draw_col.g = g;
                draw_col.b = g;
                draw_col.a *= 0.5;
            }

            shape->draw(get_canvas_item(), draw_col);
        }
        break;

        case NOTIFICATION_PARENTED:
        {
            SGCollisionObject2D* parent_node = Object::cast_to<SGCollisionObject2D>(get_parent());
            if (parent_node)
                collision_object_rid = parent_node->get_rid();
            if (collision_object_rid.is_valid() && rid.is_valid() && !disabled)
            {
                SGPhysics2DServer::get_singleton()->collision_object_add_shape(collision_object_rid,
                                                                               rid);
            }
        }
        break;

        case NOTIFICATION_UNPARENTED:
        {
            if (collision_object_rid.is_valid() && rid.is_valid() && !disabled)
            {
                SGPhysics2DServer::get_singleton()->collision_object_remove_shape(
                    collision_object_rid, rid);
            }
            collision_object_rid = RID();
        }
        break;
    }
}

void SGCollisionShape2D::set_disabled(bool p_disabled)
{
    if (disabled != p_disabled)
    {
        disabled = p_disabled;
        queue_redraw();
        if (collision_object_rid.is_valid() && rid.is_valid())
        {
            if (disabled)
            {
                SGPhysics2DServer::get_singleton()->collision_object_remove_shape(
                    collision_object_rid, rid);
            }
            else
            {
                SGPhysics2DServer::get_singleton()->collision_object_add_shape(collision_object_rid,
                                                                               rid);
            }
        }
    }
}

bool SGCollisionShape2D::get_disabled() const
{
    return disabled;
}

void SGCollisionShape2D::set_shape(const Ref<SGShape2D>& p_shape)
{
    SGPhysics2DServer* physics_server = SGPhysics2DServer::get_singleton();

    if (shape.is_valid())
    {
        shape->disconnect("changed", Callable(this, "_shape_changed"));
        if (collision_object_rid.is_valid() && rid.is_valid() && !disabled)
        {
            physics_server->collision_object_remove_shape(collision_object_rid, rid);
            physics_server->free_rid(rid);
            rid = RID();
        }
    }

    shape = p_shape;

    if (shape.is_valid())
    {
        shape->connect("changed", Callable(this, "_shape_changed"));
        rid = shape->create_internal_shape();
        physics_server->shape_set_data_ptr(rid, this);
        if (collision_object_rid.is_valid() && !disabled)
            physics_server->collision_object_add_shape(collision_object_rid, rid);
    }

    queue_redraw();
}

Ref<SGShape2D> SGCollisionShape2D::get_shape()
{
    return shape;
}

void SGCollisionShape2D::_shape_changed()
{
    queue_redraw();
}

void SGCollisionShape2D::set_debug_color(const Color& p_color)
{
    debug_color = p_color;
    queue_redraw();
}

Color SGCollisionShape2D::get_debug_color() const
{
    return debug_color;
}

void SGCollisionShape2D::sync_to_physics_engine() const
{
    if (shape.is_valid() && rid.is_valid() && !disabled)
    {
        SGPhysics2DServer::get_singleton()->shape_set_transform(rid, get_fixed_transform());
        shape->sync_to_physics_engine(rid);
    }
}

SGCollisionShape2D::SGCollisionShape2D()
{
    disabled = false;
}

SGCollisionShape2D::~SGCollisionShape2D()
{
    if (collision_object_rid.is_valid() && rid.is_valid() && !disabled)
    {
        SGPhysics2DServer::get_singleton()->collision_object_remove_shape(collision_object_rid, rid);
    }
    if (rid.is_valid())
        SGPhysics2DServer::get_singleton()->free_rid(rid);
}
