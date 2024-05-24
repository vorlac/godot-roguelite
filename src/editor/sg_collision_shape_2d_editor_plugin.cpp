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

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include "scene/2d/sg_collision_shape_2d.h"
#include "scene/resources/sg_shapes_2d.h"
#include "sg_collision_shape_2d_editor_plugin.h"

Variant SGCollisionShape2DEditorPlugin::get_handle_value(int idx) const
{
    switch (shape_type)
    {
        case RECTANGLE_SHAPE:
        {
            Ref<SGRectangleShape2D> rectangle = node->get_shape();

            if (idx < 3)
                return rectangle->get_extents()->abs();
        }
        break;

        case CIRCLE_SHAPE:
        {
            Ref<SGCircleShape2D> circle = node->get_shape();

            if (idx == 0)
                return circle->get_radius();
        }
        break;

        case CAPSULE_SHAPE:
        {
            Ref<SGCapsuleShape2D> capsule = node->get_shape();

            if (idx == 0)
                return capsule->get_radius();
            else if (idx == 1)
                return capsule->get_height();
        }
        break;
    }

    return Variant();
}

void SGCollisionShape2DEditorPlugin::set_handle(int idx, Point2& p_point)
{
    switch (shape_type)
    {
        case RECTANGLE_SHAPE:
        {
            Ref<SGRectangleShape2D> rectangle = node->get_shape();

            Ref<SGFixedVector2> extents = rectangle->get_extents();
            switch (idx)
            {
                case 0:
                    extents->set_x(fixed::from_float(p_point.x).value);
                    break;
                case 1:
                    extents->set_y(fixed::from_float(p_point.y).value);
                    break;
                case 2:
                    extents->from_float(p_point);
                    break;
            }
            rectangle->set_extents(extents->abs());

            update_overlays();
        }
        break;

        case CIRCLE_SHAPE:
        {
            Ref<SGCircleShape2D> circle = node->get_shape();
            circle->set_radius(fixed::from_float(p_point.length()).value);

            update_overlays();
        }
        break;

        case CAPSULE_SHAPE:
        {
            Ref<SGCapsuleShape2D> capsule = node->get_shape();
            switch (idx)
            {
                case 0:
                    capsule->set_radius(fixed::from_float(p_point.length()).value);
                    break;
                case 1:
                    capsule->set_height(fixed::from_float(p_point.length()).value);
                    break;
            }

            update_overlays();
        }
        break;
    }
}

void SGCollisionShape2DEditorPlugin::commit_handle(int idx, Variant& p_org)
{
    undo_redo->create_action("Set Handle");

    switch (shape_type)
    {
        case RECTANGLE_SHAPE:
        {
            Ref<SGRectangleShape2D> rectangle = node->get_shape();

            undo_redo->add_do_method(rectangle.ptr(), "set_extents",
                                     rectangle->get_extents()->copy());
            undo_redo->add_do_method(this, "update_overlays");
            undo_redo->add_undo_method(rectangle.ptr(), "set_extents", p_org);
            undo_redo->add_undo_method(this, "update_overlays");
        }
        break;

        case CIRCLE_SHAPE:
        {
            Ref<SGCircleShape2D> circle = node->get_shape();

            undo_redo->add_do_method(circle.ptr(), "set_radius", circle->get_radius());
            undo_redo->add_do_method(this, "update_overlays");
            undo_redo->add_undo_method(circle.ptr(), "set_radius", p_org);
            undo_redo->add_undo_method(this, "update_overlays");
        }
        break;

        case CAPSULE_SHAPE:
        {
            Ref<SGCapsuleShape2D> capsule = node->get_shape();

            if (idx == 0)
            {
                undo_redo->add_do_method(capsule.ptr(), "set_radius", capsule->get_radius());
                undo_redo->add_undo_method(capsule.ptr(), "set_radius", p_org);
            }
            else if (idx == 1)
            {
                undo_redo->add_do_method(capsule.ptr(), "set_height", capsule->get_height());
                undo_redo->add_undo_method(capsule.ptr(), "set_height", p_org);
            }

            undo_redo->add_do_method(this, "update_overlays");
            undo_redo->add_undo_method(this, "update_overlays");
        }
        break;
    }

    undo_redo->commit_action();
}

void SGCollisionShape2DEditorPlugin::_get_current_shape_type()
{
    if (!node)
        return;

    Ref<SGShape2D> shape = node->get_shape();

    if (!shape.is_valid())
        return;

    if (Object::cast_to<SGRectangleShape2D>(*shape))
        shape_type = RECTANGLE_SHAPE;
    else if (Object::cast_to<SGCircleShape2D>(*shape))
        shape_type = CIRCLE_SHAPE;
    else if (Object::cast_to<SGCapsuleShape2D>(*shape))
        shape_type = CAPSULE_SHAPE;
    else
        shape_type = -1;

    update_overlays();
}

void SGCollisionShape2DEditorPlugin::_notification(int p_what)
{
    switch (p_what)
    {
        case NOTIFICATION_ENTER_TREE:
            get_tree()->connect("node_removed", Callable(this, "_node_removed"));
            break;

        case NOTIFICATION_EXIT_TREE:
            get_tree()->disconnect("node_removed", Callable(this, "_node_removed"));
            break;
    }
}

