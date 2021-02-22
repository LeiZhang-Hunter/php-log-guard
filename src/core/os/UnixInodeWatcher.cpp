//
// Created by zhanglei on 2020/11/13.
//

#include "os/UnixInodeWatcher.h"

bool OS::UnixInodeWatcher::enableWatcher() {

    //检查文件是否存在，文件不存在的话创建文件
    OS::UnixUtil util;
    //检查文件是否合理
    if (access(watcherPath.c_str(), F_OK) == -1) {
        //创建文件
        int fd = open(watcherPath.c_str(), O_CREAT, 0664);
        if (fd > 0) {
            close(fd);
        }
    }
    watcherFd = inotify_add_watch(iNotifyId, watcherPath.c_str(), IN_ATTRIB |IN_MODIFY  |IN_CREATE |IN_DELETE |IN_MOVE_SELF);
    if (watcherFd == -1) {
        std::cerr << getErrorMsg() << std::endl;
        return false;
    }
    return true;
}

bool OS::UnixInodeWatcher::disableWatcher() {
    inotify_rm_watch(iNotifyId, watcherFd);
    return true;
}

void OS::UnixInodeWatcher::reloadWatcher() {
    disableWatcher();
    enableWatcher();
}

void OS::UnixInodeWatcher::watcherOnRead() {
    char buf[BUFSIZ];
    bzero(buf, BUFSIZ);
    size_t i = 0;
    struct inotify_event* event;
    ssize_t readBufSize = read(iNotifyId, buf, BUFSIZ);

    if(readBufSize>0)
    {
        while (i < readBufSize)
        {
            event = (struct inotify_event*)&buf[i];
            i += (sizeof(struct inotify_event) + event->len);

            if (event->mask & IN_MODIFY) {
                fileEvent->onModify();
            }

            if (event->mask & IN_ACCESS) {
                fileEvent->onAccess();
            }

            if (event->mask & IN_CREATE) {
                fileEvent->onCreate();
            }

            if (event->mask & IN_DELETE || event->mask & IN_DELETE_SELF) {
                fileEvent->onDelete();
            }

            if (event->mask & IN_MOVE_SELF) {
                fileEvent->onMove();
            }

            if (event->mask & IN_ATTRIB) {
                fileEvent->onAttrChange();
            }


            count++;
        }
    }


}