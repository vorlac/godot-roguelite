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

#ifndef SG_COLLISION_SHAPE_2D_EDITOR_PLUGIN_H
#define SG_COLLISION_SHAPE_2D_EDITOR_PLUGIN_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/templates/vector.hpp>

using namespace godot;

class SGCollisionShape2D;

class SGCollisionShape2DEditorPlugin : public EditorPlugin
{
    GDCLASS(SGCollisionShape2DEditorPlugin, EditorPlugin);

    enum ShapeType {
        RECTANGLE_SHAPE,
        CIRCLE_SHAPE,
        CAPSULE_SHAPE,
    };

    EditorUndoRedoManager* undo_redo;
    SGCollisionShape2D* node = NULL;

    Vector<Point2> handles;

    int shape_type;
    int edit_handle;
    bool pressed;
    Variant original;

    Variant get_handle_value(int idx) const;
    void set_handle(int idx, Point2& p_point);
    void commit_handle(int idx, Variant& p_org);

    void _get_current_shape_type();

protected:
    static void _bind_methods();

    void _notification(int p_what);
    void _node_removed(Node* p_node);

    Transform2D get_canvas_transform() const;

public:
    virtual bool _forward_canvas_gui_input(const Ref<InputEvent>& p_event) override;
    virtual void _forward_canvas_draw_over_viewport(Control* p_overlay) override;

    virtual String _get_plugin_name() const override
    {
        return "SGCollisionShape2D";
    }

    virtual bool _has_main_screen() const override
    {
        return false;
    }

    virtual bool _handles(Object* p_obj) const override;
    virtual void _edit(Object* p_obj) override;
    virtual void _make_visible(bool visible) override;

    SGCollisionShape2DEditorPlugin();
    ~SGCollisionShape2DEditorPlugin();
};

#endif
