#pragma once

#include <atomic>
#include <vector>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

#include "core/constants.hpp"
#include "entity/character/player.hpp"
#include "entity/controller/player_controller.hpp"
#include "entity/projectile/projectile_spawner.hpp"
#include "util/bind.hpp"
#include "util/scene.hpp"

namespace godot
{
    class RigidBody2D;
}

namespace rl
{
    class Player;

    class Level : public godot::Node2D
    {
        GDCLASS(Level, godot::Node2D);

    public:
        Level();
        ~Level() = default;

        virtual void _ready() override;
        void _draw() override;
        void _process(double delta_time) override;

        void activate(bool active = true);
        bool active() const;

    protected:
        static void _bind_methods();

        [[signal_slot]] void on_physics_box_entered(godot::Node* node) const;
        [[signal_slot]] void on_physics_box_exited(godot::Node* node) const;
        [[signal_slot]] void on_player_spawn_projectile(godot::Node* obj);
        [[signal_slot]] void on_character_position_changed(const godot::Object* const obj,
                                                           godot::Vector2 location) const;

    private:
        std::atomic<bool> m_active{ false };
        godot::Node* m_background{ nullptr };
        ProjectileSpawner* m_projectile_spawner{ memnew(rl::ProjectileSpawner) };
        resource::preload::scene<Player> player_scene{ path::scene::Player };
        Player* m_player{ player_scene.instantiate(memnew(PlayerController)) };
        godot::RigidBody2D* m_physics_box{ nullptr };
    };
}
