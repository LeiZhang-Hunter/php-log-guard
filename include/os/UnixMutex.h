//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_UNIXMUTEX_H
#define PHPLOGGUARD_UNIXMUTEX_H
#include <pthread.h>
#include "Noncopyable.h"

namespace OS {
    class __attribute__((capability("mutex"))) UnixMutex : Noncopyable{
    public:
        UnixMutex() {
            pthread_mutex_init(&mutex, &mutexAttr);
        }

        void lock() {
            pthread_mutex_lock(&mutex);
        }

        void unlock() {
            pthread_mutex_unlock(&mutex);
        }

        ~UnixMutex() {
            pthread_mutex_destroy(&mutex);
        }

    private:
        pthread_mutex_t mutex;
        pthread_mutexattr_t mutexAttr;
    };
}

#endif //PHPLOGGUARD_UNIXMUTEX_H
