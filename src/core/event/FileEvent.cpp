//
// Created by zhanglei on 2020/11/14.
//
#include "event/FileEvent.h"
#include "os/UnixInodeWatcher.h"

App::FileEvent::FileEvent(const std::string& path, std::shared_ptr<OS::UnixInodeWatcher>& _watcher)  :
watcher(_watcher) {
    filePath = path;
    offset = 0;

    if (!openFile()) {

        exit(-1);
    }
}

bool App::FileEvent::closeFile() {
    if (monitorFileFd > 0) {
        close(monitorFileFd);
    }
    return true;
}

bool App::FileEvent::openFile() {
    monitorFileFd = open(filePath.c_str(), O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    if (monitorFileFd == -1) {
        std::cerr << filePath.c_str() << ";error:" << strerror(errno) << std::endl;
        return -1;
    }
    return monitorFileFd;
}


bool App::FileEvent::flushOffset() {
    struct stat file_stat;
    int res = fstat(monitorFileFd, &file_stat);
    if (res == -1) {
        std::cerr << ";error:" << strerror(errno) << std::endl;
        return false;
    } else {
        offset = file_stat.st_size;
        return true;
    }
}

void App::FileEvent::onModify() {
    struct stat file_state;//文件状态的结构体
    int seek = lseek(monitorFileFd, 0, SEEK_END);
    size_t readSize = seek - offset;
    std::vector<char> buffer(readSize);
    pread(monitorFileFd, &*buffer.begin(), readSize, offset);
    std::cout << "data:" << &*buffer.begin() << std::endl;
}

void App::FileEvent::onCreate() {
    std::cout << "onCreate" << std::endl;
}

void App::FileEvent::onDelete() {
    std::cout << "onDelete" << std::endl;
}

void App::FileEvent::onAttrChange() {
    closeFile();
    openFile();
    watcher->reloadWatcher();
    std::cout << "onAttrChange" << std::endl;
}

void App::FileEvent::onMove() {
    std::cout << "onMove" << std::endl;
}

void App::FileEvent::onAccess() {
    //std::cout << "onAccess" << std::endl;
}