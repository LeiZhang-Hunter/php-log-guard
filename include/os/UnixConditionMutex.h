//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_UNIXCONDITIONMUTEX_H
#define PHPLOGGUARD_UNIXCONDITIONMUTEX_H

#include <pthread.h>
#include <iostream>
#include "UnixMutex.h"

namespace OS {
    class UnixConditionMutex :Noncopyable {
    public:
        /**
         * 初始化条件变量互斥器
         * @param mutex1
         */
        UnixConditionMutex(UnixMutex &mutex1) :mutex(mutex1) {
            pthread_cond_init(&condition, nullptr);
        }

        /**
         * 等待
         */
        void wait() {
            pthread_cond_wait(&condition, (mutex.getMutex()));
        }

        /**
         * 通知
         */
        void notify() {
            pthread_cond_signal(&condition);
        }

        /**
         * 广播
         */
        void broadCast() {
            int res = pthread_cond_broadcast(&condition);
            if (res == -1) {
            }
        }

        /**
         * 释放
         */
        ~UnixConditionMutex() {
            pthread_cond_destroy(&condition);
        }
    private:
        pthread_cond_t condition;
        UnixMutex& mutex;
    };
}

#endif //PHPLOGGUARD_UNIXCONDITIONMUTEX_H
