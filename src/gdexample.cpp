#include "gdexample.hpp"

#include <cstdio>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    void GDExample::_bind_methods()
    {
        UtilityFunctions::print("Binding methods");

        ClassDB::bind_method(D_METHOD("get_speed"), &GDExample::get_speed);
        ClassDB::bind_method(D_METHOD("set_speed", "speed"), &GDExample::set_speed);

        ADD_GROUP("Movement", "movement_");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movement_speed", PropertyHint::PROPERTY_HINT_RANGE, "0,20,0.01"),
                     "set_speed", "get_speed");
    }

    void GDExample::set_speed(float speed)
    {
        m_speed = speed;
    }

    float GDExample::get_speed() const
    {
        return m_speed;
    }

    GDExample::GDExample()
        : m_elapsed_time{ 0 }
        , m_velocity{ 0, 0 }
        , m_speed{ 500 }
    {
        this->set_process_input(true);
    }

    GDExample::~GDExample()
    {
    }

    void GDExample::process_movement(double_t delta)
    {
        Input* input = Input::get_singleton();
        // Vector2 rotation{ input->get_vector("rotation_left", "rotation_right", "rotation_up", "rotation_down") };
        // Vector2 movement{ input->get_vector("movement_left", "movement_right", "movement_up", "movement_down") };

        //        printf("dir={%2.2lf,%2.2lf}", rotation.x, rotation.y);

        m_velocity = { 0.0f, 0.0f };

        if (input->is_action_pressed("ui_right"))
            m_velocity.x += 1.0f;

        if (input->is_action_pressed("ui_left"))
            m_velocity.x -= 1.0f;

        if (input->is_action_pressed("ui_up"))
            m_velocity.y -= 1.0f;

        if (input->is_action_pressed("ui_down"))
            m_velocity.y += 1.0f;

        set_position(get_position() + (m_velocity * m_speed * delta));
    }

    void GDExample::_process(double delta)
    {
        Node::_process(delta);
        process_movement(delta);
    }
}
