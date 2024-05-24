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

#include <algorithm>

#include "sg_collision_detector_2d_internal.h"

using Interval = SGCollisionDetector2DInternal::Interval;

bool SGCollisionDetector2DInternal::AABB_overlaps_AABB(const SGFixedRect2Internal& aabb1,
                                                       const SGFixedRect2Internal& aabb2)
{
    SGFixedVector2Internal min_one = aabb1.get_min();
    SGFixedVector2Internal max_one = aabb1.get_max();
    SGFixedVector2Internal min_two = aabb2.get_min();
    SGFixedVector2Internal max_two = aabb2.get_max();

    return (min_two.x <= max_one.x) && (min_one.x <= max_two.x) && (min_two.y <= max_one.y) &&
           (min_one.y <= max_two.y);
}

Interval SGCollisionDetector2DInternal::get_interval(
    const SGShape2DInternal& shape, const SGFixedVector2Internal& axis, fixed p_margin)
{
    Interval result;

    if (shape.get_shape_type() == SGShape2DInternal::ShapeType::SHAPE_CIRCLE)
    {
        const SGCircle2DInternal& circle = (const SGCircle2DInternal&)shape;
        SGFixedTransform2DInternal t = shape.get_global_transform();
        fixed center = axis.dot(t.get_origin());
        fixed radius = circle.get_radius() * t.get_scale().x;
        result.min = center - radius;
        result.max = center + radius;
    }
    else if (shape.get_shape_type() == SGShape2DInternal::ShapeType::SHAPE_CAPSULE)
    {
        const SGCapsule2DInternal& capsule = (const SGCapsule2DInternal&)shape;
        SGFixedTransform2DInternal t = shape.get_global_transform();
        const fixed scaled_radius = capsule.get_radius() * t.get_scale().x;
        const std::vector<SGFixedVector2Internal> verts = capsule.get_global_vertices();
        result.min = axis.dot(verts[0]);
        result.max = axis.dot(verts[1]);
        if (result.min > result.max)
            std::swap(result.min, result.max);
        result.min -= scaled_radius;
        result.max += scaled_radius;
    }
    else
    {
        std::vector<SGFixedVector2Internal> verts = shape.get_global_vertices();
        result.min = result.max = axis.dot(verts[0]);
        for (std::size_t i = 1; i < verts.size(); i++)
        {
            fixed projection = axis.dot(verts[i]);
            if (projection < result.min)
                result.min = projection;
            if (projection > result.max)
                result.max = projection;
        }
    }

    result.min -= p_margin;
    result.max += p_margin;

    return result;
}

bool SGCollisionDetector2DInternal::overlaps_on_axis(
    const SGShape2DInternal& shape1, const SGShape2DInternal& shape2,
    const SGFixedVector2Internal& axis, fixed p_margin, fixed& separation)
{
    Interval i1 = get_interval(shape1, axis, p_margin);
    Interval i2 = get_interval(shape2, axis, p_margin);

    fixed d1 = i1.max - i2.min;
    fixed d2 = i2.max - i1.min;

    if (d1 > fixed::ZERO && d2 > fixed::ZERO)
    {
        separation = (d1 < d2) ? d1 : d2;

        // Attempt to make the seperation relative to shape1.
        if (i1.min < i2.min)
            separation = -separation;
        return true;
    }

    return false;
}

bool SGCollisionDetector2DInternal::sat_test(
    const SGShape2DInternal& shape1, const SGShape2DInternal& shape2,
    const std::vector<SGFixedVector2Internal>& axes, fixed p_margin,
    SGFixedVector2Internal& best_separation_vector, fixed& best_separation_length,
    SGFixedVector2Internal& collision_normal)
{
    fixed separation_component;

    for (std::size_t i = 0; i < axes.size(); i++)
    {
        const SGFixedVector2Internal& axis = axes[i];
        if (axis == SGFixedVector2Internal::ZERO)
            continue;
        if (overlaps_on_axis(shape1, shape2, axis, p_margin, separation_component))
        {
            SGFixedVector2Internal separation_vector = (axis * separation_component);

            // if the correction is too small it will be set to 0 and the overlap won't be corrected
            // in that case we don't consider it an overlap
            if ((axis.x == fixed::ZERO || separation_vector.x != fixed::ZERO) &&
                (axis.y == fixed::ZERO || separation_vector.y != fixed::ZERO))
            {
                fixed separation_length = separation_component.abs();
                if (best_separation_vector == SGFixedVector2Internal::ZERO ||
                    separation_length < best_separation_length)
                {
                    best_separation_vector = separation_vector;
                    best_separation_length = separation_length;
                    collision_normal = axes[i] * FIXED_SGN(separation_component);
                }
            }
            else
            {
                // Consider this as a separation axis
                return false;
            }
        }
        else
        {
            // Axis of separation found! They don't overlap.
            return false;
        }
    }

    // No axis of separation found, they overlap!
    return true;
}

