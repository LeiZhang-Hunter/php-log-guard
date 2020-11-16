//
// Created by zhanglei on 2020/11/14.
//

#include "os/UnixCountDownLatch.h"

void OS::UnixCountDownLatch::down() {
    UnixAutoMutex guard(mutex);
    count--;
    if (count == 0) {
        condition.broadCast();
    }
}

void OS::UnixCountDownLatch::wait() {
    UnixAutoMutex guard(mutex);
    while (count > 0) {
        condition.wait();
    }
}

OS::UnixCountDownLatch::~UnixCountDownLatch() {

}