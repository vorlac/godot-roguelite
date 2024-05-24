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

#include "sg_fixed_transform_2d.h"

void SGFixedTransform2D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_x"), &SGFixedTransform2D::get_x);
    ClassDB::bind_method(D_METHOD("set_x", "x"), &SGFixedTransform2D::set_x);
    ClassDB::bind_method(D_METHOD("get_y"), &SGFixedTransform2D::get_y);
    ClassDB::bind_method(D_METHOD("set_y", "y"), &SGFixedTransform2D::set_y);
    ClassDB::bind_method(D_METHOD("get_origin"), &SGFixedTransform2D::get_origin);
    ClassDB::bind_method(D_METHOD("set_origin", "origin"), &SGFixedTransform2D::set_origin);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "x", PROPERTY_HINT_NONE, "", 0), "set_x", "get_x");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "y", PROPERTY_HINT_NONE, "", 0), "set_y", "get_y");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "origin", PROPERTY_HINT_NONE, "", 0), "set_origin",
                 "get_origin");

    ClassDB::bind_method(D_METHOD("inverse"), &SGFixedTransform2D::inverse);
    ClassDB::bind_method(D_METHOD("affine_inverse"), &SGFixedTransform2D::affine_inverse);
    ClassDB::bind_method(D_METHOD("get_rotation"), &SGFixedTransform2D::get_rotation);
    ClassDB::bind_method(D_METHOD("rotated", "radians"), &SGFixedTransform2D::rotated);
    ClassDB::bind_method(D_METHOD("get_scale"), &SGFixedTransform2D::get_scale);
    ClassDB::bind_method(D_METHOD("scaled", "scale"), &SGFixedTransform2D::scaled);
    ClassDB::bind_method(D_METHOD("translated", "offset"), &SGFixedTransform2D::translated);
    ClassDB::bind_method(D_METHOD("orthonormalized"), &SGFixedTransform2D::orthonormalized);
    ClassDB::bind_method(D_METHOD("is_equal_approx", "transform"),
                         &SGFixedTransform2D::is_equal_approx);
    ClassDB::bind_method(D_METHOD("mul", "transform"), &SGFixedTransform2D::mul);
    ClassDB::bind_method(D_METHOD("interpolate_with", "transform"),
                         &SGFixedTransform2D::interpolate_with);
    ClassDB::bind_method(D_METHOD("basis_xform", "vector"), &SGFixedTransform2D::basis_xform);
    ClassDB::bind_method(D_METHOD("basis_xform_inv", "vector"), &SGFixedTransform2D::basis_xform_inv);
    ClassDB::bind_method(D_METHOD("xform", "vector"), &SGFixedTransform2D::xform);
    ClassDB::bind_method(D_METHOD("xform_inv", "vector"), &SGFixedTransform2D::xform_inv);

    ClassDB::bind_method(D_METHOD("copy"), &SGFixedTransform2D::copy);
}

Transform2D SGFixedTransform2D::to_float() const
{
    Vector2 x_float = x->to_float();
    Vector2 y_float = y->to_float();
    Vector2 origin_float = origin->to_float();
    return Transform2D(x_float.x, x_float.y, y_float.x, y_float.y, origin_float.x, origin_float.y);
}

void SGFixedTransform2D::from_float(const Transform2D& p_float_transform)
{
    x->from_float(p_float_transform[0]);
    y->from_float(p_float_transform[1]);
    origin->from_float(p_float_transform[2]);
}

Ref<SGFixedTransform2D> SGFixedTransform2D::copy() const
{
    Ref<SGFixedTransform2D> ret(memnew(SGFixedTransform2D));
    ret->x->set_internal(x->get_internal());
    ret->y->set_internal(y->get_internal());
    ret->origin->set_internal(origin->get_internal());
    return ret;
}

void SGFixedTransform2D::set_x(const Ref<SGFixedVector2>& p_x)
{
    ERR_FAIL_COND(!p_x.is_valid());
    x->set_internal(p_x->get_internal());
}

void SGFixedTransform2D::set_y(const Ref<SGFixedVector2>& p_y)
{
    ERR_FAIL_COND(!p_y.is_valid());
    y->set_internal(p_y->get_internal());
}

void SGFixedTransform2D::set_origin(const Ref<SGFixedVector2>& p_origin)
{
    ERR_FAIL_COND(!p_origin.is_valid());
    origin->set_internal(p_origin->get_internal());
}

Ref<SGFixedTransform2D> SGFixedTransform2D::inverse() const
{
    SGFixedTransform2DInternal internal = get_internal();
    internal.invert();
    return SGFixedTransform2D::from_internal(internal);
}

Ref<SGFixedTransform2D> SGFixedTransform2D::affine_inverse() const
{
    SGFixedTransform2DInternal internal = get_internal();
    internal.affine_invert();
    return SGFixedTransform2D::from_internal(internal);
}

int64_t SGFixedTransform2D::get_rotation() const
{
    SGFixedTransform2DInternal internal = get_internal();
    return internal.get_rotation().value;
}

