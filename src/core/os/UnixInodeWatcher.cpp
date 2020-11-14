//
// Created by zhanglei on 2020/11/13.
//

#include "os/UnixInodeWatcher.h"

bool OS::UnixInodeWatcher::setWatcher(const std::string& path) {
    watcherFd = inotify_add_watch(iNotifyId, path.c_str(), IN_ATTRIB |IN_MODIFY | IN_ACCESS |IN_CREATE |IN_DELETE |IN_MOVE_SELF);
    if (watcherFd == -1) {
        return false;
    }
    return true;
}

void OS::UnixInodeWatcher::watcherOnRead() {
    char buf[BUFSIZ];
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