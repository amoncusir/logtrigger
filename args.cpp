//
// Created by amoncusir on 04/10/23.
//

#include "args.h"

#include <iostream>
#include <string>

#include <cstring>

namespace Logtrigger
{
    Args::Args(int argc, char** argv)
    {
        if (argc < 1) throw std::runtime_error("Invalid init. Must be at least one argument");

        this->m_name = *argv;

        for (int i = 1, p = 2; p < argc;)
        {
            if (strlen(argv[i]) != 2)
            {
                std::cerr << "Invalid args. Malformed flags" << '\n';
                return;
            }

            Args::TriggerArgs* triggerArgs {};

            switch (argv[i][1])
            {
                case 's':
                    if (strlen(argv[p]) <= 1)
                    {
                        std::cerr << "Invalid args. Empty value for -s flag" << '\n';
                        return;
                    }

                    this->m_sock_path = argv[p];

                    i += 2;
                    p += 2;
                    break;

                case 't':
                    if (argc - p <= 0)
                    {
                        std::cerr << "Invalid args. Invalid value for -t flag" << '\n';
                        return;
                    }

                    triggerArgs = make_struct(argv[p], argv[p + 1], argv[p + 2]);

                    m_triggers.push_back(triggerArgs);

                    i += 4;
                    p += 4;

                    if (argv[i][0] == '+') // TODO: bad way to add more args...!
                    {
                        if (argv[i][1] == 't')
                        {
                            triggerArgs->cold_down = strtol(argv[p], nullptr, 10);

                            i += 2;
                            p += 2;
                        }
                    }

                    break;
                default:
                    std::cerr << "Invalid args. Unknown flag : -" << argv[i][1] << '\n';
                    return;
            }
        }
    }


    Args::~Args()
    {
        for (Args::TriggerArgs*& item: m_triggers)
        {
            delete item;
        }
    }

    const char* Args::sock_path()
    {
        return m_sock_path;
    }

    const std::vector<Args::TriggerArgs*>& Args::triggers()
    {
        return m_triggers;
    }

    bool Args::is_valid()
    {
        return !(
            this->m_triggers.empty()
        );
    }

    void Args::usage()
    {
        std::cout << "Usage: " << this->m_name << " [options]" << '\n';
        std::cout << "Options:" << '\n';
        std::cout << "    [-s] <socket>                Socket to read" << '\n';
        std::cout << "     -t  <type> <value> <path>   Match configuration" << '\n';
        std::cout << "         [+c <seconds>] Add cold down time for next execution" << '\n';
        std::cout << '\n';
        std::cout << "Match Configuration:" << '\n';
        std::cout << "    Types:" << '\n';
        std::cout << "      0 - All: match with all log events" << '\n';
        std::cout << "      1 - Regex: Regular expression for match" << '\n';
        std::cout << "    Usage:" << '\n';
        std::cout << "      All: ... -t all '' '/mi/abs/script.sh'" << '\n';
        std::cout << "      Regex: ... -t regex '^cool ?regex$' '/mi/abs/script.sh'" << '\n';
        std::cout << "      Regex with cold down: ... -t regex '^cool ?regex$' '/mi/abs/script.sh' +c 3600" << '\n';
        std::cout << "    " << '\n';
        std::cout << std::endl;
    }

    Args::TriggerArgs* Args::make_struct(const char* type, const char* value, const char* path)
    {
        Args::TriggerArgs* match {new Args::TriggerArgs {}};

        match->type = type;
        match->script_path = path;
        match->matcher = value;

        return match;
    }
}