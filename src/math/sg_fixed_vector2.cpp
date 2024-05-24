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

#include "sg_fixed_vector2.h"

void SGFixedVector2::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_x"), &SGFixedVector2::get_x);
    ClassDB::bind_method(D_METHOD("set_x", "x"), &SGFixedVector2::set_x);
    ClassDB::bind_method(D_METHOD("get_y"), &SGFixedVector2::get_y);
    ClassDB::bind_method(D_METHOD("set_y", "y"), &SGFixedVector2::set_y);

    ClassDB::bind_method(D_METHOD("clear"), &SGFixedVector2::clear);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "x", PROPERTY_HINT_NONE), "set_x", "get_x");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "y", PROPERTY_HINT_NONE), "set_y", "get_y");

    ClassDB::bind_method(D_METHOD("add", "value"), &SGFixedVector2::add);
    ClassDB::bind_method(D_METHOD("iadd", "value"), &SGFixedVector2::iadd);
    ClassDB::bind_method(D_METHOD("sub", "value"), &SGFixedVector2::sub);
    ClassDB::bind_method(D_METHOD("isub", "value"), &SGFixedVector2::isub);
    ClassDB::bind_method(D_METHOD("mul", "value"), &SGFixedVector2::mul);
    ClassDB::bind_method(D_METHOD("imul", "value"), &SGFixedVector2::imul);
    ClassDB::bind_method(D_METHOD("div", "value"), &SGFixedVector2::div);
    ClassDB::bind_method(D_METHOD("idiv", "value"), &SGFixedVector2::idiv);

    ClassDB::bind_method(D_METHOD("copy"), &SGFixedVector2::copy);
    ClassDB::bind_method(D_METHOD("abs"), &SGFixedVector2::abs);
    ClassDB::bind_method(D_METHOD("normalized"), &SGFixedVector2::normalized);
    ClassDB::bind_method(D_METHOD("is_normalized"), &SGFixedVector2::is_normalized);
    ClassDB::bind_method(D_METHOD("length"), &SGFixedVector2::length);
    ClassDB::bind_method(D_METHOD("length_squared"), &SGFixedVector2::length_squared);

    ClassDB::bind_method(D_METHOD("distance_to", "other_vector"), &SGFixedVector2::distance_to);
    ClassDB::bind_method(D_METHOD("distance_squared_to", "other_vector"),
                         &SGFixedVector2::distance_squared_to);
    ClassDB::bind_method(D_METHOD("angle_to", "other_vector"), &SGFixedVector2::angle_to);
    ClassDB::bind_method(D_METHOD("angle_to_point", "other_vector"), &SGFixedVector2::angle_to_point);
    ClassDB::bind_method(D_METHOD("direction_to", "other_vector"), &SGFixedVector2::direction_to);

    ClassDB::bind_method(D_METHOD("rotate", "radians"), &SGFixedVector2::rotate);
    ClassDB::bind_method(D_METHOD("rotated", "radians"), &SGFixedVector2::rotated);
    ClassDB::bind_method(D_METHOD("angle"), &SGFixedVector2::angle);

    ClassDB::bind_method(D_METHOD("dot", "other_vector"), &SGFixedVector2::dot);
    ClassDB::bind_method(D_METHOD("cross", "other_vector"), &SGFixedVector2::cross);

    ClassDB::bind_method(D_METHOD("linear_interpolate", "other_vector", "weight"),
                         &SGFixedVector2::linear_interpolate);
    ClassDB::bind_method(D_METHOD("cubic_interpolate", "other_vector", "pre_a", "post_b", "weight"),
                         &SGFixedVector2::cubic_interpolate);

    ClassDB::bind_method(D_METHOD("slide", "normal"), &SGFixedVector2::slide);
    ClassDB::bind_method(D_METHOD("bounce", "normal"), &SGFixedVector2::bounce);
    ClassDB::bind_method(D_METHOD("reflect", "normal"), &SGFixedVector2::reflect);

    ClassDB::bind_method(D_METHOD("is_equal_approx", "other_vector"),
                         &SGFixedVector2::is_equal_approx);

    ClassDB::bind_method(D_METHOD("from_float", "float_vector"), &SGFixedVector2::from_float);
    ClassDB::bind_method(D_METHOD("to_float"), &SGFixedVector2::to_float);
}

