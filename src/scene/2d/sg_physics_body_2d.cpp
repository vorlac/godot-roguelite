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

#include "internal/sg_bodies_2d_internal.h"
#include "servers/sg_physics_2d_server.h"
#include "sg_physics_body_2d.h"

void SGPhysicsBody2D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("move_and_collide", "linear_velocity"),
                         &SGPhysicsBody2D::move_and_collide);
}

SGPhysicsBody2D::SGPhysicsBody2D()
    : SGCollisionObject2D()
{
}

SGPhysicsBody2D::SGPhysicsBody2D(RID p_rid)
    : SGCollisionObject2D(p_rid)
{
}

Ref<SGKinematicCollision2D> SGPhysicsBody2D::move_and_collide(
    const Ref<SGFixedVector2>& p_linear_velocity)
{
    ERR_FAIL_COND_V(!p_linear_velocity.is_valid(), Ref<SGKinematicCollision2D>());
    SGPhysics2DServer* physics_server = SGPhysics2DServer::get_singleton();
    Ref<SGKinematicCollision2D> ret = physics_server->body_move_and_collide(rid, p_linear_velocity);
    // Sync only position from physics server to prevent precision loss.
    set_global_fixed_position_internal(
        physics_server->collision_object_get_internal(rid)->get_transform().get_origin());
    return ret;
}
