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

#ifndef SG_FIXED_TRANSFORM_2D_H
#define SG_FIXED_TRANSFORM_2D_H

#include <godot_cpp/classes/ref_counted.hpp>

#include "internal/sg_fixed_transform_2d_internal.h"
#include "sg_fixed_vector2.h"

using namespace godot;

class SGFixedTransform2D : public RefCounted
{
    GDCLASS(SGFixedTransform2D, RefCounted);

    Ref<SGFixedVector2> x;
    Ref<SGFixedVector2> y;
    Ref<SGFixedVector2> origin;

protected:
    static void _bind_methods();

public:
    _FORCE_INLINE_ Ref<SGFixedVector2> get_x() const
    {
        return x;
    }

    void set_x(const Ref<SGFixedVector2>& p_x);

    _FORCE_INLINE_ Ref<SGFixedVector2> get_y() const
    {
        return y;
    }

    void set_y(const Ref<SGFixedVector2>& p_y);

    _FORCE_INLINE_ Ref<SGFixedVector2> get_origin() const
    {
        return origin;
    }

    void set_origin(const Ref<SGFixedVector2>& p_origin);

    _FORCE_INLINE_ SGFixedTransform2DInternal get_internal() const
    {
        return SGFixedTransform2DInternal(fixed(x->get_x()), fixed(x->get_y()), fixed(y->get_x()),
                                          fixed(y->get_y()), fixed(origin->get_x()),
                                          fixed(origin->get_y()));
    }

    _FORCE_INLINE_ void set_internal(const SGFixedTransform2DInternal& p_internal)
    {
        x->set_x(p_internal[0][0].value);
        x->set_y(p_internal[0][1].value);
        y->set_x(p_internal[1][0].value);
        y->set_y(p_internal[1][1].value);
        origin->set_x(p_internal[2][0].value);
        origin->set_y(p_internal[2][1].value);
    }

    _FORCE_INLINE_ static Ref<SGFixedTransform2D> from_internal(
        const SGFixedTransform2DInternal& p_internal)
    {
        return Ref<SGFixedTransform2D>(memnew(SGFixedTransform2D(p_internal)));
    }

    Transform2D to_float() const;
    void from_float(const Transform2D& p_float_transform);

    Ref<SGFixedTransform2D> copy() const;

    Ref<SGFixedTransform2D> inverse() const;
    Ref<SGFixedTransform2D> affine_inverse() const;

    int64_t get_rotation() const;
    Ref<SGFixedTransform2D> rotated(int64_t p_radians) const;

    Ref<SGFixedVector2> get_scale() const;
    Ref<SGFixedTransform2D> scaled(const Ref<SGFixedVector2>& p_scale) const;
    Ref<SGFixedTransform2D> translated(const Ref<SGFixedVector2>& p_offset) const;

    Ref<SGFixedTransform2D> orthonormalized() const;
    bool is_equal_approx(const Ref<SGFixedTransform2D>& p_transform) const;

    Ref<SGFixedTransform2D> mul(const Ref<SGFixedTransform2D>& p_transform) const;

    Ref<SGFixedTransform2D> interpolate_with(const Ref<SGFixedTransform2D>& p_transform,
                                             int64_t p_weight) const;

    Ref<SGFixedVector2> basis_xform(const Ref<SGFixedVector2>& p_vec) const;
    Ref<SGFixedVector2> basis_xform_inv(const Ref<SGFixedVector2>& p_vec) const;
    Ref<SGFixedVector2> xform(const Ref<SGFixedVector2>& p_vec) const;
    Ref<SGFixedVector2> xform_inv(const Ref<SGFixedVector2>& p_vec) const;

    SGFixedTransform2D();
    SGFixedTransform2D(const SGFixedTransform2DInternal& p_internal);
};

#endif
