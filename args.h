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
        struct TriggerArgs
        {
            const char* type {};
            const char* matcher {};
            const char* script_path {};
            long cold_down = -1;
        };

    private:
        const char* m_name {};

        const char* m_sock_path {};

        std::vector<TriggerArgs*> m_triggers {};

    public:
        Args(int argc, char** argv);

        ~Args();

        const char* sock_path();

        const std::vector<TriggerArgs*>& triggers();

        bool is_valid();

        void usage();

    private:

        static TriggerArgs* make_struct(const char* type, const char* value, const char* path);
    };
}

#endif //LOGTRIGGER_SRC_ARGS_H