bool SGCollisionDetector2DInternal::Rectangle_overlaps_Rectangle(
    const SGRectangle2DInternal& rectangle1, const SGRectangle2DInternal& rectangle2,
    fixed p_margin, OverlapInfo* p_info)
{
    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;

    if (!sat_test(rectangle1, rectangle2, rectangle1.get_global_axes(), p_margin,
                  best_separation_vector, best_separation_length, collision_normal))
    {
        return false;
    }

    if (!sat_test(rectangle1, rectangle2, rectangle2.get_global_axes(), p_margin,
                  best_separation_vector, best_separation_length, collision_normal))
    {
        return false;
    }

    if (p_info)
    {
        p_info->separation = best_separation_vector;
        p_info->collision_normal = collision_normal;
    }

    return true;
}

bool SGCollisionDetector2DInternal::Circle_overlaps_Circle(const SGCircle2DInternal& circle1,
                                                           const SGCircle2DInternal& circle2,
                                                           fixed p_margin, OverlapInfo* p_info)
{
    SGFixedTransform2DInternal t1 = circle1.get_global_transform();
    SGFixedTransform2DInternal t2 = circle2.get_global_transform();

    SGFixedVector2Internal line = t1.get_origin() - t2.get_origin();
    fixed line_length = line.length();
    // We only multiply by the scale.x because we don't support non-uniform scaling.
    fixed combined_radius = (circle1.get_radius() * t1.get_scale().x +
                             circle2.get_radius() * t2.get_scale().x) +
                            p_margin;

    // A separation of fixed(1) is not considered an overlap
    // as in further computations normalized_vector * fixed(1) = (0, 0)
    fixed separation = combined_radius - line_length;
    bool overlapping;
    SGFixedVector2Internal collision_normal;
    SGFixedVector2Internal separation_vector;
    if (line_length == fixed::ZERO)
    {
        // If circles share an origin, then we arbitrarily decide that we
        // separate them by moving up.
        overlapping = true;
        collision_normal = SGFixedVector2Internal(fixed::ZERO, fixed::NEG_ONE);
        separation_vector = collision_normal * separation;
    }
    else
    {
        collision_normal = line.normalized();
        separation_vector = collision_normal * separation;
        // if the correction is too small it will be set to 0 and the overlap won't be corrected
        // in that case we don't consider it an overlap
        overlapping = (separation > fixed::ZERO) &&
                      (collision_normal.x == fixed::ZERO || separation_vector.x != fixed::ZERO) &&
                      (collision_normal.y == fixed::ZERO || separation_vector.y != fixed::ZERO);
    }

    if (overlapping && p_info)
    {
        p_info->collision_normal = collision_normal;
        p_info->separation = separation_vector;
    }

    return overlapping;
}

