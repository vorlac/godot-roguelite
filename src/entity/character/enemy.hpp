#pragma once

#include "core/constants.hpp"
#include "entity/character/character.hpp"
#include "util/bind.hpp"

namespace rl
{
    class Enemy : public Character
    {
        GDCLASS(Enemy, Character);

    public:
        Enemy();

        using Character::_ready;

    protected:
        static void _bind_methods();
    };
}
