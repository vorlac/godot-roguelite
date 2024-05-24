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

#ifndef SG_FIXED_RECT2_INTERNAL_H
#define SG_FIXED_RECT2_INTERNAL_H

#include "sg_fixed_vector2_internal.h"

struct SGFixedRect2Internal
{
    SGFixedVector2Internal position;
    SGFixedVector2Internal size;

    _FORCE_INLINE_ SGFixedRect2Internal()
    {
    }

    _FORCE_INLINE_ SGFixedRect2Internal(SGFixedVector2Internal p_position,
                                        SGFixedVector2Internal p_size)
        : position(p_position)
        , size(p_size)
    {
    }

    _FORCE_INLINE_ SGFixedVector2Internal get_min() const
    {
        return position;
    }

    _FORCE_INLINE_ SGFixedVector2Internal get_max() const
    {
        return position + size;
    }

    inline bool has_point(const SGFixedVector2Internal& p_point) const
    {
        if (p_point.x < position.x)
            return false;
        if (p_point.y < position.y)
            return false;

        if (p_point.x >= (position.x + size.x))
            return false;
        if (p_point.y >= (position.y + size.y))
            return false;

        return true;
    }

    inline bool intersects(const SGFixedRect2Internal& p_other) const
    {
        SGFixedVector2Internal min_one = get_min();
        SGFixedVector2Internal max_one = get_max();
        SGFixedVector2Internal min_two = p_other.get_min();
        SGFixedVector2Internal max_two = p_other.get_max();

        return (min_two.x <= max_one.x) && (min_one.x <= max_two.x) && (min_two.y <= max_one.y) &&
               (min_one.y <= max_two.y);
    }

    inline SGFixedRect2Internal merge(const SGFixedRect2Internal& p_rect) const
    {
        SGFixedRect2Internal new_rect;

        new_rect.position.x = MIN(p_rect.position.x, position.x);
        new_rect.position.y = MIN(p_rect.position.y, position.y);

        new_rect.size.x = MAX(p_rect.position.x + p_rect.size.x, position.x + size.x);
        new_rect.size.y = MAX(p_rect.position.y + p_rect.size.y, position.y + size.y);

        // Make relative again.
        new_rect.size = new_rect.size - new_rect.position;

        return new_rect;
    };

    inline void grow_by(fixed p_by)
    {
        position.x -= p_by;
        position.y -= p_by;
        size.width += p_by * fixed::TWO;
        size.height += p_by * fixed::TWO;
    }

    inline void expand_to(const SGFixedVector2Internal& p_vector)
    {
        SGFixedVector2Internal begin = position;
        SGFixedVector2Internal end = position + size;

        if (p_vector.x < begin.x)
            begin.x = p_vector.x;
        if (p_vector.y < begin.y)
            begin.y = p_vector.y;

        if (p_vector.x > end.x)
            end.x = p_vector.x;
        if (p_vector.y > end.y)
            end.y = p_vector.y;

        position = begin;
        size = end - begin;
    }
};

#endif
