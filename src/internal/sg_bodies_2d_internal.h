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

#ifndef SG_BODIES_2D_INTERNAL_H
#define SG_BODIES_2D_INTERNAL_H

#include <vector>

#include "sg_broadphase_2d_internal.h"
#include "sg_shapes_2d_internal.h"

class SGWorld2DInternal;

class SGCollisionObject2DInternal
{
public:
    enum ObjectType {
        OBJECT_AREA = 1,
        OBJECT_BODY = 2,
        OBJECT_BOTH = (OBJECT_AREA | OBJECT_BODY),
    };

private:
    ObjectType object_type;
    SGFixedTransform2DInternal transform;
    std::vector<SGShape2DInternal*> shapes;
    SGWorld2DInternal* world;
    SGBroadphase2DInternal* broadphase;
    SGBroadphase2DInternal::Element* broadphase_element;
    void* data;

    uint32_t collision_layer;
    uint32_t collision_mask;
    bool monitorable;

    friend class SGWorld2DInternal;

    _FORCE_INLINE_ void set_world(SGWorld2DInternal* p_world)
    {
        world = p_world;
    }

public:
    _FORCE_INLINE_ ObjectType get_object_type() const
    {
        return object_type;
    }

    _FORCE_INLINE_ SGWorld2DInternal* get_world() const
    {
        return world;
    }

    _FORCE_INLINE_ SGFixedTransform2DInternal get_transform() const
    {
        return transform;
    }

    void set_transform(const SGFixedTransform2DInternal& p_transform);

    void add_shape(SGShape2DInternal* p_shape);
    void remove_shape(SGShape2DInternal* p_shape);

    _FORCE_INLINE_ const std::vector<SGShape2DInternal*>& get_shapes() const
    {
        return shapes;
    }

    SGFixedRect2Internal get_bounds() const;

    void add_to_broadphase(SGBroadphase2DInternal* p_broadphase);
    void remove_from_broadphase();

    _FORCE_INLINE_ void set_data(void* p_data)
    {
        data = p_data;
    }

    _FORCE_INLINE_ void* get_data() const
    {
        return data;
    }

    _FORCE_INLINE_ void set_collision_layer(uint32_t p_collision_layer)
    {
        collision_layer = p_collision_layer;
    }

    _FORCE_INLINE_ uint32_t get_collision_layer() const
    {
        return collision_layer;
    }

    _FORCE_INLINE_ void set_collision_mask(uint32_t p_collision_mask)
    {
        collision_mask = p_collision_mask;
    }

    _FORCE_INLINE_ uint32_t get_collision_mask() const
    {
        return collision_mask;
    }

    void set_monitorable(bool p_monitorable);

    _FORCE_INLINE_ bool get_monitorable()
    {
        return monitorable;
    }

    _FORCE_INLINE_ bool test_collision_layers(SGCollisionObject2DInternal* p_other) const
    {
        return (collision_layer & p_other->collision_mask) ||
               (p_other->collision_layer & collision_mask);
    }

    SGCollisionObject2DInternal(ObjectType p_type);
    virtual ~SGCollisionObject2DInternal();
};

class SGArea2DInternal : public SGCollisionObject2DInternal
{
public:
    SGArea2DInternal();
    ~SGArea2DInternal();
};

class SGBody2DInternal : public SGCollisionObject2DInternal
{
public:
    enum BodyType {
        BODY_STATIC = 1,
        BODY_KINEMATIC = 2,
    };

protected:
    BodyType body_type;
    fixed safe_margin;

public:
    _FORCE_INLINE_ BodyType get_body_type() const
    {
        return body_type;
    }

    void set_safe_margin(fixed p_safe_margin)
    {
        safe_margin = p_safe_margin;
    }

    _FORCE_INLINE_ fixed get_safe_margin() const
    {
        return safe_margin;
    }

    SGBody2DInternal(BodyType p_type);
    ~SGBody2DInternal();
};

#endif
