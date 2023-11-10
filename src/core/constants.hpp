#pragma once

#include <cstdint>
#include <string>

namespace rl::inline constants
{
    namespace name
    {
        namespace level
        {
            constexpr inline auto level1{ "Level1" };
            constexpr inline auto physics_box{ "PhysicsBox" };
        }

        namespace dialog
        {
            constexpr inline auto console{ "ConsolePanel" };
            constexpr inline auto canvas_layer{ "MainCanvasLayer" };

        }

        namespace character
        {
            constexpr inline auto player{ "Player" };
            constexpr inline auto enemy{ "Enemy" };
            constexpr inline auto firing_pt{ "FiringPoint" };
        }

    }

    namespace event
    {
        constexpr inline auto position_changed{ "position_changed" };
        constexpr inline auto entered_area{ "entered_area" };
        constexpr inline auto exited_area{ "exited_area" };
        constexpr inline auto spawn_projectile{ "spawn_projectile" };
        constexpr inline auto character_move{ "character_move" };
        constexpr inline auto character_rotate{ "character_rotate" };
        constexpr inline auto character_shoot{ "character_shoot" };
        constexpr inline auto body_entered{ "body_entered" };
        constexpr inline auto body_exited{ "body_exited" };
    }

    enum class LayerID : uint32_t {
        Player = 0x00000001,
        NPCs = 0x00000002,
        Projectiles = 0x00000004,
        Walls = 0x00000008,
        DamageZones = 0x00000010,
        DeathZones = 0x00000020,
        PhysicsObjects = 0x00000040,
        Layer08 = 0x00000080,
        Layer09 = 0x00000100,
        Layer10 = 0x00000200,
        Layer11 = 0x00000400,
        Layer12 = 0x00000800,
        Layer13 = 0x00001000,
        Layer14 = 0x00002000,
        Layer15 = 0x00004000,
        Layer16 = 0x00008000,
    };

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
