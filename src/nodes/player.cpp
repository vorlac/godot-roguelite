#include "nodes/player.hpp"

#include "nodes/character.hpp"
#include "util/constants.hpp"
#include "util/scene.hpp"

namespace rl
{
    Player::Player()
        : Character()
    {
        scene::node::set_unique_name(this, name::character::player);
    }
}
