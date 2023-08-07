#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot
{
    class Gizmo : Resource
    {
        GDCLASS(Gizmo, Resource);

    public:
        Gizmo() = default;
        virtual ~Gizmo() = default;

        virtual void _draw();

        Dictionary shapes{};
    };
}
