#pragma once

#include "nodes/character.hpp"
#include "util/bind.hpp"
#include "util/constants.hpp"

namespace rl
{
    class Player : public Character
    {
        GDCLASS(Player, Character);

    public:
        Player();

        void _ready() override
        {
            Character::_ready();
        }

    protected:
        static void _bind_methods()
        {
            signal_binding<Player, event::spawn_projectile>::add<>();
        }
    };
}
