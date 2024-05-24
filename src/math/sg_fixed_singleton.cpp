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

#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/string.hpp>

#include "internal/sg_fixed_number_internal.h"
#include "sg_fixed_singleton.h"

const int64_t SGFixed::TENS[]{ 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };
const int SGFixed::TENS_SIZE = sizeof(TENS) / sizeof(TENS[0]);

SGFixed* SGFixed::singleton = NULL;

SGFixed::SGFixed()
{
    ERR_FAIL_COND(singleton != NULL);
    singleton = this;
}

SGFixed::~SGFixed()
{
    singleton = NULL;
}

SGFixed* SGFixed::get_singleton()
{
    return singleton;
}

void SGFixed::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("from_int", "int_value"), &SGFixed::from_int);
    ClassDB::bind_method(D_METHOD("from_float", "float_value"), &SGFixed::from_float);
    ClassDB::bind_method(D_METHOD("from_string", "string_value"), &SGFixed::from_string);
    ClassDB::bind_method(D_METHOD("to_int", "fixed_value"), &SGFixed::to_int);
    ClassDB::bind_method(D_METHOD("to_float", "fixed_value"), &SGFixed::to_float);
    ClassDB::bind_method(D_METHOD("format_string", "fixed_value"), &SGFixed::format_string);
    ClassDB::bind_method(D_METHOD("mul", "fixed_one", "fixed_two"), &SGFixed::mul);
    ClassDB::bind_method(D_METHOD("div", "fixed_one", "fixed_two"), &SGFixed::div);

    ClassDB::bind_method(D_METHOD("sin", "fixed_value"), &SGFixed::sin);
    ClassDB::bind_method(D_METHOD("cos", "fixed_value"), &SGFixed::cos);
    ClassDB::bind_method(D_METHOD("tan", "fixed_value"), &SGFixed::tan);
    ClassDB::bind_method(D_METHOD("asin", "fixed_value"), &SGFixed::asin);
    ClassDB::bind_method(D_METHOD("acos", "fixed_value"), &SGFixed::acos);
    ClassDB::bind_method(D_METHOD("atan", "fixed_value"), &SGFixed::atan);
    ClassDB::bind_method(D_METHOD("atan2", "fixed_y_value", "fixed_x_value"), &SGFixed::atan2);
    ClassDB::bind_method(D_METHOD("sqrt", "fixed_value"), &SGFixed::sqrt);
    ClassDB::bind_method(D_METHOD("deg_to_rad", "fixed_value"), &SGFixed::deg_to_rad);
    ClassDB::bind_method(D_METHOD("rad_to_deg", "fixed_value"), &SGFixed::rad_to_deg);
    ClassDB::bind_method(D_METHOD("floor", "fixed_value"), &SGFixed::floor);
    ClassDB::bind_method(D_METHOD("round", "fixed_value"), &SGFixed::round);
    ClassDB::bind_method(D_METHOD("ceil", "fixed_value"), &SGFixed::ceil);
    ClassDB::bind_method(D_METHOD("div_rounded", "fixed_one", "fixed_two"), &SGFixed::div_rounded);
    ClassDB::bind_method(D_METHOD("exp", "fixed_value"), &SGFixed::exp);
    ClassDB::bind_method(D_METHOD("log", "fixed_value"), &SGFixed::log);
    ClassDB::bind_method(D_METHOD("pow", "fixed_one", "fixed_two"), &SGFixed::pow);
    ClassDB::bind_method(
        D_METHOD("move_toward", "fixed_from_value", "fixed_to_value", "fixed_delta_value"),
        &SGFixed::move_toward);

    ClassDB::bind_method(D_METHOD("vector2", "fixed_x", "fixed_y"), &SGFixed::vector2);
    ClassDB::bind_method(D_METHOD("from_float_vector2", "float_vector"),
                         &SGFixed::from_float_vector2);

    ClassDB::bind_method(D_METHOD("rect2", "position", "size"), &SGFixed::rect2);
    ClassDB::bind_method(D_METHOD("from_float_rect2", "float_rect"), &SGFixed::from_float_rect2);

    ClassDB::bind_method(D_METHOD("transform2d", "rotation", "origin"), &SGFixed::transform2d);

    BIND_CONSTANT(ONE);
    BIND_CONSTANT(HALF);
    BIND_CONSTANT(TWO);
    BIND_CONSTANT(NEG_ONE);
    BIND_CONSTANT(E);
    BIND_CONSTANT(PI);
    BIND_CONSTANT(TAU);
    BIND_CONSTANT(PI_DIV_2);
    BIND_CONSTANT(PI_DIV_4);
    BIND_CONSTANT(EPSILON);
}

int64_t SGFixed::from_int(int64_t p_int_value) const
{
    return fixed::from_int(p_int_value).value;
}

int64_t SGFixed::from_float(float p_float_value) const
{
    return fixed::from_float(p_float_value).value;
}

