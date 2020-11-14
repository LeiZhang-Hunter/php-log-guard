//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_UNIXAUTOMUTEX_H
#define PHPLOGGUARD_UNIXAUTOMUTEX_H
#include "UnixMutex.h"
namespace OS {
    class UnixAutoMutex {
    public:
        UnixAutoMutex(UnixMutex &mutex_) :mutexVar(mutex_){
            mutexVar.lock();
        }

        ~UnixAutoMutex() {
            mutexVar.unlock();
        }

    private:
        UnixMutex& mutexVar;
    };
}
#endif //PHPLOGGUARD_UNIXAUTOMUTEX_H