bool SGCollisionDetector2DInternal::Circle_overlaps_Rectangle(const SGCircle2DInternal& circle,
                                                              const SGRectangle2DInternal& rectangle,
                                                              fixed p_margin, OverlapInfo* p_info)
{
    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;

    // First, we see if the circle has any seperation from the rectangle's axes.
    if (!sat_test(rectangle, circle, rectangle.get_global_axes(), p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    // Next, we need to find the axis to check for the circle (it's a vector
    // from the closest vertex to the circle center).

    std::vector<SGFixedVector2Internal> vertices = rectangle.get_global_vertices();
    SGFixedTransform2DInternal ct = circle.get_global_transform();
    SGFixedVector2Internal closest_vertex = vertices[0];
    fixed closest_distance = (ct.get_origin() - vertices[0]).length();

    for (std::size_t i = 1; i < vertices.size(); i++)
    {
        fixed distance = (ct.get_origin() - vertices[i]).length();
        if (distance < closest_distance)
        {
            closest_distance = distance;
            closest_vertex = vertices[i];
        }
    }

    std::vector<SGFixedVector2Internal> circle_axes;
    circle_axes.push_back((ct.get_origin() - closest_vertex).normalized());
    if (!sat_test(rectangle, circle, circle_axes, p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    if (p_info)
    {
        p_info->collision_normal = -collision_normal;
        p_info->separation = -best_separation_vector;
    }

    return true;
}

bool SGCollisionDetector2DInternal::Polygon_overlaps_Polygon(const SGPolygon2DInternal& polygon1,
                                                             const SGPolygon2DInternal& polygon2,
                                                             fixed p_margin, OverlapInfo* p_info)
{
    if (polygon1.get_points().size() < 3)
        return false;
    if (polygon2.get_points().size() < 3)
        return false;

    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;

    if (!sat_test(polygon1, polygon2, polygon1.get_global_axes(), p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    if (!sat_test(polygon1, polygon2, polygon2.get_global_axes(), p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    if (p_info)
    {
        p_info->collision_normal = collision_normal;
        p_info->separation = best_separation_vector;
    }

    return true;
}

bool SGCollisionDetector2DInternal::Polygon_overlaps_Circle(const SGPolygon2DInternal& polygon,
                                                            const SGCircle2DInternal& circle,
                                                            fixed p_margin, OverlapInfo* p_info)
{
    if (polygon.get_points().size() < 3)
        return false;

    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;

    // First, we see if the circle has any seperation from the polygon's axes.
    if (!sat_test(polygon, circle, polygon.get_global_axes(), p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    // Next, we need to find the axis to check for the circle (it's a vector
    // from the closest vertex to the circle center).

    std::vector<SGFixedVector2Internal> vertices = polygon.get_global_vertices();
    SGFixedTransform2DInternal ct = circle.get_global_transform();
    SGFixedVector2Internal closest_vertex = vertices[0];
    fixed closest_distance = (ct.get_origin() - vertices[0]).length();

    for (std::size_t i = 1; i < vertices.size(); i++)
    {
        fixed distance = (ct.get_origin() - vertices[i]).length();
        if (distance < closest_distance)
        {
            closest_distance = distance;
            closest_vertex = vertices[i];
        }
    }

    std::vector<SGFixedVector2Internal> circle_axes;
    circle_axes.push_back((ct.get_origin() - closest_vertex).normalized());
    if (!sat_test(polygon, circle, circle_axes, p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    if (p_info)
    {
        p_info->collision_normal = collision_normal;
        p_info->separation = best_separation_vector;
    }

    return true;
}

bool SGCollisionDetector2DInternal::Polygon_overlaps_Rectangle(
    const SGPolygon2DInternal& polygon, const SGRectangle2DInternal& rectangle, fixed p_margin,
    OverlapInfo* p_info)
{
    if (polygon.get_points().size() < 3)
        return false;

    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;

    if (!sat_test(polygon, rectangle, polygon.get_global_axes(), p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    if (!sat_test(polygon, rectangle, rectangle.get_global_axes(), p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    if (p_info)
    {
        p_info->collision_normal = collision_normal;
        p_info->separation = best_separation_vector;
    }

    return true;
}

bool SGCollisionDetector2DInternal::Capsule_overlaps_Circle(const SGCapsule2DInternal& capsule,
                                                            const SGCircle2DInternal& circle,
                                                            fixed p_margin, OverlapInfo* p_info)
{
    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;
    std::vector<SGFixedVector2Internal> axes;
    const SGFixedTransform2DInternal capsule_transform = capsule.get_global_transform();
    // Capsule width axis
    axes.push_back(capsule_transform.elements[0].normalized());

    // Center of the circle to capsule internal endpoints
    const std::vector<SGFixedVector2Internal> verts = capsule.get_global_vertices();
    const SGFixedVector2Internal circle_center = circle.get_global_transform().get_origin();
    axes.push_back((circle_center - verts[0]).normalized());
    axes.push_back((circle_center - verts[1]).normalized());
    if (!sat_test(capsule, circle, axes, p_margin, best_separation_vector, best_separation_length,
                  collision_normal))
    {
        return false;
    }

    if (p_info)
    {
        p_info->collision_normal = collision_normal;
        p_info->separation = best_separation_vector;
    }

    return true;
}

bool SGCollisionDetector2DInternal::Capsule_overlaps_Rectangle(
    const SGCapsule2DInternal& capsule, const SGRectangle2DInternal& rectangle, fixed p_margin,
    OverlapInfo* p_info)
{
    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;
    const SGFixedTransform2DInternal capsule_transform = capsule.get_global_transform();
    // Rectangle edges
    if (!sat_test(capsule, rectangle, rectangle.get_global_axes(), p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    std::vector<SGFixedVector2Internal> axes;
    // Capsule width axis
    axes.push_back(capsule_transform.elements[0].normalized());

    // Rectangle vertex to capsule internal endpoints
    const std::vector<SGFixedVector2Internal> capsule_endpoints = capsule.get_global_vertices();
    axes.push_back(
        (rectangle.get_closest_vertex(capsule_endpoints[0]) - capsule_endpoints[0]).normalized());
    axes.push_back(
        (rectangle.get_closest_vertex(capsule_endpoints[1]) - capsule_endpoints[1]).normalized());

    if (!sat_test(capsule, rectangle, axes, p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    if (p_info)
    {
        p_info->collision_normal = collision_normal;
        p_info->separation = best_separation_vector;
    }

    return true;
}

bool SGCollisionDetector2DInternal::Capsule_overlaps_Polygon(const SGCapsule2DInternal& capsule,
                                                             const SGPolygon2DInternal& polygon,
                                                             fixed p_margin, OverlapInfo* p_info)
{
    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;
    const SGFixedTransform2DInternal capsule_transform = capsule.get_global_transform();
    // Polygon edges
    if (!sat_test(capsule, polygon, polygon.get_global_axes(), p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    std::vector<SGFixedVector2Internal> axes;
    // Capsule width axis
    axes.push_back(capsule_transform.elements[0].normalized());
    if (!sat_test(capsule, polygon, axes, p_margin, best_separation_vector, best_separation_length,
                  collision_normal))
    {
        return false;
    }

    // Polygon vertices to capsule internal endpoints
    const std::vector<SGFixedVector2Internal> capsule_endpoints = capsule.get_global_vertices();
    const std::vector<SGFixedVector2Internal> polygon_vertices = polygon.get_global_vertices();
    for (std::size_t i = 0; i < polygon_vertices.size(); i++)
    {
        axes.clear();
        axes.push_back((polygon_vertices[i] - capsule_endpoints[0]).normalized());
        axes.push_back((polygon_vertices[i] - capsule_endpoints[1]).normalized());

        if (!sat_test(capsule, polygon, axes, p_margin, best_separation_vector,
                      best_separation_length, collision_normal))
        {
            return false;
        }
    }

    if (p_info)
    {
        p_info->collision_normal = collision_normal;
        p_info->separation = best_separation_vector;
    }

    return true;
}

bool SGCollisionDetector2DInternal::Capsule_overlaps_Capsule(const SGCapsule2DInternal& capsule1,
                                                             const SGCapsule2DInternal& capsule2,
                                                             fixed p_margin, OverlapInfo* p_info)
{
    SGFixedVector2Internal best_separation_vector;
    fixed best_separation_length;
    SGFixedVector2Internal collision_normal;
    std::vector<SGFixedVector2Internal> axes;
    // Capsule width axis
    axes.push_back(capsule1.get_global_transform().elements[0].normalized());
    axes.push_back(capsule2.get_global_transform().elements[0].normalized());
    if (!sat_test(capsule1, capsule2, axes, p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    // Internal endpoints to internal endpoints
    const std::vector<SGFixedVector2Internal> capsule1_endpoints = capsule1.get_global_vertices();
    const std::vector<SGFixedVector2Internal> capsule2_endpoints = capsule2.get_global_vertices();
    axes.clear();
    axes.push_back((capsule1_endpoints[0] - capsule2_endpoints[0]).normalized());
    axes.push_back((capsule1_endpoints[0] - capsule2_endpoints[1]).normalized());
    axes.push_back((capsule1_endpoints[1] - capsule2_endpoints[0]).normalized());
    axes.push_back((capsule1_endpoints[1] - capsule2_endpoints[1]).normalized());

    if (!sat_test(capsule1, capsule2, axes, p_margin, best_separation_vector,
                  best_separation_length, collision_normal))
    {
        return false;
    }

    if (p_info)
    {
        p_info->collision_normal = collision_normal;
        p_info->separation = best_separation_vector;
    }

    return true;
}

bool SGCollisionDetector2DInternal::segment_intersects_Capsule(
    const SGFixedVector2Internal& p_start, const SGFixedVector2Internal& p_cast_to,
    const SGCapsule2DInternal& capsule, SGFixedVector2Internal& p_intersection_point,
    SGFixedVector2Internal& p_collision_normal)
{
    SGCircle2DInternal circle(capsule.get_radius());
    auto t = capsule.get_global_transform();
    t.set_origin(capsule.get_global_vertices()[0]);
    circle.set_transform(t);
    SGFixedVector2Internal intersection_point;
    SGFixedVector2Internal collision_normal;
    fixed closest_distance_squared;
    bool colliding = false;
    if (segment_intersects_Circle(p_start, p_cast_to, circle, intersection_point, collision_normal))
    {
        closest_distance_squared = intersection_point.distance_squared_to(p_start);
        p_intersection_point = intersection_point;
        p_collision_normal = collision_normal;
        colliding = true;
    }
    t.set_origin(capsule.get_global_vertices()[1]);
    circle.set_transform(t);
    if (segment_intersects_Circle(p_start, p_cast_to, circle, intersection_point, collision_normal))
    {
        fixed distance_squared = intersection_point.distance_squared_to(p_start);
        if (!colliding || closest_distance_squared > distance_squared)
        {
            closest_distance_squared = distance_squared;
            p_intersection_point = intersection_point;
            p_collision_normal = collision_normal;
        }
        colliding = true;
    }
    SGRectangle2DInternal rectangle(capsule.get_radius(), fixed(capsule.get_height().value / 2));
    rectangle.set_transform(capsule.get_global_transform());
    if (segment_intersects_Polygon(p_start, p_cast_to, rectangle, intersection_point,
                                   collision_normal))
    {
        fixed distance_squared = intersection_point.distance_squared_to(p_start);
        if (!colliding || closest_distance_squared > distance_squared)
        {
            p_intersection_point = intersection_point;
            p_collision_normal = collision_normal;
        }
        colliding = true;
    }

    return colliding;
}

// Algorithm from https://stackoverflow.com/a/565282
//
// License: CC BY-SA 3.0
// Author: Gareth Rees
//
// p = p_start_1
// r = p_cast_to_1
// q = p_start_2
// s = p_cast_to_2
bool SGCollisionDetector2DInternal::segment_intersects_segment(
    const SGFixedVector2Internal& p_start_1, const SGFixedVector2Internal& p_cast_to_1,
    const SGFixedVector2Internal& p_start_2, const SGFixedVector2Internal& p_cast_to_2,
    SGFixedVector2Internal& p_intersection_point)
{
    fixed denominator = p_cast_to_1.cross(p_cast_to_2);
    fixed u_nominator = (p_start_2 - p_start_1).cross(p_cast_to_1);

    if (denominator == fixed::ZERO && u_nominator == fixed::ZERO)
    {
        // Line segments are collinear.
        //
        // They could overlap, but since we are always dealing with polygons,
        // we know that there will always be another edge that shares one of
        // the 2nd line segments end points, so we can trust that we'll
        // collide with that edge (so long as there aren't two collinear edges).
        return false;
    }

    if (denominator == fixed::ZERO)
    {
        // Line segments are parallel and so non-intersecting.
        return false;
    }

    fixed u = u_nominator / denominator;
    if (u < fixed::ZERO || u > fixed::ONE)
    {
        // Intersection would happen before the start or after the end of the 2nd segment.
        return false;
    }

    fixed t = (p_start_2 - p_start_1).cross(p_cast_to_2) / denominator;
    if (t < fixed::ZERO || t > fixed::ONE)
    {
        // Intersection would happen before the start or after the end of the 1st segment.
        return false;
    }

    p_intersection_point = p_start_1 + (p_cast_to_1 * t);

    return true;
}

bool SGCollisionDetector2DInternal::segment_intersects_Polygon(
    const SGFixedVector2Internal& p_start, const SGFixedVector2Internal& p_cast_to,
    const SGShape2DInternal& polygon, SGFixedVector2Internal& p_intersection_point,
    SGFixedVector2Internal& p_collision_normal)
{
    std::vector<SGFixedVector2Internal> verts = polygon.get_global_vertices();

    bool intersecting = false;

    SGFixedVector2Internal closest_intersection_point;
    SGFixedVector2Internal closest_collision_normal;
    fixed closest_distance;

    SGFixedVector2Internal previous = verts[verts.size() - 1];
    for (std::size_t i = 0; i < verts.size(); i++)
    {
        SGFixedVector2Internal cur = verts[i];
        SGFixedVector2Internal edge = cur - previous;
        SGFixedVector2Internal intersection_point;
        if (segment_intersects_segment(p_start, p_cast_to, previous, edge, intersection_point))
        {
            fixed distance = (intersection_point - p_start).length();
            if (!intersecting || distance < closest_distance)
            {
                closest_distance = distance;
                closest_intersection_point = intersection_point;
                closest_collision_normal = SGFixedVector2Internal(edge.y, -edge.x).normalized();
            }
            intersecting = true;
        }
        previous = cur;
    }

    if (intersecting)
    {
        p_collision_normal = closest_collision_normal;
        p_intersection_point = closest_intersection_point;
    }

    return intersecting;
}

// Algorithm from https://stackoverflow.com/a/1084899
//
// License: CC BY-SA 4.0
// Original author: bobobobo
//
// The code has been adapted to this engine and fixed-point path.
//
// E = p_start
// d = p_cast_to
bool SGCollisionDetector2DInternal::segment_intersects_Circle(
    const SGFixedVector2Internal& p_start, const SGFixedVector2Internal& p_cast_to,
    const SGCircle2DInternal& circle, SGFixedVector2Internal& p_intersection_point,
    SGFixedVector2Internal& p_collision_normal)
{
    SGFixedTransform2DInternal ct = circle.get_global_transform();

    SGFixedVector2Internal C = ct.get_origin();
    SGFixedVector2Internal f = p_start - C;

    fixed r = circle.get_radius() * ct.get_scale().x;

    fixed a = p_cast_to.dot(p_cast_to);
    if (a == fixed::ZERO)
    {
        // This would lead to division by zero.
        return false;
    }

    fixed b = fixed::TWO * f.dot(p_cast_to);
    fixed c = f.dot(f) - (r * r);

    // Reduce precision - we're just going to use whole integers to calculate
    // the determinant, in an attempt to avoid overflowing 64-bits.
    int64_t small_a = a.value >> 16;
    int64_t small_b = b.value >> 16;
    int64_t small_c = c.value >> 16;

    int64_t small_discriminant = (small_b * small_b) - (4 * small_a * small_c);
    if (small_discriminant < 0)
    {
        // No intersection.
        return false;
    }
    else
    {
        // Get the square root and return to our normal precision.
        fixed discriminant(sg_sqrt_64(small_discriminant) << 16);

        fixed t1 = (-b - discriminant) / (fixed::TWO * a);
        fixed t2 = (-b + discriminant) / (fixed::TWO * a);

        // This is where we're outside the circle, and intersect at least once.
        if (t1 >= fixed::ZERO && t1 <= fixed::ONE)
        {
            p_intersection_point = p_start + p_cast_to * t1;
            p_collision_normal = (p_intersection_point - C).normalized();
            return true;
        }

        // This is where we're inside the circle, and intersect the outer edge.
        if (t2 >= fixed::ZERO && t2 <= fixed::ONE)
        {
            p_intersection_point = p_start + p_cast_to * t2;
            p_collision_normal = (p_intersection_point - C).normalized();
            return true;
        }
    }

    return false;
}
