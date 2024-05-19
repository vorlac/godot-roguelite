#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>

#include <godot_cpp/classes/editor_script.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/thread.hpp>

#include "core/constants.hpp"
#include "entity/level.hpp"
#include "singletons/console.hpp"
#include "ui/main_dialog.hpp"
#include "util/bind.hpp"

namespace rl
{
    class Test : public godot::Node
    {
        GDCLASS(Test, godot::Node)

    public:
        Test() = default;

        virtual void _ready() override
        {
            const auto resource_loader{ rl::resource::loader::get() };
            godot::Ref<godot::Script> script = resource_loader->load("res://scripts/test.gd", "Script");
            godot::Variant var{ script };
            this->set_script(var);

            m_thread = new std::jthread{ [&] {
                this->call("coroutine");
            } };

            signal<event::chunk_generated>::connect(this) <=> signal_callback(this, process_data);
        }

        virtual void _physics_process(double delta_time) override
        {
            m_delay_timer.fetch_add(static_cast<uint32_t>(delta_time * 1000),
                                    std::memory_order_relaxed);
            if (m_delay_timer > 5000)
            {
                m_batch_processed_count = 0;
                m_time_delay_satisfied.notify_one();
            }
        }

    protected:
        void event_processed_test(godot::Node* node)
        {
            // this is the part that isn't working..
            // this never gets called from the gdscript.
            // i don't think the signal is able to connect
            // to the function in the script for some reason
            console::get()->print("GDScript callback triggered");
            node->queue_free();
        }

        void append(const int command_code)
        {
            std::unique_lock lock(m_append_lock);
            m_time_delay_satisfied.wait(lock, [&] {
                // block if less than 10 calls have been made for the current
                // batch of calls
                //
                // m_force_block is a separate flag that can be set to tell
                // it to block regardless of the other conditions checked above
                return m_batch_processed_count < 10 && !m_force_block;
            });

            godot::Node* data = Test::process_command(command_code);
            this->emit_signal(event::chunk_generated, data);

            if (m_batch_processed_count++ < 10)
            {
                console::get()->print("Processing command {} - call: {}",
                                      command_code, m_batch_processed_count);

                m_delay_timer = 0;
                if (m_batch_processed_count == 10)
                    console::get()->print("Blocking for 5 seconds...");
            }
        }

        static godot::Node* process_command(int command_code)
        {
            // since this function is called from a background thraed in gdscript
            // i'm assuming it's going to be called from that same thread when called
            godot::Node* ret{ nullptr };

            switch (command_code)
            {
                case 1:
                    // handle different logic for command 1 in here
                    [[fallthrough]];
                case 2:
                    // handle different logic for command 2 in here
                    [[fallthrough]];
                default:
                    // do some type of work and return what should be emitted
                    // through the signal back over to the gdscript code
                    ret = memnew(godot::Node);
            }

            return ret;
        }

        static void _bind_methods()
        {
            bind_member_function(Test, append);
            bind_member_function(Test, event_processed_test);
            signal_binding<Test, event::chunk_generated>::add<godot::Object*>();
        }

    private:
        std::jthread* m_thread{ nullptr };
        bool m_force_block{ false };
        uint32_t m_batch_processed_count{ 0 };
        std::atomic<uint32_t> m_delay_timer{ 0 };
        std::condition_variable m_time_delay_satisfied{};
        std::mutex m_append_lock{};
    };
}
