#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include "cppnpc.h"

using namespace godot;

void CPPNPC::_bind_methods()
{
}

CPPNPC::CPPNPC()
{
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

CPPNPC::~CPPNPC()
{
    // Add your cleanup here.
}

void CPPNPC::_ready()
{
    // Generate raycasts
    int raycount = (int)(ANGLE_CONE_OF_VISION / ANGLE_BETWEEN_RAYS);
    for (int i = 0; i < N_RAYCASTS; i++)
    {
        double angle = ANGLE_BETWEEN_RAYS * (i - raycount / 2.0);
        RayCast2D raycast = RayCast2D();
        raycast.set_position(this->get_position());
        raycast.set_target_position(this->direction.rotated(angle) * MAX_VIEW_DISTANCE);
        this->raycast_array[i] = &raycast;
        // add_child(&raycast);
    }
    this->state = Thinking;
}

void CPPNPC::_process(double delta)
{
}

void CPPNPC::_physics_process(double delta)
{
    UtilityFunctions::print("_physics_process!!!");
    if (this->state == Thinking)
        think();
    // else if (this->state == Moving) move_to_target(delta);
}

void CPPNPC::think()
{
    UtilityFunctions::print("Thinking hard!!!");
    // this->target = NULL;
    // for (int i = 0; i < N_RAYCASTS; i++) {
    //	if (this->raycast_array[i]->is_colliding()) {
    //		this->target = dynamic_cast< Node2D* > (this->raycast_array[i]->get_collider());
    //		if(this->target) this->state = Moving;
    //	}
    // }
}

void CPPNPC::move_to_target(double delta)
{
    if (this->target)
    {
        if (this->get_position().distance_to(target->get_position()) >= 30)
        {
            this->set_position(
                this->get_position() +
                (target->get_position() - this->get_position()).normalized() * delta * this->SPEED);
        }
        else
        {
            this->state = Thinking;
        }
    }
}

void CPPNPC::update_raycasts(Vector2 new_direction)
{
    double angle = this->direction.angle_to(new_direction);
    for (int i = 0; i < N_RAYCASTS; i++)
    {
        this->raycast_array[i]->set_target_position(
            this->raycast_array[i]->get_position().rotated(angle));
    }
}