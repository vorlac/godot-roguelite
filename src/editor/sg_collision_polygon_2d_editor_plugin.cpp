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

#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/v_separator.hpp>

#include "sg_collision_polygon_2d_editor_plugin.h"

bool SGCollisionPolygon2DEditor::Vertex::operator==(
    const SGCollisionPolygon2DEditor::Vertex& p_vertex) const
{
    return polygon == p_vertex.polygon && vertex == p_vertex.vertex;
}

bool SGCollisionPolygon2DEditor::Vertex::operator!=(
    const SGCollisionPolygon2DEditor::Vertex& p_vertex) const
{
    return !(*this == p_vertex);
}

bool SGCollisionPolygon2DEditor::Vertex::valid() const
{
    return vertex >= 0;
}

bool SGCollisionPolygon2DEditor::_is_empty() const
{
    if (node == nullptr)
        return true;

    const int n = _get_polygon_count();

    for (int i = 0; i < n; i++)
    {
        PackedVector2Array vertices = _get_polygon(i);

        if (vertices.size() != 0)
            return false;
    }

    return true;
}

int SGCollisionPolygon2DEditor::_get_polygon_count() const
{
    return 1;
}

Variant SGCollisionPolygon2DEditor::_get_polygon(int p_idx) const
{
    return node->get("polygon");
}

void SGCollisionPolygon2DEditor::_set_polygon(int p_idx, const Variant& p_polygon) const
{
    node->set("polygon", p_polygon);
}

void SGCollisionPolygon2DEditor::_action_set_polygon(int p_idx, const Variant& p_previous,
                                                     const Variant& p_polygon)
{
    EditorUndoRedoManager* undo_redo = plugin->get_undo_redo();
    undo_redo->add_do_method(node, "set_polygon", p_polygon);
    undo_redo->add_undo_method(node, "set_polygon", p_previous);
}

Vector2 SGCollisionPolygon2DEditor::_get_offset(int p_idx) const
{
    return Vector2(0, 0);
}

void SGCollisionPolygon2DEditor::_commit_action()
{
    EditorUndoRedoManager* undo_redo = plugin->get_undo_redo();
    undo_redo->add_do_method(plugin, "update_overlays");
    undo_redo->add_undo_method(plugin, "update_overlays");
    undo_redo->commit_action();
}

void SGCollisionPolygon2DEditor::_action_add_polygon(const Variant& p_polygon)
{
    _action_set_polygon(0, p_polygon);
}

void SGCollisionPolygon2DEditor::_action_remove_polygon(int p_idx)
{
    _action_set_polygon(p_idx, _get_polygon(p_idx), PackedVector2Array());
}

void SGCollisionPolygon2DEditor::_action_set_polygon(int p_idx, const Variant& p_polygon)
{
    _action_set_polygon(p_idx, _get_polygon(p_idx), p_polygon);
}

bool SGCollisionPolygon2DEditor::_has_resource() const
{
    return true;
}

void SGCollisionPolygon2DEditor::_create_resource()
{
}

void SGCollisionPolygon2DEditor::_menu_option(int p_option)
{
    switch (p_option)
    {
        case MODE_CREATE:
        {
            mode = MODE_CREATE;
            button_create->set_pressed(true);
            button_edit->set_pressed(false);
            button_delete->set_pressed(false);
        }
        break;
        case MODE_EDIT:
        {
            _wip_close();
            mode = MODE_EDIT;
            button_create->set_pressed(false);
            button_edit->set_pressed(true);
            button_delete->set_pressed(false);
        }
        break;
        case MODE_DELETE:
        {
            _wip_close();
            mode = MODE_DELETE;
            button_create->set_pressed(false);
            button_edit->set_pressed(false);
            button_delete->set_pressed(true);
        }
        break;
    }
}

void SGCollisionPolygon2DEditor::_menu_option_create()
{
    _menu_option(MODE_CREATE);
}

void SGCollisionPolygon2DEditor::_menu_option_edit()
{
    _menu_option(MODE_EDIT);
}

void SGCollisionPolygon2DEditor::_menu_option_delete()
{
    _menu_option(MODE_DELETE);
}

