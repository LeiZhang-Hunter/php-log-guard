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
#include "event/EventLoop.h"
#include "os/UnixThreadProc.h"
#include "os/UnixCountDownLatch.h"
#include "event/Channel.h"

namespace OS {
    class UnixThread {
    public:
        //构造函数
        UnixThread() {
            //初始化互斥锁
            mTerminated = false;
            daemonize = false;
            isSuspend = false;
            mRunStatus = false;
            loop = new Event::EventLoop();
            latch = std::make_shared<UnixCountDownLatch>(1);
        }

        //析构函数
        ~UnixThread();

        //启动线程
        bool Start();

        //停止线程
        void Stop() {
            mMutex.lock();
            loop->stop();
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
            return true;
        }

        //获取线程的tid
        size_t  getPid() {
            return mThreadID;
        }

        //获取线程内部的事件循环
        Event::EventLoop* getEventLoop() {
            return loop;
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
        pthread_cond_t mCondLock;

        /**
         * 线程id
         */
        pthread_t mThreadID;

        /**
         * 是否是守护线程
         */
        uint8_t daemonize;


        /**
         * 是否要终止线程运行
         */
        bool mTerminated;

        /**
         * 是否要将线程挂起
         */
        bool isSuspend;

        //线程运行状态
        bool mRunStatus;

        //线程属性
        pthread_attr_t attr;

        //事件循环
        Event::EventLoop* loop;

        //互斥原语
        std::shared_ptr<UnixCountDownLatch> latch;

        //线程id
        pid_t tid;
    };
}

#endif //PHPLOGGUARD_UNIXTHREAD_H
