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
        namespace scene
        {
            constexpr inline auto Level1{ "res://scenes/levels/level1.tscn" };
            constexpr inline auto Player{ "res://scenes/characters/player.tscn" };
            constexpr inline auto Bullet{ "res://scenes/projectiles/bullet.tscn" };
        }

        namespace ui
        {
            constexpr inline auto MainDialog{ "res://scenes/ui/main_dialog.tscn" };
        }
    }
}
