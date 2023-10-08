//
// Created by amoncusir on 06/10/23.
//

#ifndef LOGTRIGGER_SRC_UTIL_H
#define LOGTRIGGER_SRC_UTIL_H

#include "ubus.h"

namespace Logtrigger
{
    void run_and_forget_script(const char* script_path, ubus_log_event& event);
}

#endif //LOGTRIGGER_SRC_UTIL_H
