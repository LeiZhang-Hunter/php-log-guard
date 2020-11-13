//
// Created by zhanglei on 2020/11/13.
//

#include "UnixInodeWatcher.h"

bool OS::UnixInodeWatcher::setWatcher(const std::string& path) {
    watcherFd = inotify_add_watch(iNotifyId, path.c_str(), IN_MODIFY);
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
            count++;
        }
    }

    std::cout << count << std::endl;

}