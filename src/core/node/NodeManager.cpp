//
// Created by zhanglei on 2020/11/10.
//

#include <libgen.h>
#include "NodeManager.h"
#include "event/EventLoop.h"
#include "app/PHPError.h"


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
 * 获取用户输入的命令,设置进程名字
 * @param argc
 * @param argv
 * @param cmd
 * @return
 */
bool Node::NodeManager::reName(int argc, char **argv, const char &cmd) {
    processName = argv[optind];
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

    //持久化pid进程文件锁
    char buf[64];
    size_t res = read(mutexFd, &buf, sizeof(buf));
    if (res == -1) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
    }
    pid_t  pid = atoi(buf);
    return pid;
}

/**
 * 设置唯一进程副本的pid
 * @param pidFilePath
 * @return
 */
pid_t Node::NodeManager::setStorageMutexPid(const std::string &pidFilePath) {
    mutexFd = open(pidFilePath.c_str(), O_CREAT | O_RDWR, 0664);
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
    std::stringstream pidStr;
    pidStr << managerPid;
//    std::string buffer(static_cast<char*>(managerPid));

    lseek(mutexFd, 0, SEEK_SET);
    //清空文件内容
    ftruncate(mutexFd, 0);

    //重新写入pid
    res = write(mutexFd, pidStr.str().c_str(), (pidStr.str().length()));

    if(res <= 0)
    {
        std::cerr << persistLock.getErrorMsg() << std::endl;
        exit(-1);
    }
    return managerPid;
}

int Node::NodeManager::createDaemon()
{
    pid_t pid = fork();

    if(pid < 0)
    {
        std::cerr << "create daemon process error" << std::endl;
        return  -1;
    }else if(pid > 0)
    {
        //杀死掉父进程
        exit(0);
    }

    //如果说设置进程组组长失败，
    if(setsid() < 0)
    {
        std::cerr << "create process leader error" << std::endl;
        return  -1;
    }


    //这里需要忽略掉信号
    signal(SIGHUP,SIG_IGN);

    pid = fork();

    if(pid < 0)
    {
        return  -1;
    }else if(pid)
    {
        exit(0);
    }

    chdir("/");

    //改变描述符指向,没有做日志,为了方便起见，不做输入输出重定向了
//    int i;
//
//    for(i=0;i<64;i++)
//    {
//        close(i);
//    }


    /*
    open("/dev/null",O_RDONLY);
    open("/dev/null",O_RDWR);
    open("/dev/null",O_RDWR);
     */

    //运行run函数
    return  0;
}

/**
 * 接收到SIGTERM的处理函数
 */
void Node::NodeManager::onStop() {
    struct signalfd_siginfo fdsi;
    ssize_t size;

    size = read(signalDescription->getSignalFd(), &fdsi, sizeof(struct signalfd_siginfo));

    if (size == -1) {
        std::cerr << signalDescription->getErrorMsg() << std::endl;
        exit(-1);
    }

    if (size != sizeof(struct signalfd_siginfo)) {
        std::cerr << "read signalfd error!" << std::endl;
    }

    if (fdsi.ssi_signo == SIGTERM) {

        {
            std::map<int, std::shared_ptr<OS::UnixThread>>::iterator threadIterator;
            for(threadIterator = threadPool.begin(); threadIterator != threadPool.end(); threadIterator ++) {
                threadIterator->second->Stop();
                threadIterator->second->getEventLoop()->wakeup();
                threadIterator->second.reset();
            }
        }
        mainLoop->stop();
    }
}

/**
 * 运行主程序
 */
