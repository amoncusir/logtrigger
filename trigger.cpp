//
// Created by amoncusir on 04/10/23.
//

#include "trigger.h"
#include "event.h"
#include "ubus.h"
#include "util.h"

#include <string>
#include <ctime>

namespace Logtrigger
{
    // TriggerEvent ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TriggerEvent::TriggerEvent(EventMatcher* matcher, const char* script_path, int cool_down) :
        m_matcher {matcher}, m_script_path {script_path}, m_cold_down {cool_down}
    {}

    TriggerEvent::~TriggerEvent()
    {
        delete m_matcher;
    }

    bool TriggerEvent::accept(ubus_log_event& event)
    {
        if (m_cold_down > 0)
        {
            time_t current_time = time(nullptr);

            if (m_next_time >= current_time)
            {
                return false;
            }

            m_next_time = current_time + m_cold_down;
        }

        return m_matcher->accept(event);
    }

    void TriggerEvent::run(ubus_log_event& event)
    {
        run_and_forget_script(m_script_path, event);
    }

    // TriggerHandler ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TriggerHandler::~TriggerHandler()
    {
        for (TriggerEvent* item: m_triggers)
        {
            delete item;
        }
    }

    TriggerHandler& TriggerHandler::add_event(const Args::TriggerArgs* args)
    {
        EventMatcher* matcher = generate_from(args);
        auto* event = new TriggerEvent(matcher, args->script_path, args->cold_down);

        m_triggers.emplace_back(event);
        return *this;
    }

    void TriggerHandler::process(ubus_log_event& event)
    {
        for (const auto& item: m_triggers)
        {
            if (item->accept(event))
                item->run(event);
        }
    }
}