Ref<SGFixedVector2> SGFixedVector2::add(const Variant& p_other) const
{
    if (p_other.get_type() == Variant::INT)
        return SGFixedVector2::from_internal(value + fixed(p_other));

    Ref<SGFixedVector2> other_vector = Object::cast_to<SGFixedVector2>(p_other);
    ERR_FAIL_COND_V_MSG(!other_vector.is_valid(), Variant(), "Invalid type.");

    return SGFixedVector2::from_internal(value + other_vector->get_internal());
}

void SGFixedVector2::iadd(const Variant& p_other)
{
    if (p_other.get_type() == Variant::INT)
    {
        value += fixed(p_other);
    }
    else
    {
        Ref<SGFixedVector2> other_vector = Object::cast_to<SGFixedVector2>(p_other);
        ERR_FAIL_COND_MSG(!other_vector.is_valid(), "Invalid type.");

        value += other_vector->get_internal();
    }
    if (watcher)
        watcher->fixed_vector2_changed(this);
}

Ref<SGFixedVector2> SGFixedVector2::sub(const Variant& p_other) const
{
    if (p_other.get_type() == Variant::INT)
        return SGFixedVector2::from_internal(value - fixed(p_other));

    Ref<SGFixedVector2> other_vector = Object::cast_to<SGFixedVector2>(p_other);
    ERR_FAIL_COND_V_MSG(!other_vector.is_valid(), Variant(), "Invalid type.");

    return SGFixedVector2::from_internal(value - other_vector->get_internal());
}

void SGFixedVector2::isub(const Variant& p_other)
{
    if (p_other.get_type() == Variant::INT)
    {
        value -= fixed(p_other);
    }
    else
    {
        Ref<SGFixedVector2> other_vector = Object::cast_to<SGFixedVector2>(p_other);
        ERR_FAIL_COND_MSG(!other_vector.is_valid(), "Invalid type.");

        value -= other_vector->get_internal();
    }
    if (watcher)
        watcher->fixed_vector2_changed(this);
}

Ref<SGFixedVector2> SGFixedVector2::mul(const Variant& p_other) const
{
    if (p_other.get_type() == Variant::INT)
        return SGFixedVector2::from_internal(value * fixed(p_other));

    Ref<SGFixedVector2> other_vector = Object::cast_to<SGFixedVector2>(p_other);
    ERR_FAIL_COND_V_MSG(!other_vector.is_valid(), Variant(), "Invalid type.");

    return SGFixedVector2::from_internal(value * other_vector->get_internal());
}

void SGFixedVector2::imul(const Variant& p_other)
{
    if (p_other.get_type() == Variant::INT)
    {
        value *= fixed(p_other);
    }
    else
    {
        Ref<SGFixedVector2> other_vector = Object::cast_to<SGFixedVector2>(p_other);
        ERR_FAIL_COND_MSG(!other_vector.is_valid(), "Invalid type.");

        value *= other_vector->get_internal();
    }
    if (watcher)
        watcher->fixed_vector2_changed(this);
}

Ref<SGFixedVector2> SGFixedVector2::div(const Variant& p_other) const
{
    if (p_other.get_type() == Variant::INT)
        return SGFixedVector2::from_internal(value / fixed(p_other));

    Ref<SGFixedVector2> other_vector = Object::cast_to<SGFixedVector2>(p_other);
    ERR_FAIL_COND_V_MSG(!other_vector.is_valid(), Variant(), "Invalid type.");

    return SGFixedVector2::from_internal(value / other_vector->get_internal());
}

void SGFixedVector2::idiv(const Variant& p_other)
{
    if (p_other.get_type() == Variant::INT)
    {
        value /= fixed(p_other);
    }
    else
    {
        Ref<SGFixedVector2> other_vector = Object::cast_to<SGFixedVector2>(p_other);
        ERR_FAIL_COND_MSG(!other_vector.is_valid(), "Invalid type.");

        value /= other_vector->get_internal();
    }
    if (watcher)
        watcher->fixed_vector2_changed(this);
}

Ref<SGFixedVector2> SGFixedVector2::copy() const
{
    return SGFixedVector2::from_internal(value);
}

Ref<SGFixedVector2> SGFixedVector2::abs() const
{
    return SGFixedVector2::from_internal(value.abs());
}

Ref<SGFixedVector2> SGFixedVector2::normalized() const
{
    return SGFixedVector2::from_internal(value.normalized());
}

