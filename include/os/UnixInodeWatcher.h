//
// Created by zhanglei on 2020/11/13.
//

#ifndef PHPLOGGUARD_UNIXINODEWATCHER_H
#define PHPLOGGUARD_UNIXINODEWATCHER_H

#include <sys/inotify.h>
#include <unistd.h>

#include <string>
#include <cstring>
#include <iostream>
#include <memory>
#include "event/FileEvent.h"
#include "Noncopyable.h"

namespace OS {
    class UnixInodeWatcher : public Noncopyable {
    public:
        UnixInodeWatcher() {
            iNotifyId = inotify_init();
        }

        bool setFileEvent(const std::shared_ptr<App::FileEvent>& event) {
            fileEvent = event;
            return true;
        }

        bool setWatcher(const std::string& path);

        /**
         * 获取最后的错误码
         * @return
         */
        int getErrorno() {
            return errno;
        }

        void watcherOnRead();

        /**
         * 获取错误详细信息
         * @return
         */
        std::string getErrorMsg() {
            return strerror(errno);
        }

        int getWatcherFd() {
            return watcherFd;
        }

        int getINotifyId() {
            return iNotifyId;
        }

    private:
        int iNotifyId;
        int watcherFd;
        int count = 0;
        std::shared_ptr<App::FileEvent> fileEvent;
    };
}

#endif //PHPLOGGUARD_UNIXINODEWATCHER_H
