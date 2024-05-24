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

#ifndef SG_FIXED_SINGLETON_H
#define SG_FIXED_SINGLETON_H

#include <cstdint>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string.hpp>

#include "sg_fixed_rect2.h"
#include "sg_fixed_transform_2d.h"
#include "sg_fixed_vector2.h"

class SGFixed : public Object
{
    GDCLASS(SGFixed, Object);

    static SGFixed* singleton;

protected:
    static void _bind_methods();

public:
    enum : int64_t {
        ONE = 65536,
        HALF = 32768,
        TWO = 131072,
        NEG_ONE = -65536,
        E = 178145,
        PI = 205887,
        TAU = 411774,
        PI_DIV_2 = 102943,
        PI_DIV_4 = 51472,
        EPSILON = 1,
        ARITHMETIC_OVERFLOW = INT64_MIN,
    };

    static const int64_t TENS[];
    static const int TENS_SIZE;

    static SGFixed* get_singleton();

    int64_t from_int(int64_t p_int_value) const;
    int64_t from_float(float p_float_value) const;
    int64_t from_string(String p_string_value) const;

    int64_t to_int(int64_t p_fixed_value) const;
    float to_float(int64_t p_fixed_value) const;
    String format_string(int64_t p_fixed_value) const;

    int64_t mul(int64_t p_fixed_one, int64_t p_fixed_two) const;
    int64_t div(int64_t p_fixed_one, int64_t p_fixed_two) const;

    int64_t sin(int64_t p_fixed_value) const;
    int64_t cos(int64_t p_fixed_value) const;
    int64_t tan(int64_t p_fixed_value) const;
    int64_t asin(int64_t p_fixed_value) const;
    int64_t acos(int64_t p_fixed_value) const;
    int64_t atan(int64_t p_fixed_value) const;
    int64_t atan2(int64_t p_fixed_y_value, int64_t p_fixed_x_value) const;
    int64_t sqrt(int64_t p_fixed_value) const;
    int64_t deg_to_rad(int64_t p_fixed_value) const;
    int64_t rad_to_deg(int64_t p_fixed_value) const;
    int64_t floor(int64_t p_fixed_value) const;
    int64_t round(int64_t p_fixed_value) const;
    int64_t ceil(int64_t p_fixed_value) const;
    int64_t div_rounded(int64_t p_fixed_one, int64_t p_fixed_two) const;
    int64_t exp(int64_t p_fixed_value) const;
    int64_t log(int64_t p_fixed_value) const;
    int64_t pow(int64_t p_fixed_one, int64_t p_fixed_two) const;
    int64_t move_toward(int64_t p_from, int64_t p_to, int64_t p_delta) const;

    Ref<SGFixedVector2> vector2(int64_t p_fixed_x, int64_t p_fixed_y) const;
    Ref<SGFixedVector2> from_float_vector2(const Vector2& p_float_vector) const;

    Ref<SGFixedRect2> rect2(const Ref<SGFixedVector2>& p_position,
                            const Ref<SGFixedVector2>& p_size) const;
    Ref<SGFixedRect2> from_float_rect2(const Rect2& p_float_rect) const;

    Ref<SGFixedTransform2D> transform2d(int64_t p_rotation, const Ref<SGFixedVector2>& p_origin) const;

    SGFixed();
    ~SGFixed();
};

#endif
