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

#include "sg_fixed_number_internal.h"

/**
 * Copied from
 * https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
 * but modified to use 64-bit numbers.
 *
 * Like fix16_sqrt(), for negative numbers we return the negated square root of
 * the absolute value (ie. sqrt(-x) = -sqrt(x)).
 */
int64_t sg_sqrt_64(int64_t num)
{
    if (num == 0)
        return 0;

    bool neg = num < 0;
    if (neg)
        num = -num;
    int64_t res = 0;
    int64_t bit = 1LL << 62;

    // Start bit at the highest power of four that's less than or equal to num.
    while (bit > num)
        bit >>= 2;

    while (bit != 0)
    {
        if (num >= res + bit)
        {
            num -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }

    return neg ? -res : res;
}

const fixed fixed::ZERO = fixed(0);
const fixed fixed::ONE = fixed(65536);
const fixed fixed::HALF = fixed(32768);
const fixed fixed::TWO = fixed(131072);
const fixed fixed::NEG_ONE = fixed(-65536);
const fixed fixed::PI = fixed(205887);
const fixed fixed::E = fixed(178145);
const fixed fixed::TAU = fixed(411774);
const fixed fixed::PI_DIV_2 = fixed(102943);
const fixed fixed::PI_DIV_4 = fixed(51472);
const fixed fixed::EPSILON = fixed(1);
const fixed fixed::ARITHMETIC_OVERFLOW = fixed(INT64_MIN);
const fixed fixed::DEG_180 = fixed(11796480);

// Adapted from the fpm library: https://github.com/MikeLankamp/fpm
// Copyright 2019 Mike Lankamp
// License: MIT
fixed fixed::sin() const
{
    // This sine uses a fifth-order curve-fitting approximation originally
    // described by Jasper Vijn on coranac.com which has a worst-case
    // relative error of 0.07% (over [-pi:pi]).

    // Turn x from [0..2*PI] domain into [0..4] domain
    fixed x = *this % fixed::TAU;
    x = x / fixed::PI_DIV_2;

    // Take x modulo one rotation, so [-4..+4].
    if (x < fixed::from_int(0))
        x += fixed::from_int(4);

    fixed sign = fixed::from_int(+1);
    if (x > fixed::from_int(2))
    {
        // Reduce domain to [0..2].
        sign = fixed::from_int(-1);
        x -= fixed::from_int(2);
    }

    if (x > fixed::from_int(1))
    {
        // Reduce domain to [0..1].
        x = fixed::from_int(2) - x;
    }

    const fixed x2 = x * x;
    return sign * x *
               (fixed::PI -
                x2 * (fixed::TAU - fixed::from_int(5) - x2 * (fixed::PI - fixed::from_int(3)))) >>
           1;
}

fixed fixed::cos() const
{
    return (*this + fixed::PI_DIV_2).sin();
}

fixed fixed::tan() const
{
    fixed cx = cos();
    ERR_FAIL_COND_V_MSG(cx == fixed::ZERO, fixed::ZERO, "tan() of 90 degree angles is undefined");
    return sin() / cx;
}

// Adapted from the fpm library: https://github.com/MikeLankamp/fpm
// Copyright 2019 Mike Lankamp
// License: MIT
fixed fixed::asin() const
{
    if (*this < fixed::NEG_ONE || *this > fixed::ONE)
        return fixed::ZERO;

    fixed yy = fixed::ONE - (*this) * (*this);
    if (yy == fixed::ZERO)
        return (*this > fixed::ZERO) ? fixed::PI_DIV_2 : -fixed::PI_DIV_2;

    return atan_div(*this, yy.sqrt());
}

// Adapted from the fpm library: https://github.com/MikeLankamp/fpm
// Copyright 2019 Mike Lankamp
// License: MIT
fixed fixed::acos() const
{
    if (*this < fixed::NEG_ONE || *this > fixed::ONE)
        return fixed::ZERO;

    if (*this == fixed::NEG_ONE)
        return fixed::PI;

    fixed yy = fixed::ONE - (*this) * (*this);
    return fixed::TWO * atan_div(yy.sqrt(), fixed::ONE + *this);
}

// Adapted from the fpm library: https://github.com/MikeLankamp/fpm
// Copyright 2019 Mike Lankamp
// License: MIT
fixed fixed::atan() const
{
    if (*this < fixed::ZERO)
        return -fixed(-value).atan();

    if (*this > fixed::ONE)
        return fixed::PI_DIV_2 - atan_sanitized(fixed::ONE / *this);

    return atan_sanitized(*this);
}

// Adapted from the fpm library: https://github.com/MikeLankamp/fpm
// Copyright 2019 Mike Lankamp
// License: MIT
fixed fixed::atan2(const fixed& inX) const
{
    if (*this == fixed::ZERO)
        return (inX < fixed::ZERO) ? fixed::PI : fixed::ZERO;
    if (inX == fixed::ZERO)
        return (*this > fixed::ZERO) ? fixed::PI_DIV_2 : -fixed::PI_DIV_2;

    fixed ret = atan_div(*this, inX);

    if (inX < fixed::ZERO)
        return (*this >= fixed::ZERO) ? ret + fixed::PI : ret - fixed::PI;
    return ret;
}

// Adapted from the fpm library: https://github.com/MikeLankamp/fpm
// Copyright 2019 Mike Lankamp
// License: MIT
fixed fixed::atan_sanitized(const fixed& p_x)
{
    ERR_FAIL_COND_V(p_x < fixed::ZERO || p_x > fixed::ONE, fixed::ZERO);

    fixed a = fixed(5089);    //  0.0776509570923569
    fixed b = fixed(-18837);  // -0.2874298095703125
    fixed c = fixed(65220);   //  0.999755859375 (PI_DIV_4 - A - B)

    fixed xx = p_x * p_x;
    return ((a * xx + b) * xx + c) * p_x;
}

// Adapted from the fpm library: https://github.com/MikeLankamp/fpm
// Copyright 2019 Mike Lankamp
// License: MIT
fixed fixed::atan_div(const fixed& p_y, const fixed& p_x)
{
    ERR_FAIL_COND_V(p_x == fixed::ZERO, fixed::ZERO);

    if (p_y < fixed::ZERO)
    {
        if (p_x < fixed::ZERO)
            return atan_div(-p_y, -p_x);
        return -atan_div(-p_y, p_x);
    }
    if (p_x < fixed::ZERO)
        return -atan_div(p_y, -p_x);

    if (p_y > p_x)
        return fixed::PI_DIV_2 - atan_sanitized(p_x / p_y);
    return atan_sanitized(p_y / p_x);
}

// Adapted from libfixmath: https://github.com/PetteriAimonen/libfixmath
// Copyright 2011-2021 Flatmush <Flatmush@gmail.com>
// License: MIT
fixed fixed::log() const
{
    fixed guess = fixed::from_int(2);
    fixed delta;
    int scaling = 0;
    int count = 0;

    if (value <= 0)
        return fixed::ARITHMETIC_OVERFLOW;

    fixed inValue = fixed(value);

    // Bring the value to the most accurate range (1 < x < 100)
    const fixed e_to_fourth = fixed(3578144);
    while (inValue > fixed::from_int(100))
    {
        inValue = inValue.div_rounded(e_to_fourth);
        scaling += 4;
    }

    while (inValue < fixed::ONE)
    {
        inValue = inValue * e_to_fourth;
        scaling -= 4;
    }

    do
    {
        // Solving e(x) = y using Newton's method
        // f(x) = e(x) - y
        // f'(x) = e(x)
        fixed e = guess.exp();
        delta = (inValue - e).div_rounded(e);

        // It's unlikely that logarithm is very large, so avoid overshooting.
        if (delta > fixed::from_int(3))
            delta = fixed::from_int(3);

        guess += delta;
    }
    while ((count++ < 10) && (delta != fixed(0)));

    return guess + fixed::from_int(scaling);
}

fixed fixed::div_rounded(const fixed& p_other) const
{
    int64_t temp = (value << 17) / p_other.value;
    return fixed((temp / 2) + (temp % 2));
}

// Adapted from libfixmath: https://github.com/PetteriAimonen/libfixmath
// Copyright 2011-2021 Flatmush <Flatmush@gmail.com>
// License: MIT
fixed fixed::exp() const
{
    if (value == 0)
        return fixed::ONE;
    if (value == fixed::ONE.value)
        return fixed::E;
    if (value >= 681391)
        return fixed(INT64_MAX);
    if (value <= -772243)
        return fixed::ZERO;

    /* The algorithm is based on the power series for exp(x):
     * http://en.wikipedia.org/wiki/Exponential_function#Formal_definition
     *
     * From term n, we get term n+1 by multiplying with x/n.
     * When the sum term drops to zero, we can stop summing.
     */

    bool neg = (value < 0);
    fixed inValue = fixed(neg ? -value : value);

    fixed result = inValue + fixed::ONE;
    fixed term = inValue;

    for (int i = 2; i < 30; i++)
    {
        term = term * (inValue.div_rounded(fixed::from_int(i)));
        result += term;

        if ((term.value < 500) && ((i > 15) || (term.value < 20)))
            break;
    }

    if (neg)
        result = fixed::ONE.div_rounded(result);

    return result;
}

// Adapted from
// https://en.wikipedia.org/wiki/Exponentiation_by_squaring#With_constant_auxiliary_memory,
// Copyright: Wikipedia editors and contributors License: Creative Commons
// Attribution-ShareAlike 3.0 Unported License
fixed fixed::pow_integer(const fixed& exp) const
{
    if (value < 0)
        return fixed::ONE.div_rounded(pow(exp * fixed::NEG_ONE));

    fixed x = fixed(value);
    fixed y = fixed::ONE;
    fixed n = exp;

    if (n < fixed::ZERO)
    {
        x = fixed::ONE.div_rounded(x);
        n *= fixed::NEG_ONE;
    }

    if (n == fixed::ZERO)
        return fixed::ONE;

    while (n > fixed::ONE)
    {
        if ((n % fixed::TWO) == fixed::ZERO)
        {
            x *= x;
            n = n.div_rounded(fixed::TWO);
        }
        else
        {
            y *= x;
            x *= x;
            n = (n - fixed::ONE).div_rounded(fixed::TWO);
        }
    }
    return x * y;
}

// Adapted from the fpm library: https://github.com/MikeLankamp/fpm
// Copyright 2019 Mike Lankamp
// License: MIT
fixed fixed::pow(const fixed& exp) const
{
    if (value == 0)
    {
        ERR_FAIL_COND_V_MSG(
            exp.value < 0, fixed::ZERO,
            "Cannot use negative exponents and a base of 0 with integer exponentiation!");
        return fixed::ZERO;
    }

    if (exp < fixed::ZERO)
        return fixed::ONE.div_rounded(pow(exp * fixed::NEG_ONE));

    if ((exp.value % 65536) == 0)
    {
        if (value < 0)
            if (exp % fixed::TWO == fixed::ZERO)
                return (fixed(value) * fixed::NEG_ONE).pow_integer(exp);
            else
                return (fixed(value) * fixed::NEG_ONE).pow_integer(exp) * fixed::NEG_ONE;
        else
            return pow_integer(exp);
    }

    ERR_FAIL_COND_V_MSG(value < 0, fixed::ZERO,
                        "Negative bases and fractional exponents in pow() are not supported!");

    return (log() * exp).exp();
}
