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

#ifndef SG_FIXED_VECTOR2_H
#define SG_FIXED_VECTOR2_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>

#include "internal/sg_fixed_vector2_internal.h"

using namespace godot;

class SGFixedVector2;

class SGFixedVector2Watcher
{
public:
    virtual void fixed_vector2_changed(SGFixedVector2* p_vector) = 0;
};

class SGFixedVector2 : public RefCounted
{
    GDCLASS(SGFixedVector2, RefCounted);

    mutable SGFixedVector2Watcher* watcher;
    SGFixedVector2Internal value;

protected:
    static void _bind_methods();

public:
    _FORCE_INLINE_ int64_t get_x() const
    {
        return value.x.value;
    }

    _FORCE_INLINE_ int64_t get_y() const
    {
        return value.y.value;
    }

    void set_x(int64_t p_x)
    {
        value.x.value = p_x;
        if (watcher)
            watcher->fixed_vector2_changed(this);
    }

    void set_y(int64_t p_y)
    {
        value.y.value = p_y;
        if (watcher)
            watcher->fixed_vector2_changed(this);
    }

    void clear()
    {
        value.x.value = 0;
        value.y.value = 0;
        if (watcher)
            watcher->fixed_vector2_changed(this);
    }

    _FORCE_INLINE_ void set_watcher(SGFixedVector2Watcher* p_watcher) const
    {
        watcher = p_watcher;
    }

    Ref<SGFixedVector2> add(const Variant& p_other) const;
    void iadd(const Variant& p_other);
    Ref<SGFixedVector2> sub(const Variant& p_other) const;
    void isub(const Variant& p_other);
    Ref<SGFixedVector2> mul(const Variant& p_other) const;
    void imul(const Variant& p_other);
    Ref<SGFixedVector2> div(const Variant& p_other) const;
    void idiv(const Variant& p_other);

    Ref<SGFixedVector2> copy() const;

    Ref<SGFixedVector2> abs() const;
    Ref<SGFixedVector2> normalized() const;
    bool is_normalized() const;

    int64_t length() const;
    int64_t length_squared() const;

    int64_t distance_to(const Ref<SGFixedVector2>& p_other) const;
    int64_t distance_squared_to(const Ref<SGFixedVector2>& p_other) const;
    int64_t angle_to(const Ref<SGFixedVector2>& p_other) const;
    int64_t angle_to_point(const Ref<SGFixedVector2>& p_other) const;
    Ref<SGFixedVector2> direction_to(const Ref<SGFixedVector2>& p_other) const;

    void rotate(int64_t p_rotation);
    Ref<SGFixedVector2> rotated(int64_t p_rotation) const;
    int64_t angle() const;

    int64_t dot(const Ref<SGFixedVector2>& p_other) const;
    int64_t cross(const Ref<SGFixedVector2>& p_other) const;

    Ref<SGFixedVector2> linear_interpolate(const Ref<SGFixedVector2>& p_to, int64_t weight) const;
    Ref<SGFixedVector2> cubic_interpolate(
        const Ref<SGFixedVector2>& p_b, const Ref<SGFixedVector2>& p_pre_a,
        const Ref<SGFixedVector2>& p_post_b, int64_t p_weight) const;

    Ref<SGFixedVector2> slide(const Ref<SGFixedVector2>& p_normal) const;
    Ref<SGFixedVector2> bounce(const Ref<SGFixedVector2>& p_normal) const;
    Ref<SGFixedVector2> reflect(const Ref<SGFixedVector2>& p_normal) const;

    bool is_equal_approx(const Ref<SGFixedVector2>& p_other) const;

    void from_float(Vector2 p_float_vector);
    Vector2 to_float() const;

    // Won't trigger the "changed" signal. Meant only for internal use.
    _FORCE_INLINE_ SGFixedVector2Internal get_internal() const
    {
        return value;
    }

    _FORCE_INLINE_ void set_internal(SGFixedVector2Internal p_value)
    {
        value = p_value;
    }

    _FORCE_INLINE_ static Ref<SGFixedVector2> from_internal(const SGFixedVector2Internal& p_internal)
    {
        return Ref<SGFixedVector2>(memnew(SGFixedVector2(p_internal)));
    }

    SGFixedVector2()
    {
        watcher = nullptr;
    }

    SGFixedVector2(const SGFixedVector2Internal& p_internal_vector)
    {
        value = p_internal_vector;
        watcher = nullptr;
    }

    ~SGFixedVector2() {};
};

#endif
