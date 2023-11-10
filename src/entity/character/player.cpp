#include "core/constants.hpp"
#include "entity/character/character.hpp"
#include "entity/character/player.hpp"
#include "util/scene.hpp"

namespace rl
{
    Player::Player()
        : Character()
    {
        scene::node::set_unique_name(this, name::character::player);
    }

    void Player::_ready()
    {
        Character::_ready();
    }

    void Player::_bind_methods()
    {
    }
}
