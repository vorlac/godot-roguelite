#pragma once

#include "core/projectile_spawner.hpp"
#include "nodes/character.hpp"
#include "util/utils.hpp"

#include <utility>
#include <vector>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace rl
{
    class LevelLoader : public godot::Node2D
    {
        GDCLASS(LevelLoader, godot::Node2D);

    public:
        LevelLoader();
        ~LevelLoader();

        void _ready() override;
        void _draw() override;

    protected:
        [[signal_callback]]
        void on_position_changed(const godot::Object* const obj, godot::Vector2 location) const;
        [[signal_callback]]
        void on_shoot_projectile(const godot::Object* const obj);

        static void _bind_methods()
        {
            godot::ClassDB::bind_method(godot::D_METHOD("on_position_changed"),
                                        &LevelLoader::on_position_changed);
            godot::ClassDB::bind_method(godot::D_METHOD("on_shoot_projectile"),
                                        &LevelLoader::on_shoot_projectile);
        }

    private:
        using callback_connection = std::pair<godot::String, godot::Callable>;
        std::vector<callback_connection> m_signal_connections{};

        rl::ProjectileSpawner* m_projectile_spawner{ memnew(rl::ProjectileSpawner) };
        godot::Sprite2D* m_background{ memnew(godot::Sprite2D) };
        Character* m_player{ memnew(Character) };
    };
}
