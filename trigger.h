//
// Created by amoncusir on 04/10/23.
//

#ifndef LOGTRIGGER_SRC_TRIGGER_H
#define LOGTRIGGER_SRC_TRIGGER_H

#include "event.h"
#include "ubus.h"

#include <string>
#include <memory>
#include <vector>

#include <cstdint>
#include <ctime>

namespace Logtrigger
{
    class TriggerEvent
    {
    private:
        const EventMatcher* m_matcher {};
        const char* m_script_path {};
        long m_cold_down {-1};
        time_t m_next_time {0};

    public:
        explicit TriggerEvent(EventMatcher* matcher, const char* script_path, int cool_down);

        ~TriggerEvent();

        bool accept(ubus_log_event& event);

        void run(ubus_log_event& event);
    };

    class TriggerHandler
    {
    private:
        std::vector<TriggerEvent*> m_triggers {};

    public:
        explicit TriggerHandler() = default;

        ~TriggerHandler();

        TriggerHandler& add_event(const Args::TriggerArgs* args);

        void process(ubus_log_event& event);
    };
}

#endif //LOGTRIGGER_SRC_TRIGGER_H
