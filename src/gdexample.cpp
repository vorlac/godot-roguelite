#include "gdexample.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    void GDExample::_bind_methods()
    {
        UtilityFunctions::print("Binding methods");

        ClassDB::bind_method(D_METHOD("get_speed"), &GDExample::get_speed);
        ClassDB::bind_method(D_METHOD("set_speed", "speed"), &GDExample::set_speed);

        ADD_GROUP("Movement", "movement_");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movement_speed"), "set_speed", "get_speed");
    }

    void GDExample::set_speed(float_t speed)
    {
        m_speed = speed;
    }

    [[nodiscard]] float_t GDExample::get_speed() const
    {
        return m_speed;
    }

    GDExample::GDExample()
        : m_elapsed_time{ 0 }
        , m_velocity{ 0, 0 }
        , m_speed{ 500 }
    {
        //Transform2D transform(real_t{45.0}, Size2{1.0, 1.0}, real_t{0.0}, Vector2{512, 256});
        //this->set_transform(transform);
    }

    GDExample::~GDExample()
    {
        // Add your cleanup here.
    }

    void GDExample::process_movement(double_t delta)
    {
        m_velocity = Vector2(0.0f, 0.0f);
        Input& intutSingleton = *Input::get_singleton();

        if (intutSingleton.is_action_pressed("ui_right"))
            m_velocity.x += 1.0f;

        if (intutSingleton.is_action_pressed("ui_left"))
            m_velocity.x -= 1.0f;

        if (intutSingleton.is_action_pressed("ui_up"))
            m_velocity.y -= 1.0f;

        if (intutSingleton.is_action_pressed("ui_down"))
            m_velocity.y += 1.0f;

        set_position(get_position() + (m_velocity * m_speed * delta));
    }

    void GDExample::_process(double delta)
    {
        // time_passed += delta;

        // Vector2 new_position = Vector2(10.0 + (10.0 * sin(time_passed * 2.0)),
        //                                10.0 + (10.0 * cos(time_passed * 1.5)));

        // set_position(new_position);
        Node::_process(delta);
        process_movement(delta);
    }
}