Ref<SGFixedTransform2D> SGFixedTransform2D::rotated(int64_t p_radians) const
{
    SGFixedTransform2DInternal internal = get_internal();
    internal.rotate(fixed(p_radians));
    return SGFixedTransform2D::from_internal(internal);
}

Ref<SGFixedVector2> SGFixedTransform2D::get_scale() const
{
    SGFixedTransform2DInternal internal = get_internal();
    return SGFixedVector2::from_internal(internal.get_scale());
}

Ref<SGFixedTransform2D> SGFixedTransform2D::scaled(const Ref<SGFixedVector2>& p_scale) const
{
    ERR_FAIL_COND_V(!p_scale.is_valid(), Ref<SGFixedTransform2D>());
    SGFixedTransform2DInternal internal = get_internal();
    internal.scale(p_scale->get_internal());
    return SGFixedTransform2D::from_internal(internal);
}

Ref<SGFixedTransform2D> SGFixedTransform2D::translated(const Ref<SGFixedVector2>& p_offset) const
{
    ERR_FAIL_COND_V(!p_offset.is_valid(), Ref<SGFixedTransform2D>());
    SGFixedTransform2DInternal internal = get_internal();
    internal.translate(p_offset->get_internal());
    return SGFixedTransform2D::from_internal(internal);
}

Ref<SGFixedTransform2D> SGFixedTransform2D::orthonormalized() const
{
    SGFixedTransform2DInternal internal = get_internal();
    internal.orthonormalize();
    return SGFixedTransform2D::from_internal(internal);
}

bool SGFixedTransform2D::is_equal_approx(const Ref<SGFixedTransform2D>& p_transform) const
{
    ERR_FAIL_COND_V(!p_transform.is_valid(), false);
    SGFixedTransform2DInternal internal = get_internal();
    return internal.is_equal_approx(p_transform->get_internal());
}

Ref<SGFixedTransform2D> SGFixedTransform2D::mul(const Ref<SGFixedTransform2D>& p_transform) const
{
    ERR_FAIL_COND_V(!p_transform.is_valid(), Ref<SGFixedTransform2D>());
    return SGFixedTransform2D::from_internal(get_internal() * p_transform->get_internal());
}

Ref<SGFixedTransform2D> SGFixedTransform2D::interpolate_with(
    const Ref<SGFixedTransform2D>& p_transform, int64_t p_weight) const
{
    ERR_FAIL_COND_V(!p_transform.is_valid(), Ref<SGFixedTransform2D>());
    SGFixedTransform2DInternal internal = get_internal();
    internal.interpolate_with(p_transform->get_internal(), fixed(p_weight));
    return SGFixedTransform2D::from_internal(internal);
}

Ref<SGFixedVector2> SGFixedTransform2D::basis_xform(const Ref<SGFixedVector2>& p_vec) const
{
    ERR_FAIL_COND_V(!p_vec.is_valid(), Ref<SGFixedTransform2D>());
    SGFixedVector2Internal internal = p_vec->get_internal();
    internal = get_internal().basis_xform(internal);
    return SGFixedVector2::from_internal(internal);
}

Ref<SGFixedVector2> SGFixedTransform2D::basis_xform_inv(const Ref<SGFixedVector2>& p_vec) const
{
    ERR_FAIL_COND_V(!p_vec.is_valid(), Ref<SGFixedTransform2D>());
    SGFixedVector2Internal internal = p_vec->get_internal();
    internal = get_internal().basis_xform_inv(internal);
    return SGFixedVector2::from_internal(internal);
}

Ref<SGFixedVector2> SGFixedTransform2D::xform(const Ref<SGFixedVector2>& p_vec) const
{
    ERR_FAIL_COND_V(!p_vec.is_valid(), Ref<SGFixedTransform2D>());
    SGFixedVector2Internal internal = p_vec->get_internal();
    internal = get_internal().xform(internal);
    return SGFixedVector2::from_internal(internal);
}

Ref<SGFixedVector2> SGFixedTransform2D::xform_inv(const Ref<SGFixedVector2>& p_vec) const
{
    ERR_FAIL_COND_V(!p_vec.is_valid(), Ref<SGFixedTransform2D>());
    SGFixedVector2Internal internal = p_vec->get_internal();
    internal = get_internal().xform_inv(internal);
    return SGFixedVector2::from_internal(internal);
}

SGFixedTransform2D::SGFixedTransform2D()
    : x(memnew(SGFixedVector2(SGFixedVector2Internal(fixed::ONE, fixed::ZERO))))
    , y(memnew(SGFixedVector2(SGFixedVector2Internal(fixed::ZERO, fixed::ONE))))
    , origin(memnew(SGFixedVector2()))
{
}

SGFixedTransform2D::SGFixedTransform2D(const SGFixedTransform2DInternal& p_internal)
    : x(memnew(SGFixedVector2(p_internal.elements[0])))
    , y(memnew(SGFixedVector2(p_internal.elements[1])))
    , origin(memnew(SGFixedVector2(p_internal.elements[2])))
{
}
