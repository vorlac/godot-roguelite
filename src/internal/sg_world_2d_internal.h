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

#ifndef SG_WORLD_2D_INTERNAL_H
#define SG_WORLD_2D_INTERNAL_H

#include <unordered_set>
#include <vector>

#include "sg_fixed_rect2_internal.h"
#include "sg_fixed_vector2_internal.h"
#include "sg_result_handler_internal.h"

class SGArea2DInternal;
class SGBody2DInternal;
class SGCollisionObject2DInternal;
class SGShape2DInternal;
class SGBroadphase2DInternal;

class SGWorld2DInternal
{
public:
    typedef bool (*CompareCallback)(const SGCollisionObject2DInternal*,
                                    const SGCollisionObject2DInternal*);

protected:
    std::vector<SGArea2DInternal*> areas;
    std::vector<SGBody2DInternal*> bodies;
    SGBroadphase2DInternal* broadphase;
    CompareCallback compare_callback;

public:
    struct ShapeOverlapInfo
    {
        SGShape2DInternal* shape;
        SGFixedVector2Internal separation;
        SGFixedVector2Internal collision_normal;

        ShapeOverlapInfo()
        {
            shape = nullptr;
        }
    };

    struct BodyOverlapInfo
    {
        SGCollisionObject2DInternal* collider;
        SGShape2DInternal* collider_shape;
        SGShape2DInternal* local_shape;
        SGFixedVector2Internal separation;
        SGFixedVector2Internal collision_normal;

        BodyOverlapInfo()
        {
            collider = nullptr;
            collider_shape = nullptr;
            local_shape = nullptr;
        }
    };

    struct BodyCollisionInfo
    {
        SGCollisionObject2DInternal* collider;
        // @todo How can we get the shape in here?
        SGFixedVector2Internal normal;
        SGFixedVector2Internal remainder;

        BodyCollisionInfo()
        {
            collider = nullptr;
        }
    };

    struct RayCastInfo
    {
        SGBody2DInternal* body;
        SGFixedVector2Internal collision_point;
        SGFixedVector2Internal collision_normal;

        RayCastInfo()
        {
            body = nullptr;
        }
    };

    _FORCE_INLINE_ const std::vector<SGBody2DInternal*>& get_bodies() const
    {
        return bodies;
    }

    _FORCE_INLINE_ const std::vector<SGArea2DInternal*>& get_areas() const
    {
        return areas;
    }

    _FORCE_INLINE_ const SGBroadphase2DInternal* get_broadphase() const
    {
        return broadphase;
    }

    void add_area(SGArea2DInternal* p_area);
    void remove_area(SGArea2DInternal* p_area);
    void add_body(SGBody2DInternal* p_body);
    void remove_body(SGBody2DInternal* p_body);

    bool overlaps(SGCollisionObject2DInternal* p_object1, SGCollisionObject2DInternal* p_object2,
                  fixed p_margin, BodyOverlapInfo* p_info = nullptr) const;
    bool overlaps(SGShape2DInternal* p_shape1, SGShape2DInternal* p_shape2, fixed p_margin,
                  ShapeOverlapInfo* p_info = nullptr) const;

    void get_overlapping_areas(SGCollisionObject2DInternal* p_object,
                               SGResultHandlerInternal* p_result_handler) const;
    void get_overlapping_bodies(SGCollisionObject2DInternal* p_object,
                                SGResultHandlerInternal* p_result_handler) const;

    bool get_best_overlapping_body(SGBody2DInternal* p_body, bool p_use_safe_margin,
                                   BodyOverlapInfo* p_info) const;
    bool unstuck_body(SGBody2DInternal* p_body, int p_max_attempts,
                      BodyOverlapInfo* p_info = nullptr) const;
    bool move_and_collide(SGBody2DInternal* p_body, const SGFixedVector2Internal& p_linear_velocity,
                          BodyCollisionInfo* p_collision = nullptr) const;

    bool segment_intersects_shape(
        const SGFixedVector2Internal& p_start, const SGFixedVector2Internal& p_cast_to,
        SGShape2DInternal* p_shape, SGFixedVector2Internal& p_intersection_point,
        SGFixedVector2Internal& p_collision_normal) const;
    bool cast_ray(const SGFixedVector2Internal& p_start, const SGFixedVector2Internal& p_cast_to,
                  uint32_t p_collision_mask,
                  std::unordered_set<SGCollisionObject2DInternal*>* p_exceptions = nullptr,
                  bool collide_with_areas = false, bool collide_with_bodies = true,
                  RayCastInfo* p_info = nullptr) const;

    SGWorld2DInternal(unsigned int p_broadphase_cell_size,
                      CompareCallback p_compare_callback = nullptr);
    ~SGWorld2DInternal();
};

#endif
