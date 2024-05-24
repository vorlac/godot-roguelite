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

#ifndef SG_SHAPES_2D_INTERNAL_H
#define SG_SHAPES_2D_INTERNAL_H

#include <vector>

#include "sg_fixed_rect2_internal.h"
#include "sg_fixed_transform_2d_internal.h"

class SGCollisionObject2DInternal;

class SGShape2DInternal
{
public:
    enum ShapeType {
        SHAPE_RECTANGLE,
        SHAPE_CIRCLE,
        SHAPE_POLYGON,
        SHAPE_CAPSULE,
    };

protected:
    friend class SGCollisionObject2DInternal;

    ShapeType shape_type;
    SGFixedTransform2DInternal transform;
    mutable SGFixedTransform2DInternal global_transform;
    mutable bool global_xform_dirty;
    mutable bool global_vertices_dirty;
    mutable bool global_axes_dirty;
    SGCollisionObject2DInternal* owner;
    mutable std::vector<SGFixedVector2Internal> global_vertices;
    mutable std::vector<SGFixedVector2Internal> global_axes;
    void* data;

    _FORCE_INLINE_ void mark_global_xform_dirty() const
    {
        global_xform_dirty = true;
        global_vertices_dirty = true;
        global_axes_dirty = true;
    }

    _FORCE_INLINE_ void set_owner(SGCollisionObject2DInternal* p_owner)
    {
        owner = p_owner;
        mark_global_xform_dirty();
    }

public:
    _FORCE_INLINE_ ShapeType get_shape_type() const
    {
        return shape_type;
    }

    _FORCE_INLINE_ void set_transform(const SGFixedTransform2DInternal& p_transform)
    {
        transform = p_transform;
        mark_global_xform_dirty();
    }

    _FORCE_INLINE_ SGFixedTransform2DInternal get_transform() const
    {
        return transform;
    }

    SGFixedTransform2DInternal get_global_transform() const;

    _FORCE_INLINE_ SGCollisionObject2DInternal* get_owner() const
    {
        return owner;
    }

    virtual std::vector<SGFixedVector2Internal> get_global_vertices() const;
    virtual std::vector<SGFixedVector2Internal> get_global_axes() const;
    virtual SGFixedRect2Internal get_bounds() const;

    _FORCE_INLINE_ void set_data(void* p_data)
    {
        data = p_data;
    }

    _FORCE_INLINE_ void* get_data() const
    {
        return data;
    }

    SGShape2DInternal(ShapeType p_shape_type)
    {
        shape_type = p_shape_type;
        global_xform_dirty = false;
        global_vertices_dirty = true;
        global_axes_dirty = true;
        owner = nullptr;
        data = nullptr;
    }

    virtual ~SGShape2DInternal()
    {
    }
};

class SGRectangle2DInternal : public SGShape2DInternal
{
protected:
    SGFixedVector2Internal extents;

public:
    _FORCE_INLINE_ SGFixedVector2Internal get_extents() const
    {
        return extents;
    }

    _FORCE_INLINE_ void set_extents(const SGFixedVector2Internal& p_extents)
    {
        extents = p_extents;
        global_vertices_dirty = true;
    }

    SGFixedVector2Internal get_closest_vertex(const SGFixedVector2Internal& point) const;
    virtual std::vector<SGFixedVector2Internal> get_global_vertices() const override;
    virtual std::vector<SGFixedVector2Internal> get_global_axes() const override;

    SGRectangle2DInternal(SGFixedVector2Internal p_extents)
        : SGShape2DInternal(SHAPE_RECTANGLE)
    {
        extents = p_extents;
        global_vertices.resize(4);
        global_axes.resize(2);
    }

    SGRectangle2DInternal(fixed p_extents_w, fixed p_extents_h)
        : SGRectangle2DInternal(SGFixedVector2Internal(p_extents_w, p_extents_h))
    {
    }
};

class SGCircle2DInternal : public SGShape2DInternal
{
protected:
    fixed radius;

public:
    _FORCE_INLINE_ fixed get_radius() const
    {
        return radius;
    }

    _FORCE_INLINE_ void set_radius(const fixed& p_radius)
    {
        radius = p_radius;
    }

    virtual SGFixedRect2Internal get_bounds() const override;

    SGCircle2DInternal(fixed p_radius)
        : SGShape2DInternal(SHAPE_CIRCLE)
    {
        radius = p_radius;
    }
};

class SGPolygon2DInternal : public SGShape2DInternal
{
protected:
    std::vector<SGFixedVector2Internal> points;

public:
    _FORCE_INLINE_ std::vector<SGFixedVector2Internal> get_points() const
    {
        return points;
    }

    _FORCE_INLINE_ void set_points(const std::vector<SGFixedVector2Internal>& p_points)
    {
        points = p_points;
        global_vertices.clear();
        global_axes.clear();
    }

    virtual std::vector<SGFixedVector2Internal> get_global_vertices() const override;
    virtual std::vector<SGFixedVector2Internal> get_global_axes() const override;

    SGPolygon2DInternal()
        : SGShape2DInternal(SHAPE_POLYGON)
    {
    }
};

class SGCapsule2DInternal : public SGShape2DInternal
{
protected:
    fixed radius;
    fixed height;

public:
    _FORCE_INLINE_ fixed get_radius() const
    {
        return radius;
    }

    _FORCE_INLINE_ void set_radius(const fixed& p_radius)
    {
        radius = p_radius;
    }

    _FORCE_INLINE_ fixed get_height() const
    {
        return height;
    }

    _FORCE_INLINE_ void set_height(const fixed& p_height)
    {
        height = p_height;
    }

    virtual std::vector<SGFixedVector2Internal> get_global_vertices() const override;
    virtual SGFixedRect2Internal get_bounds() const override;

    SGCapsule2DInternal(fixed p_radius, fixed p_height)
        : SGShape2DInternal(SHAPE_CAPSULE)
    {
        global_vertices.resize(2);
        radius = p_radius;
        height = p_height;
    }
};

#endif
