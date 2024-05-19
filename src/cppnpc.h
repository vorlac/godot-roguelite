#ifndef CPPNPC_H
#define CPPNPC_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/ray_cast2d.hpp>

#define N_RAYCASTS           10
#define ANGLE_CONE_OF_VISION 0.523599
#define MAX_VIEW_DISTANCE    400.0
#define ANGLE_BETWEEN_RAYS   5.0

namespace godot
{

    class CPPNPC : public Node2D
    {
        GDCLASS(CPPNPC, Node2D)

    private:
    protected:
        static void _bind_methods();

    public:
        CPPNPC();
        ~CPPNPC();

        void _process(double delta) override;
        void _physics_process(double delta) override;
        void _ready() override;
        void move_to_target(double delta);
        void think();
        void update_raycasts(Vector2 new_direction);

        enum State {
            Thinking = 0,
            Moving = 1
        };

        State state;
        Node2D* target;
        double SPEED = 20.0;
        Vector2 direction = Vector2(0.0, -1.0);

        RayCast2D* raycast_array[N_RAYCASTS];
    };

}

#endif