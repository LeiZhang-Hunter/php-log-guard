//
// Created by zhanglei on 2020/11/14.
//

#include "os/UnixThreadProc.h"

void OS::UnixThreadProc::onStop() {
    loop->stop();
}

void OS::UnixThreadProc::runThread() {
    //初始化线程id
    threadTid = syscall(SYS_gettid);
    signalDescription->addMask(SIGTERM);
    signalDescription->lockProcMask();
    //绑定信号处理器
    std::shared_ptr<Event::Channel> signalChannel = std::make_shared<Event::Channel>(loop,
            signalDescription->getSignalFd());
    signalChannel->setOnReadCallable(std::bind(&UnixThreadProc::onStop, this));
    signalChannel->enableReading();
    //完成初始化继续运行
    latch->down();


    //进行事件循环
    loop->start();
}