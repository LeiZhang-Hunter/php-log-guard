//
// Created by zhanglei on 2020/11/10.
//

#include "NodeManager.h"
#include "EventLoop.h"
#include "os/UnixSignalDescription.h"
#include "Channel.h"

bool Node::NodeManager::cmdExecutor(int argc, char **argv, const char &cmd) {
    executorCmd = argv[optind];
    return true;
}

pid_t Node::NodeManager::getStorageMutexPid(const std::string &pidFilePath) {
    mutexFd = open(pidFilePath.c_str(), O_RDONLY);
    if (mutexFd == -1) {
        std::cerr << "pid file(" << pidFilePath << ") not exist\n" << std::endl;
        exit(-1);
    }


    return 0;
}

pid_t Node::NodeManager::setStorageMutexPid(const std::string &pidFilePath) {
    mutexFd = open(pidFilePath.c_str(), O_CREAT | O_RDWR, __S_IREAD|__S_IWRITE);
    if (mutexFd == -1) {
        std::cerr << "pid file(" << pidFilePath << ") " << strerror(errno) << "\n" << std::endl;
        exit(-1);
    }
    int a= 4;

    bool res = persistLock.lock(mutexFd, F_WRLCK);
    if (!res) {
        if (persistLock.getErrno() == EACCES || errno == EAGAIN) {
            std::cerr << "process has been start" << std::endl;
        } else {
            std::cerr << persistLock.getErrorMsg() << std::endl;
        }
        exit(-1);
    }

    pid_t managerPid = syscall(SYS_gettid);

    char buf[sizeof(pid_t)+1];

    bzero(buf,sizeof(buf));

    int r = lseek(mutexFd, 0, SEEK_SET);//清除文件内容

    lseek(mutexFd, 0, SEEK_SET);
    //清空文件内容
    ftruncate(mutexFd, 0);

    //重新写入pid
    snprintf(buf,sizeof(buf)+1,"%d\n", managerPid);

    res = write(mutexFd,buf,strlen(buf));

    if(res <= 0)
    {
        std::cerr << persistLock.getErrorMsg() << std::endl;
        exit(-1);
    }
    return managerPid;
}

void Node::NodeManager::onStop() {
    mainLoop->stop();
}

/**
 * 运行主程序
 */
void Node::NodeManager::run() {
    command->reg('c', std::bind(&NodeManager::getConfigPath, shared_from_this(), _1, _2, _3));
    command->reg('e', std::bind(&NodeManager::cmdExecutor, shared_from_this(), _1, _2, _3));
    command->parse();

    //解析ini的配置文件
    Config::configMapType configMap = iniConfig->readConfig(configPath);

    //校验pid文件路径是否正确
    if (configMap["sentry"]["pid_file"].empty()) {
        std::cerr << "pid file set error" << std::endl;
        exit(-1);
    }

    pidFile = configMap["sentry"]["pid_file"];

    if (executorCmd == "stop") {
        //获取程序的pid
        pid_t pid = getStorageMutexPid(pidFile);
    } else if (executorCmd == "help") {

    }

    setStorageMutexPid(pidFile);

    //校验工作线程数目是否是空的
    if (configMap["sentry"]["file_sentry_thread_number"].empty()) {
        std::cerr << "file_sentry_thread_number set error" << std::endl;
        exit(-1);
    }

    //获取工作线程数目
    int workerNumber = atoi(configMap["sentry"]["file_sentry_thread_number"].c_str());
    if (workerNumber <= 0) {
        std::cerr << "file_sentry_thread_number set error" << std::endl;
        exit(-1);
    }

    mainLoop = new Event::EventLoop();

    //启动线程池
    int num;
    for (num = 0; num < workerNumber; num++) {

    }

    //创建信号处理器
    OS::UnixSignalDescription signalDescription;
    signalDescription.addMask(SIGTERM);
    //建立信号临界区
    signalDescription.lockProcMask();

    //绑定信号处理器
    std::shared_ptr<Event::Channel> signalChannel = std::make_shared<Event::Channel>(mainLoop,
            signalDescription.getSignalFd());
    //水平触发
    signalChannel->setOnReadCallable(std::bind(&NodeManager::onStop, shared_from_this()));
    signalChannel->enableReading();
    mainLoop->start();
}