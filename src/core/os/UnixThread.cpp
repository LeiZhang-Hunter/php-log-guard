//
// Created by zhanglei on 2020/11/12.
//

#include "os/UnixThread.h"

/**
 * 析构函数如果对象不是守护线程需要用join进行释放
 */
OS::UnixThread::~UnixThread() {
    if (daemonize) {
        return;
    }
    void* status;
    if(pthread_join(mThreadID,&status)<0)
    {
        std::cerr << "CThread->ReleaseThread Join Error" << std::endl;
    }

    delete loop;
}

/**
 * 线程运行的主程序
 * @param arg
 * @return
 */
void* OS::UnixThread::ThreadProc(void *arg) {

    //创建独立的线程运行空间
    OS::UnixThreadProc *proc = static_cast<OS::UnixThreadProc *>(arg);
    //运行线程
    proc->runThread();

    return (void *)0;
}

//启动线程
bool OS::UnixThread::Start() {
    /*
     * 初始化一个锁属性
     */
    pthread_attr_init(&attr);

    bool mRunStatus = false;

    //创建独立的线程运行空间
    UnixThreadProc* proc = new UnixThreadProc(loop, latch);
    /*
     *启动线程
     */
    if(pthread_create(&mThreadID,&attr,ThreadProc,proc) == 0)
    {
        mRunStatus=true;
    } else {
        delete proc;
        return mRunStatus;
    }

    latch->wait();
    tid = proc->getThreadTid();
    /*
     *释放属性
     */
    pthread_attr_destroy(&attr);
    return mRunStatus;
}
