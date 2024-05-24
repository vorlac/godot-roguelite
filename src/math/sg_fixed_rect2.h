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

#ifndef SG_FIXED_RECT2_H
#define SG_FIXED_RECT2_H

#include <godot_cpp/classes/ref_counted.hpp>

#include "internal/sg_fixed_rect2_internal.h"
#include "sg_fixed_vector2.h"

using namespace godot;

class SGFixedRect2 : public RefCounted
{
    GDCLASS(SGFixedRect2, RefCounted);

    Ref<SGFixedVector2> position;
    Ref<SGFixedVector2> size;

protected:
    static void _bind_methods();

public:
    _FORCE_INLINE_ SGFixedRect2Internal get_internal() const
    {
        return SGFixedRect2Internal(position->get_internal(), size->get_internal());
    }

    _FORCE_INLINE_ void set_internal(const SGFixedRect2Internal& p_internal)
    {
        position->set_internal(p_internal.position);
        size->set_internal(p_internal.size);
    }

    Ref<SGFixedVector2> get_position();
    void set_position(const Ref<SGFixedVector2>& p_position);

    Ref<SGFixedVector2> get_size();
    void set_size(const Ref<SGFixedVector2>& p_size);

    bool has_point(const Ref<SGFixedVector2>& p_point) const;
    bool intersects(const Ref<SGFixedRect2>& p_other) const;
    Ref<SGFixedRect2> merge(const Ref<SGFixedRect2>& p_rect) const;
    Ref<SGFixedRect2> expanded(const Ref<SGFixedVector2>& p_vector);

    _FORCE_INLINE_ static Ref<SGFixedRect2> from_internal(const SGFixedRect2Internal& p_internal)
    {
        Ref<SGFixedRect2> ret(memnew(SGFixedRect2));
        ret->set_internal(p_internal);
        return ret;
    }

    SGFixedRect2();
    SGFixedRect2(const SGFixedRect2Internal& p_internal);
    ~SGFixedRect2();
};

#endif