void SGCollisionPolygon2DEditor::_notification(int p_what)
{
    switch (p_what)
    {
        case NOTIFICATION_ENTER_TREE:
        case NOTIFICATION_THEME_CHANGED:
        {
            button_create->set_button_icon(get_theme_icon("CurveCreate", "EditorIcons"));
            button_edit->set_button_icon(get_theme_icon("CurveEdit", "EditorIcons"));
            button_delete->set_button_icon(get_theme_icon("CurveDelete", "EditorIcons"));
        }
        break;

        case NOTIFICATION_READY:
        {
            disable_polygon_editing(false, String());

            button_edit->set_pressed(true);

            // @todo Replace with callable_mp().
            // get_tree()->connect("node_removed", callable_mp(this,
            // &SGCollisionPolygon2DEditor::_node_removed));
            get_tree()->connect("node_removed", Callable(this, "_node_removed"));
            // @todo Replace with callable_mp().
            // create_resource->connect("confirmed", callable_mp(this,
            // &SGCollisionPolygon2DEditor::_create_resource));
            create_resource->connect("confirmed", Callable(this, "_create_resource"));
        }
        break;
    }
}

void SGCollisionPolygon2DEditor::_node_removed(Node* p_node)
{
    if (p_node == node)
    {
        edit(nullptr);
        hide();

        plugin->update_overlays();
    }
}

void SGCollisionPolygon2DEditor::_wip_changed()
{
}

void SGCollisionPolygon2DEditor::_wip_cancel()
{
    wip.clear();
    wip_active = false;

    edited_point = PosVertex();
    hover_point = Vertex();
    selected_point = Vertex();

    plugin->update_overlays();
}

void SGCollisionPolygon2DEditor::_wip_close()
{
    if (!wip_active)
        return;

    if (wip.size() >= 3)
    {
        EditorUndoRedoManager* undo_redo = plugin->get_undo_redo();
        undo_redo->create_action("Create Polygon");
        _action_add_polygon(wip);
        _commit_action();
    }
    else
    {
        return;
    }

    mode = MODE_EDIT;
    button_edit->set_pressed(true);
    button_create->set_pressed(false);
    button_delete->set_pressed(false);

    wip.clear();
    wip_active = false;

    edited_point = PosVertex();
    hover_point = Vertex();
    selected_point = Vertex();
}

void SGCollisionPolygon2DEditor::disable_polygon_editing(bool p_disable, String p_reason)
{
    _polygon_editing_enabled = !p_disable;

    button_create->set_disabled(p_disable);
    button_edit->set_disabled(p_disable);
    button_delete->set_disabled(p_disable);

    if (p_disable)
    {
        button_create->set_tooltip_text(p_reason);
        button_edit->set_tooltip_text(p_reason);
        button_delete->set_tooltip_text(p_reason);
    }
    else
    {
        button_create->set_tooltip_text("Create points.");
        button_edit->set_tooltip_text("Edit points.\nLMB: Move Point\nRMB: Erase Point");
        button_delete->set_tooltip_text("Erase points.");
    }
}

