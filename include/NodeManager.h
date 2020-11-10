//
// Created by zhanglei on 2020/11/10.
//

#ifndef LOGSENTRY_NODEMANAGER_H
#define LOGSENTRY_NODEMANAGER_H

#include <memory>

#include "UnixCommand.h"
using namespace std::placeholders;
namespace Node {
class NodeManager : public std::enable_shared_from_this<NodeManager> {
    public:
        NodeManager () {
            command = std::make_shared<OS::UnixCommand>();
        }
        bool getConfigPath(int argc, char** argv, const char &cmd) {
            configPath = argv[optind];
            return true;
        }

        bool setCommandArgc (int argc) {
            command->setCmdArgC(argc);
            return true;
        }

        bool setCommandArgv (char** argv) {
            command->setCmdArgV(argv);
            return true;
        }

        void run();

        ~NodeManager() {

        }
    private:
        std::shared_ptr<OS::UnixCommand> command;
        std::string configPath;
    };
}


#endif //LOGSENTRY_NODEMANAGER_H
