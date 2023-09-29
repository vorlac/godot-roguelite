#pragma once

#include "nodes/character.hpp"
#include "nodes/projectile_spawner.hpp"
#include "util/bind.hpp"
#include "util/constants.hpp"
#include "util/scene.hpp"

#include <atomic>
#include <vector>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

namespace rl::inline node
{
    class Character;

    class Level : public godot::Node2D
    {
        GDCLASS(Level, godot::Node2D);

    public:
        Level();
        ~Level() = default;

        void _ready() override;
        void _draw() override;
        void _process(double delta_time) override;

        void activate(bool active = true);
        bool active() const;

    protected:
        static void _bind_methods();

        [[signal_slot]] void on_character_spawn_projectile(godot::Node* obj);
        [[signal_slot]] void on_character_position_changed(const godot::Object* const obj,
                                                           godot::Vector2 location) const;

    private:
        std::atomic<bool> m_active{ false };
        godot::Node* m_background{ nullptr };
        ProjectileSpawner* m_projectile_spawner{ memnew(rl::ProjectileSpawner) };
        resource::preload::scene<Character> player_scene{ path::scene::Player };
        Character* m_player{ player_scene.instantiate() };
    };
}
