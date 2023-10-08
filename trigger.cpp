//
// Created by amoncusir on 04/10/23.
//

#include "trigger.h"
#include "event.h"
#include "ubus.h"
#include "util.h"

#include <string>

namespace Logtrigger
{
    // TriggerEvent ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TriggerEvent::TriggerEvent(EventMatcher* matcher, std::string script_path) :
        m_matcher {matcher}, m_script_path {std::move(script_path)}
    {}

    TriggerEvent::~TriggerEvent()
    {
        delete m_matcher;
    }

    bool TriggerEvent::accept(ubus_log_event& event) const
    {
        return m_matcher->accept(event);
    }

    void TriggerEvent::run(ubus_log_event& event) const
    {
        run_and_forget_script(m_script_path.c_str(), event);
    }

    // TriggerHandler ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    TriggerHandler::~TriggerHandler()
    {
        for (TriggerEvent* item: m_triggers)
        {
            delete item;
        }
    }

    TriggerHandler& TriggerHandler::add_event(EventMatcher* matcher, std::string script_path)
    {
        m_triggers.emplace_back(new TriggerEvent(matcher, std::move(script_path)));
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