bool SGCollisionPolygon2DEditor::forward_gui_input(const Ref<InputEvent>& p_event)
{
    if (!node || !_polygon_editing_enabled)
        return false;

    if (!node->is_visible_in_tree())
        return false;

    EditorUndoRedoManager* undo_redo = plugin->get_undo_redo();
    Ref<InputEventMouseButton> mb = p_event;

    if (!_has_resource())
    {
        if (mb.is_valid() && mb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT &&
            mb->is_pressed())
        {
            create_resource->set_text(
                String("No polygon resource on this node.\nCreate and assign one?"));
            create_resource->popup_centered();
        }
        return (mb.is_valid() && mb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT);
    }

    // @todo Get the current tool (after it's exposed to GDExtension).
    /*
    CanvasItemEditor::Tool tool = CanvasItemEditor::get_singleton()->get_current_tool();
    if (tool != CanvasItemEditor::TOOL_SELECT) {
        return false;
    }
    */

    if (mb.is_valid())
    {
        Transform2D xform = get_canvas_transform() * node->get_global_transform();

        Vector2 gpoint = mb->get_position();
        // @todo Use snap_point().
        // Vector2 cpoint =
        // node->to_local(canvas_item_editor->snap_point(get_canvas_transform().affine_inverse().xform(mb->get_position())));
        Vector2 cpoint = node->to_local(
            get_canvas_transform().affine_inverse().xform(mb->get_position()));

        if (mode == MODE_EDIT)
        {
            if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT)
            {
                if (mb->is_pressed())
                {
                    if (mb->is_ctrl_pressed() || mb->is_shift_pressed() || mb->is_alt_pressed())
                        return false;

                    const PosVertex insert = closest_edge_point(gpoint);

                    if (insert.valid())
                    {
                        PackedVector2Array vertices = _get_polygon(insert.polygon);

                        if (vertices.size() < 3)
                        {
                            vertices.push_back(cpoint);
                            undo_redo->create_action("Edit Polygon");
                            selected_point = Vertex(insert.polygon, vertices.size());
                            _action_set_polygon(insert.polygon, vertices);
                            _commit_action();
                            return true;
                        }
                        else
                        {
                            edited_point = PosVertex(insert.polygon, insert.vertex + 1,
                                                     xform.affine_inverse().xform(insert.pos));
                            vertices.insert(edited_point.vertex, edited_point.pos);
                            pre_move_edit = vertices;
                            selected_point = Vertex(edited_point.polygon, edited_point.vertex);
                            edge_point = PosVertex();

                            undo_redo->create_action("Insert Point");
                            _action_set_polygon(insert.polygon, vertices);
                            _commit_action();
                            return true;
                        }
                    }
                    else
                    {
                        // look for points to move
                        const PosVertex closest = closest_point(gpoint);

                        if (closest.valid())
                        {
                            pre_move_edit = _get_polygon(closest.polygon);
                            edited_point = PosVertex(closest,
                                                     xform.affine_inverse().xform(closest.pos));
                            selected_point = closest;
                            edge_point = PosVertex();
                            plugin->update_overlays();
                            return true;
                        }
                        else
                        {
                            selected_point = Vertex();
                        }
                    }
                }
                else if (edited_point.valid())
                {
                    // apply

                    PackedVector2Array vertices = _get_polygon(edited_point.polygon);
                    ERR_FAIL_INDEX_V(edited_point.vertex, vertices.size(), false);
                    vertices[edited_point.vertex] = edited_point.pos -
                                                    _get_offset(edited_point.polygon);

                    undo_redo->create_action("Edit Polygon");
                    _action_set_polygon(edited_point.polygon, pre_move_edit, vertices);
                    _commit_action();

                    edited_point = PosVertex();
                    return true;
                }
            }
            else if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_RIGHT &&
                     mb->is_pressed() && !edited_point.valid())
            {
                const PosVertex closest = closest_point(gpoint);

                if (closest.valid())
                {
                    remove_point(closest);
                    return true;
                }
            }
        }
        else if (mode == MODE_DELETE)
        {
            if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_RIGHT && mb->is_pressed())
            {
                const PosVertex closest = closest_point(gpoint);

                if (closest.valid())
                {
                    remove_point(closest);
                    return true;
                }
            }
        }

        if (mode == MODE_CREATE)
        {
            if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT && mb->is_pressed())
            {
                if (!wip_active)
                {
                    wip.clear();
                    wip.push_back(cpoint);
                    wip_active = true;
                    _wip_changed();
                    edited_point = PosVertex(-1, 1, cpoint);
                    plugin->update_overlays();
                    hover_point = Vertex();
                    selected_point = Vertex(0);
                    edge_point = PosVertex();
                    return true;
                }
                else
                {
                    // @todo Use EDITOR_GET()
                    // const real_t grab_threshold =
                    // EDITOR_GET("editors/polygon_editor/point_grab_radius");
                    const real_t grab_threshold = 8;

                    if (wip.size() > 1 &&
                        xform.xform(wip[0]).distance_to(xform.xform(cpoint)) < grab_threshold)
                    {
                        // wip closed
                        _wip_close();

                        return true;
                    }
                    else
                    {
                        // add wip point
                        wip.push_back(cpoint);
                        _wip_changed();
                        edited_point = PosVertex(-1, wip.size(), cpoint);
                        selected_point = Vertex(wip.size() - 1);
                        plugin->update_overlays();
                        return true;
                    }
                }
            }
            else if (mb->get_button_index() == MouseButton::MOUSE_BUTTON_RIGHT &&
                     mb->is_pressed() && wip_active)
            {
                _wip_cancel();
            }
        }
    }

    Ref<InputEventMouseMotion> mm = p_event;

    if (mm.is_valid())
    {
        Vector2 gpoint = mm->get_position();

        if (edited_point.valid() &&
            (wip_active || mm->get_button_mask().has_flag(MouseButtonMask::MOUSE_BUTTON_MASK_LEFT)))
        {
            // @todo Use snap_point()
            // Vector2 cpoint =
            // node->to_local(canvas_item_editor->snap_point(get_canvas_transform().affine_inverse().xform(gpoint)));
            Vector2 cpoint = node->to_local(get_canvas_transform().affine_inverse().xform(gpoint));

            // Move the point in a single axis. Should only work when editing a polygon and while
            // holding shift.
            if (mode == MODE_EDIT && mm->is_shift_pressed())
            {
                Vector2 old_point = pre_move_edit[selected_point.vertex];
                if (ABS(cpoint.x - old_point.x) > ABS(cpoint.y - old_point.y))
                    cpoint.y = old_point.y;
                else
                    cpoint.x = old_point.x;
            }

            edited_point = PosVertex(edited_point, cpoint);

            if (!wip_active)
            {
                PackedVector2Array vertices = _get_polygon(edited_point.polygon);
                ERR_FAIL_INDEX_V(edited_point.vertex, vertices.size(), false);
                vertices[edited_point.vertex] = cpoint - _get_offset(edited_point.polygon);
                _set_polygon(edited_point.polygon, vertices);
            }

            plugin->update_overlays();
        }
        else if (mode == MODE_EDIT)
        {
            const PosVertex onEdgeVertex = closest_edge_point(gpoint);

            if (onEdgeVertex.valid())
            {
                hover_point = Vertex();
                edge_point = onEdgeVertex;
                plugin->update_overlays();
            }
            else
            {
                if (edge_point.valid())
                {
                    edge_point = PosVertex();
                    plugin->update_overlays();
                }

                const PosVertex new_hover_point = closest_point(gpoint);
                if (hover_point != new_hover_point)
                {
                    hover_point = new_hover_point;
                    plugin->update_overlays();
                }
            }
        }
    }

    Ref<InputEventKey> k = p_event;

    if (k.is_valid() && k->is_pressed())
    {
        if (k->get_keycode() == Key::KEY_DELETE || k->get_keycode() == Key::KEY_BACKSPACE)
        {
            if (wip_active && selected_point.polygon == -1)
            {
                if (wip.size() > selected_point.vertex)
                {
                    wip.remove_at(selected_point.vertex);
                    _wip_changed();
                    selected_point = wip.size() - 1;
                    plugin->update_overlays();
                    return true;
                }
            }
            else
            {
                const Vertex active_point = get_active_point();

                if (active_point.valid())
                {
                    remove_point(active_point);
                    return true;
                }
            }
        }
        else if (wip_active && k->get_keycode() == Key::KEY_ENTER)
        {
            _wip_close();
        }
        else if (wip_active && k->get_keycode() == Key::KEY_ESCAPE)
        {
            _wip_cancel();
        }
    }

    return false;
}

