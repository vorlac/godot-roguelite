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
#include "internal/sg_world_2d_internal.h"
#include "servers/sg_physics_2d_server.h"
#include "sg_character_body_2d.h"

void SGCharacterBody2D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_safe_margin"), &SGCharacterBody2D::get_safe_margin);
    ClassDB::bind_method(D_METHOD("set_safe_margin", "fixed_margin"),
                         &SGCharacterBody2D::set_safe_margin);

    ClassDB::bind_method(D_METHOD("get_velocity"), &SGCharacterBody2D::get_velocity);
    ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &SGCharacterBody2D::set_velocity);

    ClassDB::bind_method(D_METHOD("get_up_direction"), &SGCharacterBody2D::get_up_direction);
    ClassDB::bind_method(D_METHOD("set_up_direction", "up_direction"),
                         &SGCharacterBody2D::set_up_direction);

    ClassDB::bind_method(D_METHOD("get_max_slides"), &SGCharacterBody2D::get_max_slides);
    ClassDB::bind_method(D_METHOD("set_max_slides", "max_slides"),
                         &SGCharacterBody2D::set_max_slides);

    ClassDB::bind_method(D_METHOD("get_floor_max_angle"), &SGCharacterBody2D::get_floor_max_angle);
    ClassDB::bind_method(D_METHOD("set_floor_max_angle", "floor_max_angle"),
                         &SGCharacterBody2D::set_floor_max_angle);

    ClassDB::bind_method(D_METHOD("get_floor_normal"), &SGCharacterBody2D::get_floor_normal);
    ClassDB::bind_method(D_METHOD("get_floor_angle", "up_direction"),
                         &SGCharacterBody2D::get_floor_angle, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("is_on_floor"), &SGCharacterBody2D::is_on_floor);
    ClassDB::bind_method(D_METHOD("is_on_ceiling"), &SGCharacterBody2D::is_on_ceiling);
    ClassDB::bind_method(D_METHOD("is_on_wall"), &SGCharacterBody2D::is_on_wall);

    ClassDB::bind_method(D_METHOD("get_slide_count"), &SGCharacterBody2D::get_slide_count);
    ClassDB::bind_method(D_METHOD("get_slide_collision", "slide_idx"),
                         &SGCharacterBody2D::get_slide_collision);
    ClassDB::bind_method(D_METHOD("get_last_slide_collision"),
                         &SGCharacterBody2D::get_last_slide_collision);

    ClassDB::bind_method(D_METHOD("move_and_slide"), &SGCharacterBody2D::move_and_slide);
    ClassDB::bind_method(D_METHOD("rotate_and_slide", "rotation"),
                         &SGCharacterBody2D::rotate_and_slide);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "safe_margin"), "set_safe_margin", "get_safe_margin");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_slides"), "set_max_slides", "get_max_slides");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "floor_max_angle"), "set_floor_max_angle",
                 "get_floor_max_angle");

    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE),
        "set_velocity", "get_velocity");
    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "up_direction", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE),
        "set_up_direction", "get_up_direction");
}

int SGCharacterBody2D::get_safe_margin() const
{
    return SGPhysics2DServer::get_singleton()->body_get_safe_margin(rid);
}

void SGCharacterBody2D::set_safe_margin(int p_safe_margin)
{
    return SGPhysics2DServer::get_singleton()->body_set_safe_margin(rid, p_safe_margin);
}

Ref<SGFixedVector2> SGCharacterBody2D::get_velocity() const
{
    return velocity;
}

void SGCharacterBody2D::set_velocity(const Ref<SGFixedVector2>& p_velocity)
{
    ERR_FAIL_COND(p_velocity.is_null());
    velocity->set_internal(p_velocity->get_internal());
}

Ref<SGFixedVector2> SGCharacterBody2D::get_up_direction() const
{
    return up_direction;
}

void SGCharacterBody2D::set_up_direction(const Ref<SGFixedVector2>& p_up_direction)
{
    ERR_FAIL_COND(p_up_direction.is_null());
    up_direction->set_internal(p_up_direction->get_internal());
}

int64_t SGCharacterBody2D::get_max_slides() const
{
    return max_slides;
}

void SGCharacterBody2D::set_max_slides(int64_t p_max_slides)
{
    ERR_FAIL_COND(p_max_slides < 0);
    max_slides = p_max_slides;
}

int64_t SGCharacterBody2D::get_floor_max_angle() const
{
    return floor_max_angle.value;
}

void SGCharacterBody2D::set_floor_max_angle(int64_t p_floor_max_angle)
{
    floor_max_angle = fixed(p_floor_max_angle);
}

Ref<SGFixedVector2> SGCharacterBody2D::get_floor_normal() const
{
    return floor_normal;
}