void SGCollisionShape2DEditorPlugin::_node_removed(Node* p_node)
{
    if (p_node == node)
        _edit(nullptr);
}

Transform2D SGCollisionShape2DEditorPlugin::get_canvas_transform() const
{
    return get_tree()->get_edited_scene_root()->get_viewport()->get_global_canvas_transform();
}

bool SGCollisionShape2DEditorPlugin::_forward_canvas_gui_input(const Ref<InputEvent>& p_event)
{
    if (!node)
        return false;

    if (!node->get_shape().is_valid())
        return false;

    if (shape_type == -1)
        return false;

    Ref<InputEventMouseButton> mb = p_event;
    Transform2D xform = get_canvas_transform() * node->get_global_transform();

    if (mb.is_valid())
    {
        Vector2 gpoint = mb->get_position();

        if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT)
        {
            if (mb->is_pressed())
            {
                for (int i = 0; i < handles.size(); i++)
                {
                    if (xform.xform(handles[i]).distance_to(gpoint) < 8)
                    {
                        edit_handle = i;
                        break;
                    }
                }

                if (edit_handle == -1)
                {
                    pressed = false;
                    return false;
                }

                original = get_handle_value(edit_handle);
                pressed = true;

                return true;
            }
            else if (pressed)
            {
                commit_handle(edit_handle, original);

                edit_handle = -1;
                pressed = false;

                return true;
            }
        }

        return false;
    }

    Ref<InputEventMouseMotion> mm = p_event;

    if (mm.is_valid())
    {
        if (edit_handle == -1 || !pressed)
            return false;

        // @todo Expose snap_point() to GDExtension and use it
        // Vector2 cpoint =
        // canvas_item_editor->snap_point(get_canvas_transform().affine_inverse().xform(mm->get_position()));
        Vector2 cpoint = get_canvas_transform().affine_inverse().xform(mm->get_position());
        cpoint = node->get_global_transform().affine_inverse().xform(cpoint);

        set_handle(edit_handle, cpoint);

        return true;
    }

    return false;
}

void SGCollisionShape2DEditorPlugin::_forward_canvas_draw_over_viewport(Control* p_overlay)
{
    if (!node)
        return;

    if (!node->get_shape().is_valid())
        return;

    _get_current_shape_type();

    if (shape_type == -1)
        return;

    Transform2D gt = get_canvas_transform() * node->get_global_transform();

    Ref<Texture2D> h = p_overlay->get_theme_icon("EditorHandle", "EditorIcons");
    Vector2 size = h->get_size() * 0.5;

    handles.clear();

    switch (shape_type)
    {
        case RECTANGLE_SHAPE:
        {
            Ref<SGRectangleShape2D> shape = node->get_shape();

            handles.resize(3);
            Vector2 ext = shape->get_extents()->to_float();
            handles.write[0] = Point2(ext.x, 0);
            handles.write[1] = Point2(0, -ext.y);
            handles.write[2] = Point2(ext.x, -ext.y);

            p_overlay->draw_texture(h, gt.xform(handles[0]) - size);
            p_overlay->draw_texture(h, gt.xform(handles[1]) - size);
            p_overlay->draw_texture(h, gt.xform(handles[2]) - size);
        }
        break;

        case CIRCLE_SHAPE:
        {
            Ref<SGCircleShape2D> shape = node->get_shape();

            handles.resize(1);
            handles.write[0] = Point2(fixed(shape->get_radius()).to_float(), 0);

            p_overlay->draw_texture(h, gt.xform(handles[0]) - size);
        }
        break;

        case CAPSULE_SHAPE:
        {
            Ref<SGCapsuleShape2D> shape = node->get_shape();

            handles.resize(2);
            float height = fixed(shape->get_height()).to_float();
            float radius = fixed(shape->get_radius()).to_float();
            handles.write[0] = Point2(radius, 0);
            handles.write[1] = Point2(0, -(height / 2 + radius));

            p_overlay->draw_texture(h, gt.xform(handles[0]) - size);
            p_overlay->draw_texture(h, gt.xform(handles[1]) - size);
        }
        break;
    }
}

void SGCollisionShape2DEditorPlugin::_edit(Object* p_obj)
{
    Node* new_node = Object::cast_to<Node>(p_obj);

    if (new_node)
    {
        node = Object::cast_to<SGCollisionShape2D>(new_node);

        _get_current_shape_type();
    }
    else
    {
        edit_handle = -1;
        shape_type = -1;

        node = nullptr;
    }

    update_overlays();
}

bool SGCollisionShape2DEditorPlugin::_handles(Object* p_obj) const
{
    SGCollisionShape2D* node_ = Object::cast_to<SGCollisionShape2D>(p_obj);
    return (bool)node_;
}

void SGCollisionShape2DEditorPlugin::_make_visible(bool visible)
{
    if (!visible)
        _edit(nullptr);
}

void SGCollisionShape2DEditorPlugin::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("_node_removed"), &SGCollisionShape2DEditorPlugin::_node_removed);
}

SGCollisionShape2DEditorPlugin::SGCollisionShape2DEditorPlugin()
{
    undo_redo = get_undo_redo();

    node = nullptr;
    edit_handle = -1;
    pressed = false;
}

SGCollisionShape2DEditorPlugin::~SGCollisionShape2DEditorPlugin()
{
}
