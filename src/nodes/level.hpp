#pragma once

#include "nodes/character.hpp"
#include "nodes/projectile_spawner.hpp"

#include <atomic>
#include <vector>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

namespace rl
{
    class Level : public godot::Node2D
    {
        GDCLASS(Level, godot::Node2D);

    public:
        Level();
        ~Level();

        void _ready() override;
        void _draw() override;
        void _process(double delta_time) override;

        inline void activate(bool active = true)
        {
            m_active = active;
        }

        inline bool active() const
        {
            return m_active;
        }

    protected:
        [[signal_callback]] void on_character_spawn_projectile(godot::Node* obj);
        [[signal_callback]] void on_character_position_changed(const godot::Object* const obj,
                                                               godot::Vector2 location) const;

        static void _bind_methods();

    private:
        std::vector<signal::connection> m_signal_connections{};
        rl::ProjectileSpawner* m_projectile_spawner{ memnew(rl::ProjectileSpawner) };
        godot::Sprite2D* m_background{ memnew(godot::Sprite2D) };
        rl::Character* m_player{ memnew(rl::Character) };
        std::atomic<bool> m_active{ false };
    };
}
