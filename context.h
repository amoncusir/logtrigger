//
// Created by amoncusir on 05/10/23.
//

#ifndef LOGTRIGGER_SRC_CONTEXT_H
#define LOGTRIGGER_SRC_CONTEXT_H

#include "trigger.h"
#include "ubus.h"

namespace Logtrigger
{
    class Context
    {
    private:
        static Context* s_singleton;

        TriggerHandler* m_handler = new TriggerHandler();

        Context() = default;

    public:
        ~Context();

        TriggerHandler& handler();

        void process_event(ubus_log_event* event) const;

        static Context& init_instance();

        static const Context& get_instance();
    };
}

#endif //LOGTRIGGER_SRC_CONTEXT_H
