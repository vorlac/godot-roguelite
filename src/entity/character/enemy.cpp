#include "core/constants.hpp"
#include "entity/character/character.hpp"
#include "entity/character/enemy.hpp"
#include "util/scene.hpp"

namespace rl
{
    Enemy::Enemy()
        : Character()
    {
        scene::node::set_unique_name(this, name::character::enemy);
    }

    void Enemy::_bind_methods()
    {
    }
}
