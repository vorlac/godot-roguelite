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

#ifndef SG_COLLISION_SHAPE_2D_H
#define SG_COLLISION_SHAPE_2D_H

#include "scene/resources/sg_shapes_2d.h"
#include "sg_collision_object_2d.h"
#include "sg_fixed_node_2d.h"

class SGCollisionShape2D : public SGFixedNode2D
{
    GDCLASS(SGCollisionShape2D, SGFixedNode2D);

    Ref<SGShape2D> shape;
    RID collision_object_rid;
    RID rid;
    bool disabled;
    Color debug_color = Color(0.0, 0.6, 0.7, 0.42);

protected:
    static void _bind_methods();
    void _notification(int p_what);

public:
    void set_disabled(bool p_disabled);
    bool get_disabled() const;

    void set_shape(const Ref<SGShape2D>& p_shape);
    Ref<SGShape2D> get_shape();

    void _shape_changed();

    void set_debug_color(const Color& p_color);
    Color get_debug_color() const;

    void sync_to_physics_engine() const;

    _FORCE_INLINE_ RID get_rid() const
    {
        return rid;
    };

    SGCollisionShape2D();
    ~SGCollisionShape2D();
};

#endif
