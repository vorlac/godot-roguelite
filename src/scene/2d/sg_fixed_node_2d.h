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

#ifndef SG_FIXED_NODE_2D_H
#define SG_FIXED_NODE_2D_H

#include <godot_cpp/classes/node2d.hpp>

#include "math/sg_fixed_transform_2d.h"
#include "math/sg_fixed_vector2.h"

class SGCollisionObject2D;

class SGFixedNode2D : public Node2D,
                      public SGFixedVector2Watcher
{
    GDCLASS(SGFixedNode2D, Node2D);

    friend SGCollisionObject2D;

    Ref<SGFixedTransform2D> fixed_transform;
    Ref<SGFixedVector2> fixed_scale;
    int64_t fixed_rotation;
    bool fixed_xform_dirty;

#if defined(TOOLS_ENABLED) || defined(DEBUG_ENABLED)
    bool updating_transform;
#endif

protected:
    static void _bind_methods();
    void _notification(int p_what);

#if defined(TOOLS_ENABLED) || defined(DEBUG_ENABLED)
        // virtual void _changed_callback(Object *p_changed, const char *p_prop) override;
#endif

    void _update_fixed_transform_rotation_and_scale();

    _FORCE_INLINE_ SGFixedTransform2DInternal get_fixed_transform_internal() const
    {
        return fixed_transform->get_internal();
    }

    SGFixedTransform2DInternal get_global_fixed_transform_internal() const;

    void update_fixed_transform_internal(const SGFixedTransform2DInternal& p_transform);
    void update_global_fixed_transform_internal(const SGFixedTransform2DInternal& p_global_transform);

    int64_t _get_fixed_position_x() const;
    void _set_fixed_position_x(int64_t p_x);
    int64_t _get_fixed_position_y() const;
    void _set_fixed_position_y(int64_t p_y);

    int64_t _get_fixed_scale_x() const;
    void _set_fixed_scale_x(int64_t p_x);
    int64_t _get_fixed_scale_y() const;
    void _set_fixed_scale_y(int64_t p_y);

    void transform_changed();

public:
    void set_fixed_transform(const Ref<SGFixedTransform2D>& p_transform);
    Ref<SGFixedTransform2D> get_fixed_transform() const;

    void set_fixed_position(const Ref<SGFixedVector2>& p_fixed_position);
    Ref<SGFixedVector2> get_fixed_position() const;

    void set_fixed_scale(const Ref<SGFixedVector2>& p_fixed_scale);
    Ref<SGFixedVector2> get_fixed_scale() const;

    void set_fixed_rotation(int64_t p_fixed_rotation);
    int64_t get_fixed_rotation() const;

    void set_global_fixed_transform(const Ref<SGFixedTransform2D>& p_global_transform);
    Ref<SGFixedTransform2D> get_global_fixed_transform() const;

    void set_global_fixed_position(const Ref<SGFixedVector2>& p_fixed_position);
    Ref<SGFixedVector2> get_global_fixed_position();

    void set_fixed_position_internal(const SGFixedVector2Internal& p_fixed_position);
    void set_global_fixed_position_internal(const SGFixedVector2Internal& p_fixed_position);

    void set_global_fixed_rotation(int64_t p_fixed_rotation);
    int64_t get_global_fixed_rotation() const;

    void update_float_transform();

    void fixed_vector2_changed(SGFixedVector2* p_vector);

    SGFixedNode2D();
    ~SGFixedNode2D();
};

#endif
