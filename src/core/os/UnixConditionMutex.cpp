//
// Created by zhanglei on 2020/11/14.
//

#include "os/UnixCountDownLatch.h"

void OS::UnixCountDownLatch::down() {
    mutex.lock();
    count--;
    if (count == 0) {
        condition.broadCast();
    }
}

void OS::UnixCountDownLatch::wait() {
    condition.wait();
}