void SGCollisionPolygon2DEditor::forward_canvas_draw_over_viewport(Control* p_overlay)
{
    if (!node)
        return;

    if (!node->is_visible_in_tree())
        return;

    Transform2D xform = get_canvas_transform() * node->get_global_transform_with_canvas();
    // All polygon points are sharp, so use the sharp handle icon
    const Ref<Texture2D> handle = get_theme_icon("EditorPathSharpHandle", "EditorIcons");

    const Vertex active_point = get_active_point();
    const int n_polygons = _get_polygon_count();
    const bool is_closed = true;

    for (int j = -1; j < n_polygons; j++)
    {
        if (wip_active && j != -1)
            continue;

        PackedVector2Array points;
        Vector2 offset;

        if (wip_active && j == edited_point.polygon)
        {
            points = Variant(wip);
            offset = Vector2(0, 0);
        }
        else
        {
            if (j == -1)
                continue;
            points = _get_polygon(j);
            offset = _get_offset(j);
        }

        // @todo Implement EDITOR_GET
        // if (!wip_active && j == edited_point.polygon &&
        // EDITOR_GET("editors/polygon_editor/show_previous_outline")) {
        if (!wip_active && j == edited_point.polygon)
        {
            const Color col = Color(0.5, 0.5, 0.5);  // FIXME polygon->get_outline_color();
            const int n = pre_move_edit.size();
            for (int i = 0; i < n - (is_closed ? 0 : 1); i++)
            {
                Vector2 p, p2;
                p = pre_move_edit[i] + offset;
                p2 = pre_move_edit[(i + 1) % n] + offset;

                Vector2 point = xform.xform(p);
                Vector2 next_point = xform.xform(p2);

                // @todo Multiply by EDSCALE
                // p_overlay->draw_line(point, next_point, col, Math::round(2 * EDSCALE));
                p_overlay->draw_line(point, next_point, col, 2);
            }
        }

        const int n_points = points.size();
        const Color col = Color(1, 0.3, 0.1, 0.8);

        for (int i = 0; i < n_points; i++)
        {
            const Vertex vertex(j, i);

            const Vector2 p = (vertex == edited_point) ? edited_point.pos : (points[i] + offset);
            const Vector2 point = xform.xform(p);

            if (is_closed || i < n_points - 1)
            {
                Vector2 p2;
                if (j == edited_point.polygon && ((wip_active && i == n_points - 1) ||
                                                  (((i + 1) % n_points) == edited_point.vertex)))
                {
                    p2 = edited_point.pos;
                }
                else
                {
                    p2 = points[(i + 1) % n_points] + offset;
                }

                const Vector2 next_point = xform.xform(p2);
                // @todo Multiply by EDSCALE
                // p_overlay->draw_line(point, next_point, col, Math::round(2 * EDSCALE));
                p_overlay->draw_line(point, next_point, col, 2);
            }
        }

        for (int i = 0; i < n_points; i++)
        {
            const Vertex vertex(j, i);

            const Vector2 p = (vertex == edited_point) ? edited_point.pos : (points[i] + offset);
            const Vector2 point = xform.xform(p);

            const Color overlay_modulate = vertex == active_point ? Color(0.5, 1, 2) : Color(1, 1, 1);
            p_overlay->draw_texture(handle, point - handle->get_size() * 0.5, overlay_modulate);

            if (vertex == hover_point)
            {
                Ref<Font> font = get_theme_font("bold", "EditorFonts");
                int font_size = 1.3 * get_theme_font_size("bold_size", "EditorFonts");
                String num = String::num(vertex.vertex);
                Size2 num_size = font->get_string_size(num, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size);
                const float outline_size = 4;
                Color font_color = get_theme_color("font_color", "Editor");
                Color outline_color = font_color.inverted();
                p_overlay->draw_string_outline(font, point - num_size * 0.5, num,
                                               HORIZONTAL_ALIGNMENT_LEFT, -1, font_size,
                                               outline_size, outline_color);
                p_overlay->draw_string(font, point - num_size * 0.5, num, HORIZONTAL_ALIGNMENT_LEFT,
                                       -1, font_size, font_color);
            }
        }
    }

    if (edge_point.valid())
    {
        Ref<Texture2D> add_handle = get_theme_icon("EditorHandleAdd", "EditorIcons");
        p_overlay->draw_texture(add_handle, edge_point.pos - add_handle->get_size() * 0.5);
    }
}

