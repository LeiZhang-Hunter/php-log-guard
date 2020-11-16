//
// Created by zhanglei on 2020/11/10.
//

#include "NodeManager.h"
#include "event/EventLoop.h"
#include "app/PHPError.h"
#include "app/PHPFpmError.h"
#include "app/PHPFpmSlow.h"


/**
 * 构造函数
 * 初始化命令行管理工具
 * 初始化配置文件管理工具
 * 初始化事件循环
 * 初始化信号管理器
 */
Node::NodeManager::NodeManager() {
    command = std::make_shared<OS::UnixCommand>();
    iniConfig = std::make_shared<Config::IniConfig>();
    mainLoop = new Event::EventLoop();
    signalDescription = std::make_shared<OS::UnixSignalDescription>();
}

/**
 * 获取用户输入的命令
 * @param argc
 * @param argv
 * @param cmd
 * @return
 */
bool Node::NodeManager::cmdExecutor(int argc, char **argv, const char &cmd) {
    executorCmd = argv[optind];
    return true;
}

/**
 * 获取唯一副本的进程pid
 * @param pidFilePath
 * @return
 */
pid_t Node::NodeManager::getStorageMutexPid(const std::string &pidFilePath) {
    mutexFd = open(pidFilePath.c_str(), O_RDONLY);
    if (mutexFd == -1) {
        std::cerr << "pid file(" << pidFilePath << ") not exist\n" << std::endl;
        exit(-1);
    }


    return 0;
}

/**
 * 设置唯一进程副本的pid
 * @param pidFilePath
 * @return
 */
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

/**
 * 接收到SIGTERM的处理函数
 */
void Node::NodeManager::onStop() {
    mainLoop->stop();
}

/**
 * 运行主程序
 */
void Node::NodeManager::run() {
    //注册命令行解析工具
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

    //存储路径
    std::vector<std::string> pathStorage;
    std::vector<std::map<std::string, std::function<void(const std::string&)>>> handle;

    //检查监控的php错误日志的位置
    std::string php_errors_path = configMap["sentry_log_file"]["php_errors"];
    if (php_errors_path.empty()) {
        std::cerr << "path not empty" << std::endl;
        exit(-1);
    }

    //存储要监控的文件路径
    pathStorage.push_back(php_errors_path);

    //将处理函数加入回调的map中
    std::shared_ptr<App::PHPError> phpErrorHandle = std::make_shared<App::PHPError>();
    std::map<std::string, std::function<void(const std::string&)>> phpErrorMap;
    phpErrorMap[OnReceive] = std::bind(&App::PHPError::onReceive, phpErrorHandle, _1);
    phpErrorMap[OnClose] = std::bind(&App::PHPError::onClose, phpErrorHandle, _1);
    //在容器尾部添加一个元素，这个元素原地构造，不需要触发拷贝构造和转移构造
    handle.emplace_back(phpErrorMap);

    //检查php-fpm的日志
    std::string php_fpm_path = configMap["sentry_log_file"]["php-fpm"];
    if (php_fpm_path.empty()) {
        std::cerr << "php_fpm_path not empty" << std::endl;
        exit(-1);
    }

    //存储要监控的文件路径
    pathStorage.push_back(php_fpm_path);

    //phpFpmError的map
    std::shared_ptr<App::PHPFpmError> phpFpmErrorHandle = std::make_shared<App::PHPFpmError>();
    std::map<std::string, std::function<void(const std::string&)>> PHPFpmErrorMap;
    PHPFpmErrorMap[OnReceive] = std::bind(&App::PHPFpmError::onReceive, phpFpmErrorHandle, _1);
    PHPFpmErrorMap[OnClose] = std::bind(&App::PHPFpmError::onClose, phpFpmErrorHandle, _1);
    //在容器尾部添加一个元素，这个元素原地构造，不需要触发拷贝构造和转移构造
    handle.emplace_back(PHPFpmErrorMap);

    //检查php的慢日志
    std::string php_fpm_slow_path = configMap["sentry_log_file"]["php-fpm-slow"];
    if (php_fpm_slow_path.empty()) {
        std::cerr << "php_fpm_slow_path not empty" << std::endl;
        exit(-1);
    }

    pathStorage.push_back(php_fpm_slow_path);

    std::shared_ptr<App::PHPFpmSlow> PHPFpmSlowHandle = std::make_shared<App::PHPFpmSlow>();
    std::map<std::string, std::function<void(const std::string&)>> PHPFpmSlowMap;
    PHPFpmSlowMap[OnReceive] = std::bind(&App::PHPFpmSlow::onReceive, PHPFpmSlowHandle, _1);
    PHPFpmErrorMap[OnClose] = std::bind(&App::PHPFpmSlow::onClose, PHPFpmSlowHandle, _1);
    //PHPFpmSlowMap，这个元素原地构造，不需要触发拷贝构造和转移构造
    handle.emplace_back(PHPFpmErrorMap);

    if (executorCmd == "stop") {
        //获取程序的pid
        pid_t pid = getStorageMutexPid(pidFile);
    } else if (executorCmd == "help") {

    }

    setStorageMutexPid(pidFile);

    //启动线程池
    int num;

    //将php-fpm php_errors.log 和 php-fpm-slow.log 分别 放到一个线程中做处理
    for (num = 0; num < pathStorage.size(); num++) {

        //初始化线程运行，确保在加入监控之前线程已经运行
        threadPool[num] = std::make_shared<OS::UnixThread>();
        //运行线程
        threadPool[num]->Start();

        //加入文件监控
        std::shared_ptr<OS::UnixInodeWatcher> watcher = std::make_shared<OS::UnixInodeWatcher>(pathStorage[num]);
        if (!watcher->enableWatcher()) {
            std::cerr << watcher->getErrorMsg() << std::endl;
            exit(-1);
        }

        std::shared_ptr<App::FileEvent> fileNotifyEvent = std::make_shared<App::FileEvent>(pathStorage[num], watcher);

        //设置文件发生变化的时候的通知事件
        watcher->setFileEvent(fileNotifyEvent);

        fileNotifyEvent->setOnReceiveApi(handle[num][OnReceive]);
        fileNotifyEvent->setOnCloseApi(handle[num][OnClose]);

        std::shared_ptr<Event::Channel> fileWatcherChannel = std::make_shared<Event::Channel>(threadPool[num]->getEventLoop(),
                                                                                              watcher->getINotifyId());
        fileWatcherChannel->setOnReadCallable(std::bind(&OS::UnixInodeWatcher::watcherOnRead, watcher));
        fileWatcherChannel->enableReading();
        fileChannel[num] = fileWatcherChannel;

    }

    //创建信号处理器
    signalDescription->addMask(SIGTERM);
    //建立信号临界区
    signalDescription->lockProcMask();

    //绑定信号处理器
    std::shared_ptr<Event::Channel> signalChannel = std::make_shared<Event::Channel>(mainLoop,
            signalDescription->getSignalFd());
    //水平触发
    signalChannel->setOnReadCallable(std::bind(&NodeManager::onStop, shared_from_this()));
    signalChannel->enableReading();

    std::cout << "finish" << std::endl;

    //加入
    mainLoop->start();
}

/**
 * 析构释放，删除主循环内存
 */
Node::NodeManager::~NodeManager() {
    delete mainLoop;
}