void Node::NodeManager::run() {
    //注册命令行解析工具
    command->reg('c', std::bind(&NodeManager::setConfigPath, shared_from_this(), _1, _2, _3));
    command->reg('e', std::bind(&NodeManager::cmdExecutor, shared_from_this(), _1, _2, _3));
    command->reg('n', std::bind(&NodeManager::reName, shared_from_this(), _1, _2, _3));
    command->parse();

    //没有配置默认从当前项目下config.ini去读取配置
    if (configPath.empty() && command->getCmdArgv()[0]) {
        configPath = dirname(command->getCmdArgv()[0]);
        configPath += "/config/config.ini";
    }

    //检查进程名字是否存在，存在的话设置进程名字
    if (!processName.empty()) {
        std::shared_ptr<OS::UnixUtil> util = std::make_shared<OS::UnixUtil>();
        util->setProcTitleInit(command->getCmdArgC(), command->getCmdArgv(), environ);
        util->setProcTitle("%s", processName.c_str());
    }

    std::string iniConfigPath = getConfigPath();
    //解析ini的配置文件
    Config::configMapType configMap = iniConfig->readConfig(iniConfigPath);

    //校验pid文件路径是否正确
    if (configMap["sentry"]["pid_file"].empty()) {
        std::cerr << "pid file set error" << std::endl;
        exit(-1);
    }

    pidFile = configMap["sentry"]["pid_file"];

    //最大的buffer长度
    std::string maxBuffer = configMap["sentry_log_config"]["max_buffer"];
    if (maxBuffer.empty()) {
        std::cerr << "sentry_log_config[max_buffer] must not be empty" << std::endl;
        exit(-1);
    }

    size_t maxBufferSize = static_cast<size_t>(atol(maxBuffer.c_str()));

    if (maxBufferSize < 0) {
        std::cerr << "sentry_log_config[max_buffer](" << maxBufferSize << ") error!" << std::endl;
    }

    //存储路径
    std::vector<std::string> pathStorage;
    std::vector<std::map<std::string, std::function<void(const std::string&)>>> handle;

    //检查输出路径的合理性
    std::string outPath = configMap["sentry_log_config"]["output-file"];
    if (outPath.empty()) {
        std::cerr << "sentry_log_config[output-file]" << outPath << " not empty!" << std::endl;
        exit(-1);
    }

    //检查输出的合理性
    if (access(outPath.c_str(), F_OK) == -1) {
        std::cerr << "sentry_log_config[output-file]:" << outPath << " not exist!" << std::endl;
        exit(-1);
    }

    //检查填写的是否是一个目录
    struct stat fileStat;
    int res = stat(outPath.c_str(), &fileStat);
    if (res == -1) {
        std::cerr << "sentry_log_config[output-file] fileStat error:" << outPath << "; error" << strerror(errno) << std::endl;
        exit(-1);
    }

    //检查是否是一个目录
    if (!S_ISDIR(fileStat.st_mode)) {
        std::cerr << "sentry_log_config[output-file] fileStat error:" << outPath << " must be dir"  << std::endl;
        exit(-1);
    }

    //获取应用的名字
    char name[65];
    char* char_app_name = nullptr;
    std::string appName(name);
    char_app_name = getenv("APP_NAME");
    //首先去环境变量里面找，找不到再去hostname里面去找
    if (char_app_name) {
        appName = char_app_name;
        if (appName.empty()) {
            std::cerr << "appName empty!" << std::endl;
            exit(-1);
        }
    } else {
        res = gethostname(name, sizeof(name));
        if (res == -1) {
            std::cerr << "gethostname error:" << strerror(errno)  << std::endl;
            exit(-1);
        }

        appName = name;
        //一直向后截取到最后一个字符串
        size_t pos = appName.find("-prd");
        appName = appName.substr(0, pos);
        if (appName.empty()) {
            std::cerr << "appName empty!" << std::endl;
            exit(-1);
        }
    }



    outPath = (outPath + appName);
    std::cout << outPath << std::endl;
    res = mkdir(outPath.c_str(), 0766);
    if (res == -1) {
        if (errno != EEXIST) {
            std::cerr << "mkdir(" << outPath << ")error:" << strerror(errno) << "!" << std::endl;
            exit(-1);
        }
    }
    outPath = (outPath + "/" + "app_console.log");
    std::cout << outPath << std::endl;
    /**
     * php_error log 的监控配置注册
     */
    //检查监控的php错误日志的位置
    std::string php_errors_path = configMap["sentry_log_file"]["php_errors"];
    if (php_errors_path.empty()) {
        std::cerr << "path not empty" << std::endl;
        exit(-1);
    }
    //正则表达式的匹配规则
    std::string php_errors_regex = configMap["sentry_log_regex"]["php_errors-regex"];
    if (php_errors_regex.empty()) {
        std::cerr << "php_errors_regex not empty" << std::endl;
        exit(-1);
    }

    //存储要监控的文件路径
    pathStorage.push_back(php_errors_path);
    //将处理函数加入回调的map中
    std::shared_ptr<App::PHPError> phpErrorHandle = std::make_shared<App::PHPError>();
    phpErrorHandle->setPHPLogType(App::PHP_ERROR);
    phpErrorHandle->setOutPath(outPath);
    phpErrorHandle->setRegEx(php_errors_regex);
    std::map<std::string, std::function<void(const std::string&)>> phpErrorMap;
    phpErrorMap[OnReceive] = std::bind(&App::PHPError::onReceive, phpErrorHandle, _1);
    phpErrorMap[OnClose] = std::bind(&App::PHPError::onClose, phpErrorHandle, _1);
    //检查
    //在容器尾部添加一个元素，这个元素原地构造，不需要触发拷贝构造和转移构造
    handle.emplace_back(phpErrorMap);

    /**
     * 注册php-fpm错误日志的监控配置
     */
    //检查php-fpm的日志
    std::string php_fpm_path = configMap["sentry_log_file"]["php-fpm"];
    if (php_fpm_path.empty()) {
        std::cerr << "php_fpm_path not empty" << std::endl;
        exit(-1);
    }
    //正则表达式的匹配规则
    std::string php_fpm_regex = configMap["sentry_log_regex"]["php-fpm-regex"];
    if (php_fpm_regex.empty()) {
        std::cerr << "php_fpm_regex not empty" << std::endl;
        exit(-1);
    }
    //存储要监控的文件路径
    pathStorage.push_back(php_fpm_path);
    //phpFpmError的map
    std::shared_ptr<App::PHPError> phpFpmErrorHandle = std::make_shared<App::PHPError>();
    phpFpmErrorHandle->setRegEx(php_fpm_regex);
    phpFpmErrorHandle->setPHPLogType(App::PHP_FPM_ERROR);
    phpFpmErrorHandle->setOutPath(outPath);
    std::map<std::string, std::function<void(const std::string&)>> PHPFpmErrorMap;
    PHPFpmErrorMap[OnReceive] = std::bind(&App::PHPError::onReceive, phpFpmErrorHandle, _1);
    PHPFpmErrorMap[OnClose] = std::bind(&App::PHPError::onClose, phpFpmErrorHandle, _1);
    //在容器尾部添加一个元素，这个元素原地构造，不需要触发拷贝构造和转移构造
    handle.emplace_back(PHPFpmErrorMap);

    //检查超时时间
    std::string intervalConfig = configMap["sentry_log_config"]["max_flush_time"];
    if (intervalConfig.empty()) {
        std::cerr << "sentry_log_config[max_flush_time]:" << intervalConfig << " error!" << std::endl;
        exit(-1);
    }
    int interval = atoi(intervalConfig.c_str());
    if (interval <= 0) {
        std::cerr << "sentry_log_config[max_flush_time]:" << intervalConfig << " error!" << std::endl;
        exit(-1);
    }

    //检测定时器最大回收次数
    std::string maxGcNumberConfig = configMap["sentry_log_config"]["max_gc_number"];
    if (maxGcNumberConfig.empty()) {
        std::cerr << "sentry_log_config[max_gc_number]:" << maxGcNumberConfig << " error!" << std::endl;
        exit(-1);
    }

    int maxGcNumber = atoi(configMap["sentry_log_config"]["max_gc_number"].c_str());

    if (maxGcNumber <= 0) {
        std::cerr << "sentry_log_config[max_gc_number] can't letter 0 error!" << std::endl;
        exit(-1);
    }


    /**
     * 命令行分析
     */
    if (executorCmd == "stop") {
        //获取程序的pid
        pid_t pid = getStorageMutexPid(pidFile);
        if (pid > 0) {
            int res = kill(pid, SIGTERM);
            if (res == -1) {
                std::cerr << strerror(errno) << std::endl;
                exit(-1);
            }
            exit(0);
        } else {
            exit(0);
        }
    } else if (executorCmd == "help") {

    }

    //检查是否要启动守护进程
    std::string daemonizeConfig = configMap["sentry_log_config"]["daemonize"];
    if (daemonizeConfig.empty()) {
        std::cerr << "daemonize error!" << std::endl;
    }

    int daemonize = atoi(daemonizeConfig.c_str());
    if (daemonize != 0) {
        createDaemon();
    }

    //启动唯一进程副本
    setStorageMutexPid(pidFile);

    /**
     * 启动服务应用
     */
    //启动线程池
    int num;
    //将php-fpm php_errors.log 和 php-fpm-slow.log 分别 放到一个线程中做处理
    std::shared_ptr<Event::Channel> fileWatcherChannel;
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
        //设置读取最大的buffer
        fileNotifyEvent->setMaxBufferSize(maxBufferSize);
        //设置文件发生变化的时候的通知事件
        watcher->setFileEvent(fileNotifyEvent);
        fileNotifyEvent->setOnReceiveApi(handle[num][OnReceive]);
        fileNotifyEvent->setOnCloseApi(handle[num][OnClose]);
        fileNotifyEvent->setGcFilePath(outPath);
        fileNotifyEvent->setMaxGcNumber(maxGcNumber);
        fileWatcherChannel = std::make_shared<Event::Channel>(threadPool[num]->getEventLoop(), watcher->getINotifyId());
        fileWatcherChannel->setOnReadCallable(std::bind(&OS::UnixInodeWatcher::watcherOnRead, watcher));
        fileWatcherChannel->enableReading();

        fileChannel[num] = fileWatcherChannel;

        //设置定时器
        fileNotifyEvent->setTimer(threadPool[num]->getEventLoop(), interval);

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


    //加入
    mainLoop->start();
}

/**
 * 析构释放，删除主循环内存
 */
Node::NodeManager::~NodeManager() {
    delete mainLoop;
}