void SGCollisionPolygon2DEditor::edit(SGCollisionPolygon2D* p_polygon)
{
    if (p_polygon)
    {
        node = p_polygon;

        // Enable the pencil tool if the polygon is empty.
        if (_is_empty())
            _menu_option(MODE_CREATE);
        else
            _menu_option(MODE_EDIT);

        wip.clear();
        wip_active = false;
        edited_point = PosVertex();
        hover_point = Vertex();
        selected_point = Vertex();
    }
    else
    {
        node = nullptr;
    }

    if (plugin)
        plugin->update_overlays();
}

void SGCollisionPolygon2DEditor::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("_node_removed"), &SGCollisionPolygon2DEditor::_node_removed);
    ClassDB::bind_method(D_METHOD("_create_resource"), &SGCollisionPolygon2DEditor::_create_resource);

    // @todo Replace with callable_mp() and Callable.bind().
    ClassDB::bind_method(D_METHOD("_menu_option_create"),
                         &SGCollisionPolygon2DEditor::_menu_option_create);
    ClassDB::bind_method(D_METHOD("_menu_option_edit"),
                         &SGCollisionPolygon2DEditor::_menu_option_edit);
    ClassDB::bind_method(D_METHOD("_menu_option_delete"),
                         &SGCollisionPolygon2DEditor::_menu_option_delete);
}

