//
// Created by amoncusir on 06/10/23.
//

#include "util.h"

#include "ubus.h"

#include <stdexcept>

#include <unistd.h>

namespace Logtrigger
{
    void run_and_forget_script(const char* script_path, ubus_log_event& event)
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            throw std::runtime_error("Error when try to fork. Code: -1");
        }
        else if (pid == 0)
        {
            //child
            std::string command {script_path};

            command += ' ' + std::to_string(event.id);
            command += ' ' + std::to_string(event.priority);
            command += ' ' + std::to_string(event.source);
            command += ' ' + std::to_string(event.time);
            command += " '" + std::string {event.data} + '\'';

            execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
        }
    }
}