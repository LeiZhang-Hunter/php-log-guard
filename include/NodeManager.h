//
// Created by zhanglei on 2020/11/10.
//

#ifndef LOGSENTRY_NODEMANAGER_H
#define LOGSENTRY_NODEMANAGER_H

#include <syscall.h>

#include <memory>

#include "os/UnixCommand.h"
#include "config/IniConfig.h"
#include "lock/FilePersistenceLock.h"
#include "Noncopyable.h"
#include "os/UnixSignalDescription.h"
#include "event/EventLoop.h"
#include "os/UnixInodeWatcher.h"
#include "os/UnixThread.h"
using namespace std::placeholders;
namespace Node {

    class NodeManager : public std::enable_shared_from_this<NodeManager>, Noncopyable {
    public:
        NodeManager();

        bool getConfigPath(int argc, char **argv, const char &cmd) {
            configPath = argv[optind];
            return true;
        }

        bool cmdExecutor(int argc, char **argv, const char &cmd);

        bool setCommandArgc(int argc) {
            command->setCmdArgC(argc);
            return true;
        }

        bool setCommandArgv(char **argv) {
            command->setCmdArgV(argv);
            return true;
        }

        pid_t getStorageMutexPid(const std::string &pidFilePath);

        pid_t setStorageMutexPid(const std::string &pidFilePath);

        void run();

        void onStop();

        ~NodeManager();

    private:
        //命令行解析工具
        std::shared_ptr<OS::UnixCommand> command;
        //配置文件读取工具
        std::shared_ptr<Config::IniConfig> iniConfig;
        //ini文件读出后的配置
        Config::configMapType config;
        //管理进程的pid
        pid_t managerPid;
        //持久化pid进程文件锁
        Lock::FilePersistenceLock persistLock;
        std::shared_ptr<OS::UnixSignalDescription> signalDescription;
        Event::EventLoop* mainLoop;
        std::string configPath;
        std::string pidFile;
        std::string executorCmd = "start";
        int mutexFd;
        std::map<int, std::shared_ptr<OS::UnixThread>> threadPool;
        std::map<int, std::shared_ptr<Event::Channel>> fileChannel;
    };
}


#endif //LOGSENTRY_NODEMANAGER_H
