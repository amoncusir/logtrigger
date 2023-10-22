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
            const char* type {};
            const char* script_path {};
            const char* args {};
        };

    private:
        const char* m_name {};

        const char* m_sock_path {};

        std::vector<MatchExec*> m_triggers {};

    public:
        Args(int argc, char** argv);

        ~Args();

        const char* sock_path();

        const std::vector<MatchExec*>& triggers();

        bool is_valid();

        void usage();

    private:

        static MatchExec* make_struct(const char* type, const char* value, const char* path);
    };
}

#endif //LOGTRIGGER_SRC_ARGS_H
