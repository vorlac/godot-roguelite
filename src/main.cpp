#include "main.hpp"

#include "core/assert.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"
#include "util/input.hpp"

#include <hiredis/hiredis.h>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

namespace rl
{
    Main::Main()
    {
        redisContext* context = redisConnect("localhost", 6379);
        if (context == NULL || context->err)
        {
            if (context)
            {
                UtilityFunctions::print("Connection error: %s\n", context->errstr);
                redisFree(context);
            }
            else
            {
                UtilityFunctions::print("Connection error: Can't allocate redis context\n");
            }
            exit(1);
        }

        UtilityFunctions::print("Redis connection successful");

        // SET a key-value pair
        redisReply* reply = (redisReply*)redisCommand(context, "SET mykey myvalue");
        if (reply == NULL)
        {
            UtilityFunctions::print("SET failed\n");
            exit(1);
        }
        freeReplyObject(reply);
        UtilityFunctions::print("Redis SET successful");

        // GET the value of a key
        redisReply* getReply = (redisReply*)redisCommand(context, "GET mykey");
        if (getReply == NULL)
        {
            UtilityFunctions::print("GET failed\n");
            exit(1);
        }
        UtilityFunctions::print("GET mykey: ", getReply->str);
        freeReplyObject(getReply);

        redisFree(context);

        resource::preload::scene<Level> level{ path::scene::Level1 };
        resource::preload::scene<MainDialog> dialog{ path::ui::MainDialog };

        m_active_level = level.instantiate();
        runtime_assert(m_active_level != nullptr);
        m_main_dialog = dialog.instantiate();
        runtime_assert(m_main_dialog != nullptr);

        if (m_main_dialog != nullptr)
        {
            m_canvas_layer = gdcast<godot::CanvasLayer>(
                m_main_dialog->find_child(name::dialog::canvas_layer, true, false));

            runtime_assert(m_canvas_layer != nullptr);
            if (m_active_level != nullptr && m_canvas_layer != nullptr)
                m_canvas_layer->add_child(m_active_level);

            if (m_main_dialog != nullptr)
                this->add_child(m_main_dialog);
        }
    }

    void Main::_ready()
    {
        this->apply_default_settings();
    }

    void Main::apply_default_settings()
    {
        engine::set_fps(60);
        input::use_accumulated_inputs(false);

        if (not engine::editor_active())
            engine::root_window()->set_size({ 1920, 1080 });
    }
}
