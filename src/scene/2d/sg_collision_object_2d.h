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

#ifndef SG_COLLISION_OBJECT_2D_H
#define SG_COLLISION_OBJECT_2D_H

#include "math/sg_fixed_vector2.h"
#include "sg_fixed_node_2d.h"

class SGCollisionObject2D : public SGFixedNode2D
{
    GDCLASS(SGCollisionObject2D, SGFixedNode2D);

    friend class SGCollisionShape2D;
    friend class SGCollisionPolygon2D;

    uint32_t collision_layer;
    uint32_t collision_mask;

protected:
    RID rid;
    RID world_rid;

    static void _bind_methods();
    void _notification(int p_what);

    void sync_from_physics_engine();

    SGCollisionObject2D(RID p_rid);

public:
    virtual PackedStringArray _get_configuration_warnings() const override;

    virtual void sync_to_physics_engine() const;

    _FORCE_INLINE_ RID get_rid() const
    {
        return rid;
    }

    void set_world(RID p_world);

    RID get_world() const
    {
        return world_rid;
    }

    uint32_t get_collision_layer() const;
    void set_collision_layer(uint32_t p_collision_layer);

    uint32_t get_collision_mask() const;
    void set_collision_mask(uint32_t p_collision_mask);

    void set_collision_layer_bit(int p_bit, bool p_value);
    bool get_collision_layer_bit(int p_bit) const;

    void set_collision_mask_bit(int p_bit, bool p_value);
    bool get_collision_mask_bit(int p_bit) const;

    SGCollisionObject2D();
    ~SGCollisionObject2D();
};

#endif
