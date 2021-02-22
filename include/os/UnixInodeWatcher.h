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
#include "os/UnixUtil.h"

namespace OS {
    class UnixInodeWatcher : public Noncopyable {
    public:
        /**
         * 初始化inode观察者
         * @param _path
         */
        UnixInodeWatcher(const std::string& _path) {
            iNotifyId = inotify_init();
            watcherPath = _path;
        }

        /**
         * 设置inode观察者触发器
         * @param event
         * @return
         */
        bool setFileEvent(const std::shared_ptr<App::FileEvent>& event) {
            fileEvent = event;
            return true;
        }

        /**
         * 打开inode观察模式
         * @return
         */
        bool enableWatcher();

        /**
         * 关闭inode观察节点
         * @return
         */
        bool disableWatcher();

        /**
         * 重载inode
         */
        void reloadWatcher();

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
        std::string watcherPath;
    };
}

#endif //PHPLOGGUARD_UNIXINODEWATCHER_H
