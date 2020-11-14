//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_UNIXAUTOMUTEX_H
#define PHPLOGGUARD_UNIXAUTOMUTEX_H
#include "UnixMutex.h"
namespace OS {
    class UnixAutoMutex {
    public:
        UnixAutoMutex(UnixMutex &mutex_) :mutex(mutex_){
            mutex.lock();
        }

        ~UnixAutoMutex() {
            mutex.unlock();
        }

    private:
        UnixMutex& mutex;
    };
}
#endif //PHPLOGGUARD_UNIXAUTOMUTEX_H
