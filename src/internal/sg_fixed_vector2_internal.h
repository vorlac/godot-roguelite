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

#ifndef SG_FIXED_VECTOR2_INTERNAL_H
#define SG_FIXED_VECTOR2_INTERNAL_H

#include "sg_fixed_number_internal.h"

struct SGFixedVector2Internal
{
    enum Axis {
        AXIS_X,
        AXIS_Y,
    };

    union
    {
        fixed x;
        fixed width;
    };

    union
    {
        fixed y;
        fixed height;
    };

    static const fixed FIXED_UNIT_EPSILON;
    static const SGFixedVector2Internal ZERO;

    _FORCE_INLINE_ fixed& operator[](int p_idx)
    {
        return p_idx ? y : x;
    }

    _FORCE_INLINE_ const fixed& operator[](int p_idx) const
    {
        return p_idx ? y : x;
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator+(const SGFixedVector2Internal& p_v) const
    {
        return SGFixedVector2Internal(x + p_v.x, y + p_v.y);
    }

    _FORCE_INLINE_ void operator+=(const SGFixedVector2Internal& p_v)
    {
        x += p_v.x;
        y += p_v.y;
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator-(const SGFixedVector2Internal& p_v) const
    {
        return SGFixedVector2Internal(x - p_v.x, y - p_v.y);
    }

    _FORCE_INLINE_ void operator-=(const SGFixedVector2Internal& p_v)
    {
        x -= p_v.x;
        y -= p_v.y;
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator*(const SGFixedVector2Internal& p_v) const
    {
        return SGFixedVector2Internal(x * p_v.x, y * p_v.y);
    }

    _FORCE_INLINE_ void operator*=(const SGFixedVector2Internal& p_v)
    {
        x *= p_v.x;
        y *= p_v.y;
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator/(const SGFixedVector2Internal& p_v) const
    {
        return SGFixedVector2Internal(x / p_v.x, y / p_v.y);
    }

    _FORCE_INLINE_ void operator/=(const SGFixedVector2Internal& p_v)
    {
        x /= p_v.x;
        y /= p_v.y;
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator+(const fixed& p_v) const
    {
        return SGFixedVector2Internal(x + p_v, y + p_v);
    }

    _FORCE_INLINE_ void operator+=(const fixed& p_v)
    {
        x += p_v;
        y += p_v;
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator-(const fixed& p_v) const
    {
        return SGFixedVector2Internal(x - p_v, y - p_v);
    }

    _FORCE_INLINE_ void operator-=(const fixed& p_v)
    {
        x -= p_v;
        y -= p_v;
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator*(const fixed& p_v) const
    {
        return SGFixedVector2Internal(x * p_v, y * p_v);
    }

    _FORCE_INLINE_ void operator*=(const fixed& p_v)
    {
        x *= p_v;
        y *= p_v;
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator/(const fixed& p_v) const
    {
        return SGFixedVector2Internal(x / p_v, y / p_v);
    }

    _FORCE_INLINE_ void operator/=(const fixed& p_v)
    {
        x /= p_v;
        y /= p_v;
    }

    bool operator==(const SGFixedVector2Internal& p_v) const;
    bool operator!=(const SGFixedVector2Internal& p_v) const;

    fixed angle() const;

    void set_rotation(fixed p_radians)
    {
        x = p_radians.cos();
        y = p_radians.sin();
    }

    _FORCE_INLINE_ SGFixedVector2Internal abs() const
    {
        return SGFixedVector2Internal(x.abs(), y.abs());
    }

    _FORCE_INLINE_ SGFixedVector2Internal operator-() const
    {
        return SGFixedVector2Internal(-x, -y);
    }

    SGFixedVector2Internal rotated(fixed p_rotation) const;

    void normalize();
    SGFixedVector2Internal normalized() const;
    bool is_normalized() const;

    SGFixedVector2Internal safe_scale(const SGFixedVector2Internal& p_other) const;
    SGFixedVector2Internal safe_scale(fixed p_scale) const;

    fixed length() const;
    fixed length_squared() const;

    fixed distance_to(const SGFixedVector2Internal& p_other) const;
    fixed distance_squared_to(const SGFixedVector2Internal& p_other) const;
    fixed angle_to(const SGFixedVector2Internal& p_other) const;
    fixed angle_to_point(const SGFixedVector2Internal& p_other) const;
    _FORCE_INLINE_ SGFixedVector2Internal direction_to(const SGFixedVector2Internal& p_to) const;

    fixed dot(const SGFixedVector2Internal& p_other) const;
    fixed cross(const SGFixedVector2Internal& p_other) const;

    // fixed posmod(const fixed p_mod) const;
    // fixed posmodv(const SGFixedVector2Internal &p_modv) const;
    // SGFixedVector2Internal project(const SGFixedVector2Internal &p_to) const;
    _FORCE_INLINE_ SGFixedVector2Internal tangent() const
    {
        return SGFixedVector2Internal(y, -x);
    }

    _FORCE_INLINE_ static SGFixedVector2Internal linear_interpolate(
        const SGFixedVector2Internal& p_a, const SGFixedVector2Internal& p_b, fixed p_weight);

    SGFixedVector2Internal slide(const SGFixedVector2Internal& p_normal) const;
    SGFixedVector2Internal bounce(const SGFixedVector2Internal& p_normal) const;
    SGFixedVector2Internal reflect(const SGFixedVector2Internal& p_normal) const;

    bool is_equal_approx(const SGFixedVector2Internal& p_v) const;

    _FORCE_INLINE_ SGFixedVector2Internal(fixed p_x, fixed p_y)
        : x(p_x)
        , y(p_y)
    {
    }

    _FORCE_INLINE_ SGFixedVector2Internal()
        : x(fixed::ZERO)
        , y(fixed::ZERO)
    {
    }

    SGFixedVector2Internal cubic_interpolate(
        const SGFixedVector2Internal& p_b, const SGFixedVector2Internal& p_pre_a,
        const SGFixedVector2Internal& p_post_b, fixed p_weight) const;

    static SGFixedVector2Internal get_closest_point_to_segment_2d(
        const SGFixedVector2Internal& p_point, const SGFixedVector2Internal* p_segment);
};

SGFixedVector2Internal SGFixedVector2Internal::direction_to(const SGFixedVector2Internal& p_to) const
{
    SGFixedVector2Internal ret(p_to.x - x, p_to.y - y);
    ret.normalize();
    return ret;
}

SGFixedVector2Internal SGFixedVector2Internal::linear_interpolate(
    const SGFixedVector2Internal& p_a, const SGFixedVector2Internal& p_b, fixed p_weight)
{
    SGFixedVector2Internal res = p_a;
    res.x += (p_weight * (p_b.x - p_a.x));
    res.y += (p_weight * (p_b.y - p_a.y));
    return res;
}

#endif
