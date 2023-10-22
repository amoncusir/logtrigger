//
// Created by amoncusir on 05/10/23.
//

#include "event.h"

#include "ubus.h"

#include <string>
#include <regex>

namespace Logtrigger
{
    EventMatcher* generate_from(const Args::TriggerArgs* exec)
    {
        if (strcmp(exec->type, "all") == 0)
        {
            return new AcceptAllMatcher(exec);
        }
        else if (strcmp(exec->type, "regex") == 0)
        {
            return new RegexDataMatcher(exec);
        }

        throw std::runtime_error("Invalid type for matcher: " + std::string(exec->type));
    }

    // AcceptAllMatcher ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    AcceptAllMatcher::AcceptAllMatcher(const Args::TriggerArgs*)
    {}

    bool Logtrigger::AcceptAllMatcher::accept(const ubus_log_event& event) const
    {
        return true;
    }

    // RegexDataMatcher ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    RegexDataMatcher::RegexDataMatcher(const char* regex) : m_regex {regex, std::regex::ECMAScript}
    {}

    RegexDataMatcher::RegexDataMatcher(const Args::TriggerArgs* exec) : RegexDataMatcher(exec->matcher)
    {}

    bool RegexDataMatcher::accept(const ubus_log_event& event) const
    {
        return std::regex_match(event.data, m_regex);
    }

} // Logtrigger