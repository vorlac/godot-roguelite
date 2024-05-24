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

#include "sg_fixed_rect2.h"

void SGFixedRect2::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_position"), &SGFixedRect2::get_position);
    ClassDB::bind_method(D_METHOD("set_position", "position"), &SGFixedRect2::set_position);
    ClassDB::bind_method(D_METHOD("get_size"), &SGFixedRect2::get_size);
    ClassDB::bind_method(D_METHOD("set_size", "size"), &SGFixedRect2::set_size);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "position", PROPERTY_HINT_NONE, "", 0),
                 "set_position", "get_position");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "size", PROPERTY_HINT_NONE, "", 0), "set_size",
                 "get_size");

    ClassDB::bind_method(D_METHOD("has_point", "point"), &SGFixedRect2::has_point);
    ClassDB::bind_method(D_METHOD("intersects", "rect"), &SGFixedRect2::intersects);
    ClassDB::bind_method(D_METHOD("merge", "rect"), &SGFixedRect2::merge);
    ClassDB::bind_method(D_METHOD("expanded", "point"), &SGFixedRect2::expanded);
}

Ref<SGFixedVector2> SGFixedRect2::get_position()
{
    return position;
}

void SGFixedRect2::set_position(const Ref<SGFixedVector2>& p_position)
{
    ERR_FAIL_COND(!p_position.is_valid());
    position->set_internal(p_position->get_internal());
}

Ref<SGFixedVector2> SGFixedRect2::get_size()
{
    return size;
}

void SGFixedRect2::set_size(const Ref<SGFixedVector2>& p_size)
{
    ERR_FAIL_COND(!p_size.is_valid());
    size->set_internal(p_size->get_internal());
}

bool SGFixedRect2::has_point(const Ref<SGFixedVector2>& p_point) const
{
    ERR_FAIL_COND_V(!p_point.is_valid(), false);
    return get_internal().has_point(p_point->get_internal());
}

bool SGFixedRect2::intersects(const Ref<SGFixedRect2>& p_other) const
{
    ERR_FAIL_COND_V(!p_other.is_valid(), false);
    return get_internal().intersects(p_other->get_internal());
}

Ref<SGFixedRect2> SGFixedRect2::merge(const Ref<SGFixedRect2>& p_rect) const
{
    ERR_FAIL_COND_V(!p_rect.is_valid(), Ref<SGFixedRect2>());
    return SGFixedRect2::from_internal(get_internal().merge(p_rect->get_internal()));
}

Ref<SGFixedRect2> SGFixedRect2::expanded(const Ref<SGFixedVector2>& p_vector)
{
    ERR_FAIL_COND_V(!p_vector.is_valid(), Ref<SGFixedRect2>());
    SGFixedRect2Internal internal = get_internal();
    internal.expand_to(p_vector->get_internal());
    return SGFixedRect2::from_internal(internal);
}

SGFixedRect2::SGFixedRect2()
    : position(memnew(SGFixedVector2))
    , size(memnew(SGFixedVector2))
{
}

SGFixedRect2::~SGFixedRect2()
{
}
