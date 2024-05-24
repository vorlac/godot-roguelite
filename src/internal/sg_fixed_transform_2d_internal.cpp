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

#include "sg_fixed_transform_2d_internal.h"

void SGFixedTransform2DInternal::invert()
{
    SWAP(elements[0][1], elements[1][0]);
    elements[2] = basis_xform(-elements[2]);
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::inverse() const
{
    SGFixedTransform2DInternal inv = *this;
    inv.invert();
    return inv;
}

void SGFixedTransform2DInternal::affine_invert()
{
    fixed det = basis_determinant();
#ifdef MATH_CHECKS
    ERR_FAIL_COND(det == fixed::ZERO);
#endif
    SWAP(elements[0][0], elements[1][1]);
    elements[0] /= SGFixedVector2Internal(det, -det);
    elements[1] /= SGFixedVector2Internal(-det, det);

    elements[2] = basis_xform(-elements[2]);
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::affine_inverse() const
{
    SGFixedTransform2DInternal inv = *this;
    inv.affine_invert();
    return inv;
}

void SGFixedTransform2DInternal::rotate(fixed p_phi)
{
    SGFixedVector2Internal scale = get_scale();
    *this = SGFixedTransform2DInternal(p_phi, SGFixedVector2Internal()) * (*this);
    set_scale(scale);
}

fixed SGFixedTransform2DInternal::get_rotation() const
{
    return elements[0].y.atan2(elements[0].x);
}

void SGFixedTransform2DInternal::set_rotation(fixed p_rot)
{
    SGFixedVector2Internal scale = get_scale();
    fixed cr = p_rot.cos();
    fixed sr = p_rot.sin();
    elements[0][0] = cr;
    elements[0][1] = sr;
    elements[1][0] = -sr;
    elements[1][1] = cr;
    set_scale(scale);
}

SGFixedTransform2DInternal::SGFixedTransform2DInternal(fixed p_rot,
                                                       const SGFixedVector2Internal& p_pos)
{
    fixed cr = p_rot.cos();
    fixed sr = p_rot.sin();
    elements[0][0] = cr;
    elements[0][1] = sr;
    elements[1][0] = -sr;
    elements[1][1] = cr;
    elements[2] = p_pos;
}

SGFixedVector2Internal SGFixedTransform2DInternal::get_scale() const
{
    fixed det_sign = FIXED_SGN(basis_determinant());
    return SGFixedVector2Internal(elements[0].length(), det_sign * elements[1].length());
}

void SGFixedTransform2DInternal::set_scale(const SGFixedVector2Internal& p_scale)
{
    elements[0].normalize();
    elements[1].normalize();

    // If scale is very nearly 1, then we just trust normalize() to do its magic.
    if (!fixed::is_equal_approx(p_scale.x, fixed::ONE, SGFixedVector2Internal::FIXED_UNIT_EPSILON))
        elements[0] = elements[0].safe_scale(p_scale.x);
    if (!fixed::is_equal_approx(p_scale.y, fixed::ONE, SGFixedVector2Internal::FIXED_UNIT_EPSILON))
        elements[1] = elements[1].safe_scale(p_scale.y);
}

void SGFixedTransform2DInternal::scale(const SGFixedVector2Internal& p_scale)
{
    scale_basis(p_scale);
    elements[2] *= p_scale;
}

void SGFixedTransform2DInternal::scale_basis(const SGFixedVector2Internal& p_scale)
{
    elements[0] = elements[0].safe_scale(p_scale);
    elements[1] = elements[1].safe_scale(p_scale);
}

void SGFixedTransform2DInternal::translate(fixed p_tx, fixed p_ty)
{
    translate(SGFixedVector2Internal(p_tx, p_ty));
}

void SGFixedTransform2DInternal::translate(const SGFixedVector2Internal& p_translation)
{
    elements[2] += basis_xform(p_translation);
}

void SGFixedTransform2DInternal::orthonormalize()
{
    SGFixedVector2Internal x = elements[0];
    SGFixedVector2Internal y = elements[1];

    x.normalized();
    y = (y - x * (x.dot(y)));
    y.normalize();

    elements[0] = x;
    elements[1] = y;
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::orthonormalized() const
{
    SGFixedTransform2DInternal t = *this;
    t.orthonormalize();
    return t;
}

bool SGFixedTransform2DInternal::is_equal_approx(const SGFixedTransform2DInternal& p_transform) const
{
    return elements[0].is_equal_approx(p_transform.elements[0]) &&
           elements[1].is_equal_approx(p_transform.elements[1]) &&
           elements[2].is_equal_approx(p_transform.elements[2]);
}

bool SGFixedTransform2DInternal::operator==(const SGFixedTransform2DInternal& p_transform) const
{
    for (int i = 0; i < 3; i++)
        if (elements[i] != p_transform.elements[i])
            return false;
    return true;
}

bool SGFixedTransform2DInternal::operator!=(const SGFixedTransform2DInternal& p_transform) const
{
    for (int i = 0; i < 3; i++)
        if (elements[i] != p_transform.elements[i])
            return true;
    return false;
}

void SGFixedTransform2DInternal::operator*=(const SGFixedTransform2DInternal& p_transform)
{
    elements[2] = xform(p_transform.elements[2]);

    fixed x0, x1, y0, y1;

    x0 = tdotx(p_transform.elements[0]);
    x1 = tdoty(p_transform.elements[0]);
    y0 = tdotx(p_transform.elements[1]);
    y1 = tdoty(p_transform.elements[1]);

    elements[0][0] = x0;
    elements[0][1] = x1;
    elements[1][0] = y0;
    elements[1][1] = y1;
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::operator*(
    const SGFixedTransform2DInternal& p_transform) const
{
    SGFixedTransform2DInternal t = *this;
    t *= p_transform;
    return t;
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::scaled(const SGFixedVector2Internal& p_scale) const
{
    SGFixedTransform2DInternal t = *this;
    t.scale(p_scale);
    return t;
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::basis_scaled(
    const SGFixedVector2Internal& p_scale) const
{
    SGFixedTransform2DInternal t = *this;
    t.scale_basis(p_scale);
    return t;
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::translated(
    const SGFixedVector2Internal& p_offset) const
{
    SGFixedTransform2DInternal t = *this;
    t.translate(p_offset);
    return t;
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::rotated(fixed p_phi) const
{
    SGFixedTransform2DInternal t = *this;
    t.rotate(p_phi);
    return t;
}

fixed SGFixedTransform2DInternal::basis_determinant() const
{
    return elements[0].x * elements[1].y - elements[0].y * elements[1].x;
}

SGFixedTransform2DInternal SGFixedTransform2DInternal::interpolate_with(
    const SGFixedTransform2DInternal& p_transform, fixed p_c) const
{
    SGFixedVector2Internal p1 = get_origin();
    SGFixedVector2Internal p2 = p_transform.get_origin();

    fixed r1 = get_rotation();
    fixed r2 = p_transform.get_rotation();

    SGFixedVector2Internal s1 = get_scale();
    SGFixedVector2Internal s2 = p_transform.get_scale();

    SGFixedVector2Internal v1(r1.cos(), r1.sin());
    SGFixedVector2Internal v2(r2.cos(), r2.sin());

    fixed dot = v1.dot(v2);
    dot = CLAMP(dot, fixed::NEG_ONE, fixed::ONE);

    SGFixedVector2Internal v;

    // 65500 = ~0.9995
    if (dot > fixed(65500))
    {
        v = SGFixedVector2Internal::linear_interpolate(v1, v2, p_c).normalized();
    }
    else
    {
        fixed angle = p_c * dot.acos();
        SGFixedVector2Internal v3 = (v2 - v1 * dot).normalized();
        v = v1 * angle.cos() + v3 * angle.sin();
    }

    SGFixedTransform2DInternal res(v.y.atan2(v.x),
                                   SGFixedVector2Internal::linear_interpolate(p1, p2, p_c));
    res.scale_basis(SGFixedVector2Internal::linear_interpolate(s1, s2, p_c));
    return res;
}