int SGCharacterBody2D::get_floor_angle(const Ref<SGFixedVector2>& p_up_direction) const
{
    SGFixedVector2Internal up_direction(fixed::ZERO, fixed::NEG_ONE);
    if (p_up_direction.is_valid())
        up_direction = p_up_direction->get_internal();
    return floor_normal->get_internal().dot(up_direction).acos().value;
}

bool SGCharacterBody2D::is_on_floor() const
{
    return on_floor;
}

bool SGCharacterBody2D::is_on_ceiling() const
{
    return on_ceiling;
}

bool SGCharacterBody2D::is_on_wall() const
{
    return on_wall;
}

int SGCharacterBody2D::get_slide_count() const
{
    return slide_colliders.size();
}

Ref<SGKinematicCollision2D> SGCharacterBody2D::get_slide_collision(int p_bounce)
{
    ERR_FAIL_INDEX_V(p_bounce, slide_colliders.size(), Ref<SGKinematicCollision2D>());
    return slide_colliders[p_bounce];
}

Ref<SGKinematicCollision2D> SGCharacterBody2D::get_last_slide_collision()
{
    if (slide_colliders.size() == 0)
        return Ref<SGKinematicCollision2D>();
    return get_slide_collision(slide_colliders.size() - 1);
}

bool SGCharacterBody2D::move_and_slide()
{
    SGPhysics2DServer* physics_server = SGPhysics2DServer::get_singleton();

    SGFixedVector2Internal motion_internal = velocity->get_internal();
    SGFixedVector2Internal body_velocity = motion_internal;
    SGFixedVector2Internal up_direction_internal = up_direction->get_internal();

    slide_colliders.clear();
    floor_normal->clear();
    on_floor = false;
    on_ceiling = false;
    on_wall = false;

    Ref<SGFixedVector2> motion;
    motion.instantiate();

    bool collided = false;
    int64_t remaining_slides = max_slides;
    while (remaining_slides)
    {
        motion->set_internal(motion_internal);
        Ref<SGKinematicCollision2D> collision = physics_server->body_move_and_collide(rid, motion);
        if (collision.is_null())
        {
            // No collision, so we're good - bail!
            break;
        }
        collided = true;

        SGFixedVector2Internal collision_normal = collision->get_normal()->get_internal();
        SGFixedVector2Internal collision_remainder = collision->get_remainder()->get_internal();

        if (collision_normal == SGFixedVector2Internal::ZERO)
        {
            // This means we couldn't unstuck the body. Clear out the motion
            // vector and bail.
            body_velocity = SGFixedVector2Internal::ZERO;
            break;
        }

        slide_colliders.push_back(collision);

        if (up_direction_internal == SGFixedVector2Internal::ZERO)
        {
            // All is wall!
            on_wall = true;
        }
        else if (collision_normal.dot(up_direction_internal).acos() <= floor_max_angle)
        {
            on_floor = true;
            floor_normal->set_internal(collision_normal);
        }
        else if (collision_normal.dot(-up_direction_internal).acos() <= floor_max_angle)
        {
            on_ceiling = true;
        }
        else
        {
            on_wall = true;
        }

        motion_internal = collision_remainder.slide(collision_normal);
        body_velocity = body_velocity.slide(collision_normal);

        if (motion_internal == SGFixedVector2Internal::ZERO)
        {
            // No remaining motion, so we're good - bail!
            break;
        }

        remaining_slides--;
    }

    // Sync only position from physics server to prevent precision loss.
    set_global_fixed_position_internal(
        physics_server->collision_object_get_internal(rid)->get_transform().get_origin());

    velocity->set_internal(body_velocity);

    return collided;
}

bool SGCharacterBody2D::rotate_and_slide(int64_t p_rotation)
{
    set_fixed_rotation(get_fixed_rotation() + p_rotation);

    sync_to_physics_engine();
    bool stuck = SGPhysics2DServer::get_singleton()->body_unstuck(rid, max_slides);

    // Sync only position from physics server to prevent precision loss.
    set_global_fixed_position_internal(SGPhysics2DServer::get_singleton()
                                           ->collision_object_get_internal(rid)
                                           ->get_transform()
                                           .get_origin());

    return stuck;
}

SGCharacterBody2D::SGCharacterBody2D()
    : SGPhysicsBody2D(SGPhysics2DServer::get_singleton()->collision_object_create(
          SGPhysics2DServer::OBJECT_BODY, SGPhysics2DServer::BODY_CHARACTER))
{
    velocity.instantiate();
    up_direction = Ref<SGFixedVector2>(
        memnew(SGFixedVector2(SGFixedVector2Internal(fixed(0), fixed(-65536)))));
    max_slides = 4;
    floor_max_angle = fixed(51471);

    floor_normal.instantiate();
    on_floor = false;
    on_ceiling = false;
    on_wall = false;
}

SGCharacterBody2D::~SGCharacterBody2D()
{
}
