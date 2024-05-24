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

#include "sg_bodies_2d_internal.h"
#include "sg_shapes_2d_internal.h"

SGFixedTransform2DInternal SGShape2DInternal::get_global_transform() const
{
    if (!owner)
        return transform;
    if (global_xform_dirty)
    {
        global_transform = owner->get_transform() * transform;
        global_xform_dirty = false;
    }
    return global_transform;
}

std::vector<SGFixedVector2Internal> SGShape2DInternal::get_global_vertices() const
{
    return global_vertices;
}

std::vector<SGFixedVector2Internal> SGShape2DInternal::get_global_axes() const
{
    return global_axes;
}

SGFixedRect2Internal SGShape2DInternal::get_bounds() const
{
    std::vector<SGFixedVector2Internal> points = get_global_vertices();
    if (points.size() == 0)
        return SGFixedRect2Internal(global_transform.get_origin(), SGFixedVector2Internal());

    SGFixedRect2Internal bounds(points[0], SGFixedVector2Internal());
    for (std::size_t i = 1; i < points.size(); i++)
        bounds.expand_to(points[i]);

    return bounds;
}

std::vector<SGFixedVector2Internal> SGRectangle2DInternal::get_global_vertices() const
{
    if (global_vertices_dirty)
    {
        SGFixedTransform2DInternal t = get_global_transform();
        global_vertices[0] = t.xform(SGFixedVector2Internal(-extents.x, -extents.y));
        global_vertices[1] = t.xform(SGFixedVector2Internal(extents.x, -extents.y));
        global_vertices[2] = t.xform(SGFixedVector2Internal(extents.x, extents.y));
        global_vertices[3] = t.xform(SGFixedVector2Internal(-extents.x, extents.y));
        global_vertices_dirty = false;
    }

    return global_vertices;
}

std::vector<SGFixedVector2Internal> SGRectangle2DInternal::get_global_axes() const
{
    if (global_axes_dirty)
    {
        SGFixedTransform2DInternal t = get_global_transform();
        t.set_origin(SGFixedVector2Internal::ZERO);
        global_axes[0] = t.xform(SGFixedVector2Internal(extents.x, fixed::ZERO)).normalized();
        global_axes[1] = t.xform(SGFixedVector2Internal(fixed::ZERO, extents.y)).normalized();
        global_axes_dirty = false;
    }

    return global_axes;
}

SGFixedVector2Internal SGRectangle2DInternal::get_closest_vertex(
    const SGFixedVector2Internal& point) const
{
    const SGFixedTransform2DInternal t = get_global_transform();
    const SGFixedVector2Internal local_v = t.xform_inv(point);
    const SGFixedVector2Internal half_extents = SGFixedVector2Internal(
        fixed(get_extents().x.value / 2), fixed(get_extents().y.value / 2));

    const SGFixedVector2Internal vertex((local_v.x.value < 0) ? -half_extents.x : half_extents.x,
                                        (local_v.y.value < 0) ? -half_extents.y : half_extents.y);

    return t.xform(vertex);
}

std::vector<SGFixedVector2Internal> SGPolygon2DInternal::get_global_vertices() const
{
    if (global_vertices_dirty)
    {
        SGFixedTransform2DInternal t = get_global_transform();

        // Note: will only resize if it has a different size.
        global_vertices.resize(points.size());

        for (std::size_t i = 0; i < points.size(); i++)
            global_vertices[i] = t.xform(points[i]);
        global_vertices_dirty = false;
    }

    return global_vertices;
}

std::vector<SGFixedVector2Internal> SGPolygon2DInternal::get_global_axes() const
{
    if (global_axes_dirty)
    {
        SGFixedTransform2DInternal t = get_global_transform();
        t.set_origin(SGFixedVector2Internal::ZERO);

        // Note: will only resize if it has a different size.
        global_axes.resize(points.size());

        for (std::size_t i = 0; i < points.size(); i++)
        {
            std::size_t next_index = (i == points.size() - 1) ? 0 : i + 1;
            SGFixedVector2Internal edge = t.xform(points[next_index] - points[i]);
            // Get the vector perpendicular to the edge, which will be the edge normal.
            global_axes[i] = SGFixedVector2Internal(edge.y, -edge.x).normalized();
        }
        global_axes_dirty = false;
    }

    return global_axes;
}

SGFixedRect2Internal SGCircle2DInternal::get_bounds() const
{
    SGFixedTransform2DInternal t = get_global_transform();
    fixed radius_scaled = radius * t.get_scale().x;
    fixed diameter(radius_scaled.value << 1);
    return SGFixedRect2Internal(t.get_origin() - radius_scaled,
                                SGFixedVector2Internal(diameter, diameter));
}

std::vector<SGFixedVector2Internal> SGCapsule2DInternal::get_global_vertices() const
{
    if (global_vertices_dirty)
    {
        SGFixedTransform2DInternal t = get_global_transform();
        const fixed half_height = fixed(height.value / 2);
        global_vertices[0] = t.get_origin() + t.elements[1] * half_height;
        global_vertices[1] = t.get_origin() - t.elements[1] * half_height;
        global_vertices_dirty = false;
    }

    return global_vertices;
}

SGFixedRect2Internal SGCapsule2DInternal::get_bounds() const
{
    SGFixedTransform2DInternal t = get_global_transform();
    fixed radius_scaled = radius * t.get_scale().x;
    const std::vector<SGFixedVector2Internal> global_vertices = get_global_vertices();
    SGFixedRect2Internal bounds(global_vertices[0], SGFixedVector2Internal());
    bounds.expand_to(global_vertices[1]);
    bounds.position -= SGFixedVector2Internal(radius_scaled, radius_scaled);
    fixed diameter_scaled = fixed(radius_scaled.value * 2);
    bounds.size += SGFixedVector2Internal(diameter_scaled, diameter_scaled);
    return bounds;
}
