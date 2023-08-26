#pragma once

namespace rl::inline utils
{
    namespace signal
    {
        struct name
        {
            static constexpr auto position_changed{ "position_changed" };
            static constexpr auto entered_area{ "entered_area" };
            static constexpr auto exited_area{ "exited_area" };
            static constexpr auto shoot_projectile{ "shoot_projectile" };
        };
    }

    namespace path
    {
        namespace sprite
        {
            static constexpr auto Player{ "res://assets/art/placeholder/player.svg" };
            static constexpr auto Enemy{ "res://assets/art/placeholder/enemy.svg" };
            static constexpr auto Bullet{ "res://assets/art/placeholder/bullet.svg" };
            static constexpr auto Background{ "res://assets/art/placeholder/background.png" };
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
