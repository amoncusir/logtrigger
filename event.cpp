//
// Created by amoncusir on 05/10/23.
//

#include "event.h"

#include "ubus.h"

#include <string>
#include <regex>

namespace Logtrigger
{
    EventMatcher* generate_from(Args::MatchExec* exec)
    {
        if (exec->type == "all")
        {
            return new AcceptAllMatcher(exec);
        }
        else if (exec->type == "regex")
        {
            return new RegexDataMatcher(exec);
        }

        throw std::runtime_error("Invalid type for matcher: " + exec->type);
    }

    // AcceptAllMatcher ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    AcceptAllMatcher::AcceptAllMatcher(Args::MatchExec*)
    {}

    bool Logtrigger::AcceptAllMatcher::accept(ubus_log_event& event) const
    {
        return true;
    }

    // RegexDataMatcher ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    RegexDataMatcher::RegexDataMatcher(std::string& regex) : m_regex {regex.c_str(), std::regex::ECMAScript}
    {}

    RegexDataMatcher::RegexDataMatcher(Args::MatchExec* exec) : RegexDataMatcher(exec->args)
    {}

    bool RegexDataMatcher::accept(ubus_log_event& event) const
    {
        return std::regex_match(event.data, m_regex);
    }

} // Logtrigger