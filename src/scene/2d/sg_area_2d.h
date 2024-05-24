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

#ifndef SG_AREA_2D_H
#define SG_AREA_2D_H

#include <godot_cpp/classes/resource.hpp>

#include "math/sg_fixed_vector2.h"
#include "scene/2d/sg_collision_shape_2d.h"
#include "sg_collision_object_2d.h"

class SGArea2D : public SGCollisionObject2D
{
    GDCLASS(SGArea2D, SGCollisionObject2D);

    bool monitorable = true;

protected:
    static void _bind_methods();

public:
    Array get_overlapping_areas() const;
    Array get_overlapping_bodies() const;

    Array get_overlapping_area_collisions() const;
    Array get_overlapping_body_collisions() const;

    int get_overlapping_area_count() const;
    int get_overlapping_body_count() const;

    void set_monitorable(bool p_monitorable);
    bool get_monitorable() const;

    SGArea2D();
    ~SGArea2D();
};

#endif
