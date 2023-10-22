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

                    m_triggers.push_back(make_struct(argv[p], argv[p + 1], argv[p + 2]));

                    i += 4;
                    p += 4;
                    break;
                default:
                    std::cerr << "Invalid args. Unknown flag : -" << argv[i][1] << '\n';
                    return;
            }
        }
    }


    Args::~Args()
    {
        for (Args::MatchExec*& item: m_triggers)
        {
            delete item;
        }
    }

    const char* Args::sock_path()
    {
        return m_sock_path;
    }

    const std::vector<Args::MatchExec*>& Args::triggers()
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
        std::cout << '\n';
        std::cout << "Match Configuration:" << '\n';
        std::cout << "    Types:" << '\n';
        std::cout << "      0 - All: match with all log events" << '\n';
        std::cout << "      1 - Regex: Regular expression for match" << '\n';
        std::cout << "    Usage:" << '\n';
        std::cout << "      All: ... -t all '' '/mi/abs/script.sh'" << '\n';
        std::cout << "      All: ... -t regex '^cool ?regex$' '/mi/abs/script.sh'" << '\n';
        std::cout << "    " << '\n';
        std::cout << std::endl;
    }

    Args::MatchExec* Args::make_struct(const char* type, const char* value, const char* path)
    {
        Args::MatchExec* match {new Args::MatchExec {}};

        match->type = type;
        match->script_path = path;
        match->args = value;

        return match;
    }
}