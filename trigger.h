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

namespace Logtrigger
{
    class TriggerEvent
    {
    private:
        const EventMatcher* m_matcher {};
        const std::string m_script_path {};

    public:
        explicit TriggerEvent(EventMatcher* matcher, std::string script_path);

        ~TriggerEvent();

        bool accept(ubus_log_event& event) const;

        void run(ubus_log_event& event) const;
    };

    class TriggerHandler
    {
    private:
        std::vector<TriggerEvent*> m_triggers {};

    public:
        explicit TriggerHandler() = default;

        ~TriggerHandler();

        TriggerHandler& add_event(EventMatcher* matcher, std::string script_path);

        void process(ubus_log_event& event);
    };
}

#endif //LOGTRIGGER_SRC_TRIGGER_H