int64_t SGFixed::from_string(String p_string_value) const
{
    if (!(p_string_value.is_valid_float() || p_string_value.is_valid_int()))
        return 0;

    int64_t value = 0;
    PackedStringArray str_values = p_string_value.split(".", false);
    if (!str_values.is_empty())
    {
        value = str_values[0].to_int() << 16;
        if (str_values.size() > 1)
        {
            if (str_values[1].length() >= TENS_SIZE)
                str_values.set(1, str_values[1].substr(0, TENS_SIZE - 1));

            value += (str_values[1].to_int() << 16) / TENS[str_values[1].length()];
        }
    }
    return value;
}

int64_t SGFixed::to_int(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).to_int();
}

float SGFixed::to_float(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).to_float();
}

String SGFixed::format_string(int64_t p_fixed_value) const
{
    String str = String::num_int64(p_fixed_value >> 16);
    int64_t decimal = p_fixed_value & 0xFFFF;
    if (decimal != 0)
    {
        decimal = (decimal * 10000000) >> 16;  // 10.000.000 - for precision

        // add left zeros
        String left_zeros = "";
        int64_t left_zero_checker = 1000000;
        while (left_zero_checker > decimal)
        {
            left_zero_checker /= 10;
            left_zeros = left_zeros + "0";
        }

        // right 0s, as decimals, are useless info: remove them
        while (decimal % 10 == 0)
            decimal /= 10;

        str = str + String(".") + left_zeros + String::num_int64(decimal);
    }
    return str;
}

int64_t SGFixed::mul(int64_t p_fixed_one, int64_t p_fixed_two) const
{
    return (fixed(p_fixed_one) * fixed(p_fixed_two)).value;
}

int64_t SGFixed::div(int64_t p_fixed_one, int64_t p_fixed_two) const
{
    return (fixed(p_fixed_one) / fixed(p_fixed_two)).value;
}

int64_t SGFixed::sin(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).sin().value;
}

int64_t SGFixed::cos(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).cos().value;
}

int64_t SGFixed::tan(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).tan().value;
}

int64_t SGFixed::asin(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).asin().value;
}

int64_t SGFixed::acos(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).acos().value;
}

int64_t SGFixed::atan(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).atan().value;
}

int64_t SGFixed::atan2(int64_t p_fixed_y_value, int64_t p_fixed_x_value) const
{
    return fixed(p_fixed_y_value).atan2(fixed(p_fixed_x_value)).value;
}

int64_t SGFixed::sqrt(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).sqrt().value;
}

int64_t SGFixed::deg_to_rad(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).deg2rad().value;
}

int64_t SGFixed::rad_to_deg(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).rad2deg().value;
}

int64_t SGFixed::div_rounded(int64_t p_fixed_one, int64_t p_fixed_two) const
{
    return fixed(p_fixed_one).div_rounded(fixed(p_fixed_two)).value;
}

int64_t SGFixed::exp(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).exp().value;
}

int64_t SGFixed::log(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).log().value;
}

int64_t SGFixed::pow(int64_t base, int64_t exp) const
{
    return fixed(base).pow(fixed(exp)).value;
}

int64_t SGFixed::floor(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).floor().value;
}

int64_t SGFixed::round(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).round().value;
}

int64_t SGFixed::ceil(int64_t p_fixed_value) const
{
    return fixed(p_fixed_value).ceil().value;
}

int64_t SGFixed::move_toward(int64_t p_from, int64_t p_to, int64_t p_delta) const
{
    return fixed(p_from).move_toward(fixed(p_to), fixed(p_delta)).value;
}

Ref<SGFixedVector2> SGFixed::vector2(int64_t p_fixed_x, int64_t p_fixed_y) const
{
    return Ref<SGFixedVector2>(
        memnew(SGFixedVector2(SGFixedVector2Internal(fixed(p_fixed_x), fixed(p_fixed_y)))));
}

Ref<SGFixedVector2> SGFixed::from_float_vector2(const Vector2& p_float_vector) const
{
    return Ref<SGFixedVector2>(memnew(SGFixedVector2(SGFixedVector2Internal(
        fixed::from_float(p_float_vector.x), fixed::from_float(p_float_vector.y)))));
}

Ref<SGFixedRect2> SGFixed::rect2(const Ref<SGFixedVector2>& p_position,
                                 const Ref<SGFixedVector2>& p_size) const
{
    Ref<SGFixedRect2> ret(memnew(SGFixedRect2));
    ret->set_position(p_position);
    ret->set_size(p_size);
    return ret;
}

Ref<SGFixedRect2> SGFixed::from_float_rect2(const Rect2& p_float_rect) const
{
    Ref<SGFixedRect2> ret(memnew(SGFixedRect2));
    ret->set_internal(
        SGFixedRect2Internal(SGFixedVector2Internal(fixed::from_float(p_float_rect.position.x),
                                                    fixed::from_float(p_float_rect.position.y)),
                             SGFixedVector2Internal(fixed::from_float(p_float_rect.size.x),
                                                    fixed::from_float(p_float_rect.size.y))));
    return ret;
}

Ref<SGFixedTransform2D> SGFixed::transform2d(int64_t p_rotation,
                                             const Ref<SGFixedVector2>& p_origin) const
{
    Ref<SGFixedTransform2D> ret(memnew(SGFixedTransform2D));
    ret->set_internal(SGFixedTransform2DInternal(fixed(p_rotation), p_origin->get_internal()));
    return ret;
}
