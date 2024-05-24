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

#ifndef SG_FIXED_TRANSFORM_2D_INTERNAL_H
#define SG_FIXED_TRANSFORM_2D_INTERNAL_H

#include "sg_fixed_vector2_internal.h"

struct SGFixedTransform2DInternal
{
    SGFixedVector2Internal elements[3];

    _FORCE_INLINE_ fixed tdotx(const SGFixedVector2Internal& v) const
    {
        return elements[0][0] * v.x + elements[1][0] * v.y;
    }

    _FORCE_INLINE_ fixed tdoty(const SGFixedVector2Internal& v) const
    {
        return elements[0][1] * v.x + elements[1][1] * v.y;
    }

    const SGFixedVector2Internal& operator[](int p_idx) const
    {
        return elements[p_idx];
    }

    SGFixedVector2Internal& operator[](int p_idx)
    {
        return elements[p_idx];
    }

    _FORCE_INLINE_ SGFixedVector2Internal get_axis(int p_axis) const
    {
        ERR_FAIL_INDEX_V(p_axis, 3, SGFixedVector2Internal());
        return elements[p_axis];
    }

    _FORCE_INLINE_ void set_axis(int p_axis, const SGFixedVector2Internal& p_vec)
    {
        ERR_FAIL_INDEX(p_axis, 3);
        elements[p_axis] = p_vec;
    }

    void invert();
    SGFixedTransform2DInternal inverse() const;

    void affine_invert();
    SGFixedTransform2DInternal affine_inverse() const;

    void set_rotation(fixed p_rot);
    fixed get_rotation() const;
    _FORCE_INLINE_ void set_rotation_and_scale(fixed p_rot, const SGFixedVector2Internal& p_scale);
    void rotate(fixed p_phi);

    void scale(const SGFixedVector2Internal& p_scale);
    void scale_basis(const SGFixedVector2Internal& p_scale);
    void translate(fixed p_tx, fixed p_ty);
    void translate(const SGFixedVector2Internal& p_translation);

    fixed basis_determinant() const;

    SGFixedVector2Internal get_scale() const;
    void set_scale(const SGFixedVector2Internal& p_scale);

    _FORCE_INLINE_ const SGFixedVector2Internal& get_origin() const
    {
        return elements[2];
    }

    _FORCE_INLINE_ void set_origin(const SGFixedVector2Internal& p_origin)
    {
        elements[2] = p_origin;
    }

    SGFixedTransform2DInternal scaled(const SGFixedVector2Internal& p_scale) const;
    SGFixedTransform2DInternal basis_scaled(const SGFixedVector2Internal& p_scale) const;
    SGFixedTransform2DInternal translated(const SGFixedVector2Internal& p_offset) const;
    SGFixedTransform2DInternal rotated(fixed p_phi) const;

    SGFixedTransform2DInternal untranslated() const;

    void orthonormalize();
    SGFixedTransform2DInternal orthonormalized() const;
    bool is_equal_approx(const SGFixedTransform2DInternal& p_transform) const;

    bool operator==(const SGFixedTransform2DInternal& p_transform) const;
    bool operator!=(const SGFixedTransform2DInternal& p_transform) const;

    void operator*=(const SGFixedTransform2DInternal& p_transform);
    SGFixedTransform2DInternal operator*(const SGFixedTransform2DInternal& p_transform) const;

    SGFixedTransform2DInternal interpolate_with(const SGFixedTransform2DInternal& p_transform,
                                                fixed p_c) const;

    _FORCE_INLINE_ SGFixedVector2Internal basis_xform(const SGFixedVector2Internal& p_vec) const;
    _FORCE_INLINE_ SGFixedVector2Internal basis_xform_inv(const SGFixedVector2Internal& p_vec) const;
    _FORCE_INLINE_ SGFixedVector2Internal xform(const SGFixedVector2Internal& p_vec) const;
    _FORCE_INLINE_ SGFixedVector2Internal xform_inv(const SGFixedVector2Internal& p_vec) const;

    //_FORCE_INLINE_ SGFixedRect2Internal xform(const SGFixedRect2Internal &p_rect) const;
    //_FORCE_INLINE_ SGFixedRect2Internal xform_inv(const SGFixedRect2Internal &p_rect) const;

    SGFixedTransform2DInternal(fixed xx, fixed xy, fixed yx, fixed yy, fixed ox, fixed oy)
    {
        elements[0][0] = xx;
        elements[0][1] = xy;
        elements[1][0] = yx;
        elements[1][1] = yy;
        elements[2][0] = ox;
        elements[2][1] = oy;
    }

    SGFixedTransform2DInternal(fixed p_rot, const SGFixedVector2Internal& p_pos);

    SGFixedTransform2DInternal()
    {
        elements[0][0] = fixed::ONE;
        elements[1][1] = fixed::ONE;
    }

    /*
    SGFixedTransform2DInternal(const SGFixedTransform2DInternal& p_other) {
        memcpy(&elements, &p_other.elements, sizeof(elements));
    }
    */
};

SGFixedVector2Internal SGFixedTransform2DInternal::basis_xform(const SGFixedVector2Internal& p_vec) const
{
    return SGFixedVector2Internal(tdotx(p_vec), tdoty(p_vec));
}

SGFixedVector2Internal SGFixedTransform2DInternal::basis_xform_inv(
    const SGFixedVector2Internal& p_vec) const
{
    return SGFixedVector2Internal(elements[0].dot(p_vec), elements[1].dot(p_vec));
}

SGFixedVector2Internal SGFixedTransform2DInternal::xform(const SGFixedVector2Internal& p_vec) const
{
    return SGFixedVector2Internal(tdotx(p_vec), tdoty(p_vec)) + elements[2];
}

SGFixedVector2Internal SGFixedTransform2DInternal::xform_inv(const SGFixedVector2Internal& p_vec) const
{
    SGFixedVector2Internal v = p_vec - elements[2];
    return SGFixedVector2Internal(elements[0].dot(v), elements[1].dot(v));
}

void SGFixedTransform2DInternal::set_rotation_and_scale(fixed p_rot,
                                                        const SGFixedVector2Internal& p_scale)
{
    elements[0][0] = p_rot.cos() * p_scale.x;
    elements[1][1] = p_rot.cos() * p_scale.y;
    elements[1][0] = -p_rot.sin() * p_scale.y;
    elements[0][1] = p_rot.sin() * p_scale.x;
}

#endif
