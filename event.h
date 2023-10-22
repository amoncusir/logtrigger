//
// Created by amoncusir on 05/10/23.
//

#ifndef LOGTRIGGER_SRC_EVENT_H
#define LOGTRIGGER_SRC_EVENT_H

#include "ubus.h"

#include "args.h"

#include <string>
#include <regex>
#include <cstdint>

namespace Logtrigger
{
    class EventMatcher
    {
    public:
        virtual ~EventMatcher() = default;

        virtual bool accept(const ubus_log_event& event) const = 0;
    };

    EventMatcher* generate_from(const Args::MatchExec * exec);

    class AcceptAllMatcher : public EventMatcher
    {
    public:
        explicit AcceptAllMatcher(const Args::MatchExec* exec);

        bool accept(const ubus_log_event& event) const override;
    };

    class RegexDataMatcher : public EventMatcher
    {
    private:
        const std::regex m_regex {};

    public:
        explicit RegexDataMatcher(const char* regex);

        explicit RegexDataMatcher(const Args::MatchExec* exec);

        bool accept(const ubus_log_event& event) const override;
    };
} // Logtrigger

#endif //LOGTRIGGER_SRC_EVENT_H
