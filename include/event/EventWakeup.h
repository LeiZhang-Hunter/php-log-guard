//
// Created by zhanglei on 2020/11/12.
//

#ifndef PHPLOGGUARD_EVENTWAKEUP_H
#define PHPLOGGUARD_EVENTWAKEUP_H
#include <sys/eventfd.h>
namespace Event {
    int createEventFd() {
        return eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    }
}
#endif //PHPLOGGUARD_EVENTWAKEUP_H
