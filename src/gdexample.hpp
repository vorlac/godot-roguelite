#pragma once

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot
{
    class GDExample : public Sprite2D
    {
        GDCLASS(GDExample, Sprite2D)

    public:
        GDExample();
        ~GDExample();

        void _process(double delta) override;
        
        void set_speed(float speed);
        float get_speed() const;

    protected:
        static void _bind_methods();
        void process_movement(double_t delta);

    private:
        float m_speed{ 500 };
        double m_elapsed_time{ 0 };
        Vector2 m_velocity{ 0, 0 };
    };
}
