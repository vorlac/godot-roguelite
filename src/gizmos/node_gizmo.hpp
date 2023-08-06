#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

class NodeGizmo : public RefCounted
{
    GDCLASS(NodeGizmo, RefCounted);

public:
    NodeGizmo() = default;
    virtual ~NodeGizmo() = default;

    virtual void create() = 0;
    virtual void transform() = 0;
    virtual void clear() = 0;
    virtual void redraw() = 0;
    virtual void free() = 0;
};
