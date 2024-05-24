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

#ifndef SG_RAY_CAST_2D_H
#define SG_RAY_CAST_2D_H

#include <godot_cpp/classes/object.hpp>

#include "internal/sg_bodies_2d_internal.h"
#include "math/sg_fixed_vector2.h"
#include "sg_fixed_node_2d.h"

class SGRayCast2D : public SGFixedNode2D
{
    GDCLASS(SGRayCast2D, SGFixedNode2D);

    RID world_rid;

    Ref<SGFixedVector2> cast_to;
    uint32_t collision_mask;
    bool collide_with_areas = false;
    bool collide_with_bodies = true;

    bool colliding;
    ObjectID collider;
    RID collider_rid;
    Ref<SGFixedVector2> collision_point;
    Ref<SGFixedVector2> collision_normal;

    Array exceptions;

protected:
    static void _bind_methods();
    void _notification(int p_what);

    int64_t _get_cast_to_x() const;
    void _set_cast_to_x(int64_t p_x);
    int64_t _get_cast_to_y() const;
    void _set_cast_to_y(int64_t p_y);

public:
    Ref<SGFixedVector2> get_cast_to() const;
    void set_cast_to(const Ref<SGFixedVector2>& p_cast_to);

    uint32_t get_collision_mask() const;
    void set_collision_mask(uint32_t p_collision_mask);

    void set_collision_mask_bit(int p_bit, bool p_value);

    void set_collide_with_areas(bool p_collide_with_areas);
    bool get_collide_with_areas();
    void set_collide_with_bodies(bool p_collide_with_bodies);
    bool get_collide_with_bodies();

    void update_raycast_collision();

    bool is_colliding() const;
    Object* get_collider() const;
    RID get_collider_rid() const;
    Ref<SGFixedVector2> get_collision_point() const;
    Ref<SGFixedVector2> get_collision_normal() const;

    void add_exception(const Object* p_object);
    void remove_exception(const Object* p_object);
    Array get_exceptions() const;
    void set_exceptions(const Array& p_exceptions);
    void clear_exceptions();

    RID get_world() const
    {
        return world_rid;
    }

    void set_world(RID p_world);

    SGRayCast2D();
    ~SGRayCast2D();
};

#endif
