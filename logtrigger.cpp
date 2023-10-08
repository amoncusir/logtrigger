#include "args.h"
#include "ubus.h"
#include "event.h"
#include "context.h"

#include <iostream>

#include <cstdio>

static void log_callback(struct ubus_log_event* event)
{
    const Logtrigger::Context& ctx {Logtrigger::Context::get_instance()};

    ctx.process_event(event);
}

static void event_ubus_handler(struct ustream* s, int bytes)
{
    parse_ustream_to_event_log(s, bytes, log_callback);
}

int main(int argc, char** argv)
{
    using namespace Logtrigger;

    auto args {Args(argc, argv)};

    if (!args.is_valid())
    {
        std::cerr << "Must be contains at least on trigger configuration" << std::endl;
        args.usage();
        return 0;
    }

    Context ctx {Context::init_instance()};

    for (const auto& item: args.triggers())
    {
        ctx.handler().add_event(generate_from(item), item->script_path);
    }

    ubus_log_callback(args.sock_path(), event_ubus_handler);

    return 0;
}