Transform2D SGCollisionPolygon2DEditor::get_canvas_transform() const
{
    return get_tree()->get_edited_scene_root()->get_viewport()->get_global_canvas_transform();
}

void SGCollisionPolygon2DEditor::remove_point(const Vertex& p_vertex)
{
    EditorUndoRedoManager* undo_redo = plugin->get_undo_redo();
    PackedVector2Array vertices = _get_polygon(p_vertex.polygon);

    if (vertices.size() > 3)
    {
        vertices.remove_at(p_vertex.vertex);

        undo_redo->create_action("Edit Polygon (Remove Point)");
        _action_set_polygon(p_vertex.polygon, vertices);
        _commit_action();
    }
    else
    {
        undo_redo->create_action("Remove Polygon And Point");
        _action_remove_polygon(p_vertex.polygon);
        _commit_action();
    }

    if (_is_empty())
        _menu_option(MODE_CREATE);

    hover_point = Vertex();
    if (selected_point == p_vertex)
        selected_point = Vertex();
}

SGCollisionPolygon2DEditor::Vertex SGCollisionPolygon2DEditor::get_active_point() const
{
    return hover_point.valid() ? hover_point : selected_point;
}

SGCollisionPolygon2DEditor::PosVertex SGCollisionPolygon2DEditor::closest_point(
    const Vector2& p_pos) const
{
    // @todo Implement EDITOR_GET().
    // const real_t grab_threshold = EDITOR_GET("editors/polygon_editor/point_grab_radius");
    const real_t grab_threshold = 8;

    const int n_polygons = _get_polygon_count();
    const Transform2D xform = get_canvas_transform() * node->get_global_transform();

    PosVertex closest;
    real_t closest_dist = 1e10;

    for (int j = 0; j < n_polygons; j++)
    {
        PackedVector2Array points = _get_polygon(j);
        const Vector2 offset = _get_offset(j);
        const int n_points = points.size();

        for (int i = 0; i < n_points; i++)
        {
            Vector2 cp = xform.xform(points[i] + offset);

            real_t d = cp.distance_to(p_pos);
            if (d < closest_dist && d < grab_threshold)
            {
                closest_dist = d;
                closest = PosVertex(j, i, cp);
            }
        }
    }

    return closest;
}

SGCollisionPolygon2DEditor::PosVertex SGCollisionPolygon2DEditor::closest_edge_point(
    const Vector2& p_pos) const
{
    // @todo Implement EDITOR_GET().
    // const real_t grab_threshold = EDITOR_GET("editors/polygon_editor/point_grab_radius");
    const real_t grab_threshold = 8;
    const real_t eps = grab_threshold * 2;
    const real_t eps2 = eps * eps;

    const int n_polygons = _get_polygon_count();
    const Transform2D xform = get_canvas_transform() * node->get_global_transform();

    PosVertex closest;
    real_t closest_dist = 1e10;

    for (int j = 0; j < n_polygons; j++)
    {
        PackedVector2Array points = _get_polygon(j);
        const Vector2 offset = _get_offset(j);
        const int n_points = points.size();
        const int n_segments = n_points;

        for (int i = 0; i < n_segments; i++)
        {
            Vector2 segment[2] = { xform.xform(points[i] + offset),
                                   xform.xform(points[(i + 1) % n_points] + offset) };

            Vector2 cp = get_closest_point_to_segment(p_pos, segment);

            if (cp.distance_squared_to(segment[0]) < eps2 || cp.distance_squared_to(segment[1]) < eps2)
            {
                continue;  // not valid to reuse point
            }

            real_t d = cp.distance_to(p_pos);
            if (d < closest_dist && d < grab_threshold)
            {
                closest_dist = d;
                closest = PosVertex(j, i, cp);
            }
        }
    }

    return closest;
}

