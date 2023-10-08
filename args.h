//
// Created by amoncusir on 04/10/23.
//

#ifndef LOGTRIGGER_SRC_ARGS_H
#define LOGTRIGGER_SRC_ARGS_H

#include <string>
#include <string_view>
#include <vector>

namespace Logtrigger
{
    class Args
    {
    public:
        struct MatchExec
        {
            std::string type {};
            std::string script_path {};
            std::string args {};
        };

    private:
        std::string m_name {""};

        std::string m_sock_path {""};

        std::vector<MatchExec*> m_triggers {};

    public:
        Args(int argc, char** argv);

        ~Args();

        const char* sock_path();

        std::vector<MatchExec*>& triggers();

        bool is_valid();

        void usage();

    private:

        static MatchExec* make_struct(std::string_view type, std::string_view value, std::string_view path);
    };
}

#endif //LOGTRIGGER_SRC_ARGS_H
