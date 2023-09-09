#pragma once

#include <string>

namespace rl::inline constants
{
    namespace event
    {
        constexpr inline auto position_changed{ "position_changed" };
        constexpr inline auto entered_area{ "entered_area" };
        constexpr inline auto exited_area{ "exited_area" };
        constexpr inline auto spawn_projectile{ "spawn_projectile" };
        constexpr inline auto player_move{ "player_move" };
        constexpr inline auto player_rotate{ "player_rotate" };
        constexpr inline auto player_shoot{ "player_shoot" };
    }

    namespace path
    {
        namespace sprite
        {
            constexpr inline auto Player{ "res://assets/art/placeholder/player.svg" };
            constexpr inline auto Enemy{ "res://assets/art/placeholder/enemy.svg" };
            constexpr inline auto Bullet{ "res://assets/art/placeholder/bullet.svg" };
            constexpr inline auto Background{ "res://assets/art/placeholder/background.png" };
        }

        namespace scene
        {
            constexpr auto Bullet{ "res://assets/scenes/projectiles/bullet.tscn" };
        }

        namespace ui
        {
            static constexpr auto MainDialog{ "res://assets/scenes/ui/dialogs/main_dialog.tscn" };
        }
    }
}