Vector2 SGCollisionPolygon2DEditor::get_closest_point_to_segment(const Vector2& p_point,
                                                                 const Vector2* p_segment)
{
    Vector2 p = p_point - p_segment[0];
    Vector2 n = p_segment[1] - p_segment[0];
    real_t l2 = n.length_squared();
    if (l2 < 1e-20f)
        return p_segment[0];  // Both points are the same, just give any.

    real_t d = n.dot(p) / l2;

    if (d <= 0.0f)
        return p_segment[0];  // Before first point.
    else if (d >= 1.0f)
        return p_segment[1];  // After first point.
    else
        return p_segment[0] + n * d;  // Inside.
}

SGCollisionPolygon2DEditor::SGCollisionPolygon2DEditor(EditorPlugin* p_plugin)
{
    plugin = p_plugin;

    edited_point = PosVertex();

    hover_point = Vertex();
    selected_point = Vertex();
    edge_point = PosVertex();

    add_child(memnew(VSeparator));
    button_create = memnew(Button);
    button_create->set_flat(true);
    add_child(button_create);
    // @todo Switch back to callable_mp() with Callable.bind().
    // button_create->connect("pressed", callable_mp(this,
    // &SGCollisionPolygon2DEditor::_menu_option).bind(MODE_CREATE));
    button_create->connect("pressed", Callable(this, "_menu_option_create"));
    button_create->set_toggle_mode(true);

    button_edit = memnew(Button);
    button_edit->set_flat(true);
    add_child(button_edit);
    // @todo Switch back to callable_mp() with Callable.bind().
    // button_edit->connect("pressed", callable_mp(this,
    // &SGCollisionPolygon2DEditor::_menu_option).bind(MODE_EDIT));
    button_edit->connect("pressed", Callable(this, "_menu_option_edit"));
    button_edit->set_toggle_mode(true);

    button_delete = memnew(Button);
    button_delete->set_flat(true);
    add_child(button_delete);
    // @todo Switch back to callable_mp() with Callable.bind().
    // button_delete->connect("pressed", callable_mp(this,
    // &SGCollisionPolygon2DEditor::_menu_option).bind(MODE_DELETE));
    button_delete->connect("pressed", Callable(this, "_menu_option_delete"));
    button_delete->set_toggle_mode(true);

    create_resource = memnew(ConfirmationDialog);
    add_child(create_resource);
    create_resource->set_ok_button_text("Create");
}

void SGCollisionPolygon2DEditorPlugin::_edit(Object* p_object)
{
    polygon_editor->edit(Object::cast_to<SGCollisionPolygon2D>(p_object));
}

bool SGCollisionPolygon2DEditorPlugin::_handles(Object* p_object) const
{
    return Object::cast_to<SGCollisionPolygon2D>(p_object) != nullptr;
}

void SGCollisionPolygon2DEditorPlugin::_make_visible(bool p_visible)
{
    if (p_visible)
    {
        polygon_editor->show();
    }
    else
    {
        polygon_editor->hide();
        polygon_editor->edit(nullptr);
    }
}

void SGCollisionPolygon2DEditorPlugin::_bind_methods()
{
}

void SGCollisionPolygon2DEditorPlugin::_notification(int p_what)
{
    switch (p_what)
    {
        case NOTIFICATION_ENTER_TREE:
        {
            add_control_to_container(CONTAINER_CANVAS_EDITOR_MENU, polygon_editor);
            polygon_editor->hide();
        }
        break;
        case NOTIFICATION_EXIT_TREE:
        {
            remove_control_from_container(CONTAINER_CANVAS_EDITOR_MENU, polygon_editor);
        }
        break;
    }
}

SGCollisionPolygon2DEditorPlugin::SGCollisionPolygon2DEditorPlugin()
{
    polygon_editor = memnew(SGCollisionPolygon2DEditor(this));
}

SGCollisionPolygon2DEditorPlugin::~SGCollisionPolygon2DEditorPlugin()
{
    memdelete(polygon_editor);
}
