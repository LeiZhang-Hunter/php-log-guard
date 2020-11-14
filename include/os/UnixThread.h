//
// Created by zhanglei on 2020/11/12.
//

#ifndef PHPLOGGUARD_UNIXTHREAD_H
#define PHPLOGGUARD_UNIXTHREAD_H

//c
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

//c++
#include <cstdint>
#include <iostream>

#include "UnixMutex.h"

namespace OS {
    class UnixThread {
    public:
        //构造函数
        UnixThread() {
            //初始化互斥锁
            mThreadID = syscall(SYS_gettid);
            mTerminated = false;
            daemonize = false;
            isSuspend = false;
            mRunStatus = false;
        }

        //析构函数
        ~UnixThread();

        //启动线程
        bool Start();

        //停止线程
        void Stop() {
            mMutex.lock();
            mTerminated = true;
            mMutex.unlock();
        }

        //恢复线程
        void Resume() {
            mMutex.lock();
            isSuspend = false;
            mMutex.unlock();
        }

        //线程的运行状态
        bool Status() {
            return mRunStatus;
        }

        //设置守护线程
        bool SetDaemonize() {
            daemonize = true;
        }

    private:
        /**
         * 线程运行的主程序
         * @param arg
         * @return
         */
        static void* ThreadProc(void* arg);

        /**
         * 互斥锁
         */
        UnixMutex mMutex;

        /**
         * 条件锁
         */
        pthread_cond_t mCondLock __attribute__((guarded_by(mMutex)));

        /**
         * 线程id
         */
        pthread_t mThreadID __attribute__((guarded_by(mMutex)));

        /**
         * 是否是守护线程
         */
        uint8_t daemonize __attribute__((guarded_by(mMutex)));


        /**
         * 是否要终止线程运行
         */
        bool mTerminated __attribute__((guarded_by(mMutex)));

        /**
         * 是否要将线程挂起
         */
        bool isSuspend __attribute__((guarded_by(mMutex)));

        //线程运行状态
        bool mRunStatus __attribute__((guarded_by(mMutex)));


    };
}

#endif //PHPLOGGUARD_UNIXTHREAD_H
