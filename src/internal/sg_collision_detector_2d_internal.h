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

#ifndef SG_COLLISION_DETECTOR_2D_INTERNAL_H
#define SG_COLLISION_DETECTOR_2D_INTERNAL_H

#include <vector>

#include "sg_fixed_rect2_internal.h"
#include "sg_fixed_transform_2d_internal.h"
#include "sg_shapes_2d_internal.h"

class SGCollisionDetector2DInternal
{
public:
    //
    // Rectangles
    //

    struct Interval
    {
        fixed min;
        fixed max;
    };

    struct OverlapInfo
    {
        SGFixedVector2Internal separation;
        SGFixedVector2Internal collision_normal;
    };

    static bool AABB_overlaps_AABB(const SGFixedRect2Internal& aabb1,
                                   const SGFixedRect2Internal& aabb2);

    //
    // SAT testing utilities
    //

    static Interval get_interval(const SGShape2DInternal& shape, const SGFixedVector2Internal& axis,
                                 fixed p_margin);

    static bool overlaps_on_axis(const SGShape2DInternal& shape1, const SGShape2DInternal& shape2,
                                 const SGFixedVector2Internal& axis, fixed p_margin,
                                 fixed& separation);
    static bool sat_test(const SGShape2DInternal& shape1, const SGShape2DInternal& shape2,
                         const std::vector<SGFixedVector2Internal>& axes, fixed p_margin,
                         SGFixedVector2Internal& best_separation_vector,
                         fixed& best_separation_length, SGFixedVector2Internal& collision_normal);
    //
    // Rectangles
    //

    static bool Rectangle_overlaps_Rectangle(const SGRectangle2DInternal& rectangle1,
                                             const SGRectangle2DInternal& rectangle2,
                                             fixed p_margin, OverlapInfo* p_info = nullptr);

    //
    // Circles
    //

    static bool Circle_overlaps_Circle(const SGCircle2DInternal& circle1,
                                       const SGCircle2DInternal& circle2, fixed p_margin,
                                       OverlapInfo* p_info = nullptr);
    static bool Circle_overlaps_Rectangle(const SGCircle2DInternal& circle,
                                          const SGRectangle2DInternal& rectangle, fixed p_margin,
                                          OverlapInfo* p_info = nullptr);

    //
    // Polygons
    //

    static bool Polygon_overlaps_Polygon(const SGPolygon2DInternal& polygon1,
                                         const SGPolygon2DInternal& polygon2, fixed p_margin,
                                         OverlapInfo* p_info = nullptr);
    static bool Polygon_overlaps_Circle(const SGPolygon2DInternal& polygon,
                                        const SGCircle2DInternal& circle, fixed p_margin,
                                        OverlapInfo* p_info = nullptr);
    static bool Polygon_overlaps_Rectangle(const SGPolygon2DInternal& polygon,
                                           const SGRectangle2DInternal& rectangle, fixed p_margin,
                                           OverlapInfo* p_info = nullptr);

    //
    // Capsules
    //

    static bool Capsule_overlaps_Circle(const SGCapsule2DInternal& capsule,
                                        const SGCircle2DInternal& circle, fixed p_margin,
                                        OverlapInfo* p_info = nullptr);
    static bool Capsule_overlaps_Rectangle(const SGCapsule2DInternal& capsule,
                                           const SGRectangle2DInternal& rectangle, fixed p_margin,
                                           OverlapInfo* p_info);
    static bool Capsule_overlaps_Polygon(const SGCapsule2DInternal& capsule,
                                         const SGPolygon2DInternal& polygon, fixed p_margin,
                                         OverlapInfo* p_info);
    static bool Capsule_overlaps_Capsule(const SGCapsule2DInternal& capsule1,
                                         const SGCapsule2DInternal& capsule2, fixed p_margin,
                                         OverlapInfo* p_info);

    //
    // Line segments
    //

    static bool segment_intersects_segment(
        const SGFixedVector2Internal& p_start_1, const SGFixedVector2Internal& p_cast_to_1,
        const SGFixedVector2Internal& p_start_2, const SGFixedVector2Internal& p_cast_to_2,
        SGFixedVector2Internal& p_intersection_point);
    // This can handle either SGRectangle2DInternal or SGPolygon2DInternal,
    // since they are both polygons with connected vertices returned by
    // get_global_vertices().
    static bool segment_intersects_Polygon(
        const SGFixedVector2Internal& p_start, const SGFixedVector2Internal& p_cast_to,
        const SGShape2DInternal& polygon, SGFixedVector2Internal& p_intersection_point,
        SGFixedVector2Internal& p_collision_normal);
    static bool segment_intersects_Circle(
        const SGFixedVector2Internal& p_start, const SGFixedVector2Internal& p_cast_to,
        const SGCircle2DInternal& circle, SGFixedVector2Internal& p_intersection_point,
        SGFixedVector2Internal& p_collision_normal);
    static bool segment_intersects_Capsule(
        const SGFixedVector2Internal& p_start, const SGFixedVector2Internal& p_cast_to,
        const SGCapsule2DInternal& capsule, SGFixedVector2Internal& p_intersection_point,
        SGFixedVector2Internal& p_collision_normal);
};

#endif