bool SGFixedVector2::is_normalized() const
{
    return value.is_normalized();
}

int64_t SGFixedVector2::length() const
{
    return value.length().value;
}

int64_t SGFixedVector2::length_squared() const
{
    return value.length_squared().value;
}

int64_t SGFixedVector2::distance_to(const Ref<SGFixedVector2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), 0);
    return value.distance_to(p_other->get_internal()).value;
}

int64_t SGFixedVector2::distance_squared_to(const Ref<SGFixedVector2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), 0);
    return value.distance_squared_to(p_other->get_internal()).value;
}

int64_t SGFixedVector2::angle_to(const Ref<SGFixedVector2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), 0);
    return value.angle_to(p_other->get_internal()).value;
}

int64_t SGFixedVector2::angle_to_point(const Ref<SGFixedVector2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), 0);
    return value.angle_to_point(p_other->get_internal()).value;
}

Ref<SGFixedVector2> SGFixedVector2::direction_to(const Ref<SGFixedVector2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), Ref<SGFixedVector2>());
    return SGFixedVector2::from_internal(value.direction_to(p_other->get_internal()));
}

void SGFixedVector2::rotate(int64_t p_rotation)
{
    value = value.rotated(fixed(p_rotation));

    if (watcher)
        watcher->fixed_vector2_changed(this);
}

int64_t SGFixedVector2::angle() const
{
    return value.angle().value;
}

int64_t SGFixedVector2::dot(const Ref<SGFixedVector2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), 0);
    return value.dot(p_other->get_internal()).value;
}

int64_t SGFixedVector2::cross(const Ref<SGFixedVector2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), 0);
    return value.cross(p_other->get_internal()).value;
}

Ref<SGFixedVector2> SGFixedVector2::rotated(int64_t p_rotation) const
{
    return SGFixedVector2::from_internal(value.rotated(fixed(p_rotation)));
}

Ref<SGFixedVector2> SGFixedVector2::linear_interpolate(const Ref<SGFixedVector2>& p_to,
                                                       int64_t weight) const
{
    ERR_FAIL_COND_V(!p_to.is_valid(), Ref<SGFixedVector2>());
    return SGFixedVector2::from_internal(
        SGFixedVector2Internal::linear_interpolate(value, p_to->get_internal(), fixed(weight)));
}

Ref<SGFixedVector2> SGFixedVector2::slide(const Ref<SGFixedVector2>& p_normal) const
{
    ERR_FAIL_COND_V(!p_normal.is_valid(), Ref<SGFixedVector2>());
    Ref<SGFixedVector2> v(memnew(SGFixedVector2));
    v->value = value.slide(p_normal->value);
    return v;
}

Ref<SGFixedVector2> SGFixedVector2::bounce(const Ref<SGFixedVector2>& p_normal) const
{
    ERR_FAIL_COND_V(!p_normal.is_valid(), Ref<SGFixedVector2>());
    Ref<SGFixedVector2> v(memnew(SGFixedVector2));
    v->value = value.bounce(p_normal->value);
    return v;
}

Ref<SGFixedVector2> SGFixedVector2::reflect(const Ref<SGFixedVector2>& p_normal) const
{
    ERR_FAIL_COND_V(!p_normal.is_valid(), Ref<SGFixedVector2>());
    Ref<SGFixedVector2> v(memnew(SGFixedVector2));
    v->value = value.reflect(p_normal->value);
    return v;
}

bool SGFixedVector2::is_equal_approx(const Ref<SGFixedVector2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), false);
    return value.is_equal_approx(p_other->get_internal());
}

void SGFixedVector2::from_float(Vector2 p_float_vector)
{
    value.x = fixed::from_float(p_float_vector.x);
    value.y = fixed::from_float(p_float_vector.y);

    if (watcher)
        watcher->fixed_vector2_changed(this);
}

Vector2 SGFixedVector2::to_float() const
{
    return Vector2(value.x.to_float(), value.y.to_float());
}

Ref<SGFixedVector2> SGFixedVector2::cubic_interpolate(
    const Ref<SGFixedVector2>& p_b, const Ref<SGFixedVector2>& p_pre_a,
    const Ref<SGFixedVector2>& p_post_b, int64_t p_weight) const
{
    return SGFixedVector2::from_internal(value.cubic_interpolate(
        p_b->get_internal(), p_pre_a->get_internal(), p_post_b->get_internal(), fixed(p_weight)));
}
