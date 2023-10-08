//
// Created by amoncusir on 05/10/23.
//

#include "context.h"

#include <stdexcept>
#include <cstdio>

namespace Logtrigger
{
    Context* Context::s_singleton = nullptr;

    Context::~Context()
    {
        delete m_handler;
    }

    TriggerHandler& Context::handler()
    {
        return *m_handler;
    }

    void Context::process_event(ubus_log_event* event) const
    {
        try
        {
            m_handler->process(*event);
        }
        catch (...)
        {
            fprintf(stderr, "Error on handler process\n");
        }
    }

    Context& Context::init_instance()
    {
        if (s_singleton != nullptr)
        {
            throw std::runtime_error("Initialized Context");
        }

        s_singleton = new Context();

        return *s_singleton;
    }

    const Context& Context::get_instance()
    {
        if (s_singleton == nullptr)
        {
            throw std::runtime_error("Uninitialized Context. First call Context::init_instance");
        }

        return *s_singleton;
    }
}
