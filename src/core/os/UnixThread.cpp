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
}

/**
 * 线程运行的主程序
 * @param arg
 * @return
 */
void* OS::UnixThread::ThreadProc(void *arg) {

}

//启动线程
bool OS::UnixThread::Start() {
    /*
     * 初始化一个锁属性
     */
    pthread_attr_init(&attr);

    bool mRunStatus = false;
    /*
     *启动线程
     */
    if(pthread_create(&mThreadID,&attr,ThreadProc,this) == 0)
    {
        mRunStatus=true;
    }
    /*
     *释放属性
     */
    pthread_attr_destroy(&attr);
    return mRunStatus;
}
