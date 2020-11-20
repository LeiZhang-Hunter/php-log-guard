//
// Created by zhanglei on 2020/11/14.
//
#include "event/FileEvent.h"
#include "os/UnixInodeWatcher.h"

App::FileEvent::FileEvent(const std::string& path, std::shared_ptr<OS::UnixInodeWatcher>& _watcher)  :
watcher(_watcher) {
    filePath = path;

    if (!openFile()) {

        exit(-1);
    }

    //刷新存储文件的偏移量
    flushOffset();
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
    int flags = fcntl(monitorFileFd,F_GETFL,0);
    fcntl(monitorFileFd, F_SETFL, flags | O_NONBLOCK);
    return monitorFileFd;
}


bool App::FileEvent::flushOffset() {
    struct stat file_stat;
    int res = fstat(monitorFileFd, &file_stat);
    if (res == -1) {
        std::cerr << ";error:" << strerror(errno) << std::endl;
        offset = 0;
        return false;
    } else {
        offset = file_stat.st_size;
        return true;
    }
}

void App::FileEvent::onModify() {
    struct stat file_state;//文件状态的结构体
    ssize_t readSize;//当前读取的尺寸
    ssize_t readBytes;//当前读取的尺寸
    ssize_t currentPosition;//要读取的偏移量
    ssize_t readSum; //要读取的总量
    size_t oldPosition = offset;//旧的位置

    //刷新位置
    flushOffset();

    if (offset == 0) {
        return;
    }

    //要开始读取的位置
    currentPosition = offset;

    //如果说开始读取的位置比之前的,说明被人损坏过,忽略掉这次读取
    if (currentPosition <= oldPosition) {
        return;
    }

    readSum = currentPosition - oldPosition;

    //读取的缓冲区
    char readBuffer[BUFSIZ];

    //如果缓冲区要读取的长度超过了BUFSIZE，那么每次读取BUFSIZE到缓冲区，循环读取，一直到读完
    if (readSum >= BUFSIZ) {
        readBytes = BUFSIZ;
    } else {
        readBytes = readSum;
    }

    //必须要读完,读不完不能停止
    while (1) {
        if (readBytes == 0 || readSum == 0) {
            break;
        }
        readSize = pread(monitorFileFd, readBuffer, readBytes, oldPosition);
        if (readSize <= 0) {
            //读完了
            if (errno == EAGAIN) {
                break;
            }

            if (errno ==  EINTR) {
                continue;
            }

            break;
        } else {
            oldPosition += readSize;
            readBuffer[readSize] = '\0';
            modifyFunc(readBuffer);

            readSum -= readSize;

            if (readSum >= BUFSIZ) {
                readBytes = BUFSIZ;
            } else {
                readBytes = readSum;
            }

            if (readSum == 0) {
                break;
            }
        }
    }

}

void App::FileEvent::onCreate() {
    std::cout << "onCreate" << std::endl;
}

void App::FileEvent::onDelete() {
    std::cout << "onDelete" << std::endl;
}

void App::FileEvent::onAttrChange() {
    std::cout << "onAttr" << std::endl;
    closeFile();
    openFile();
    closeFunc("close");
    watcher->reloadWatcher();
    offset = 0;
}

void App::FileEvent::onMove() {
    std::cout << "onMove" << std::endl;
}

void App::FileEvent::onAccess() {
    std::cout << "onAccess" << std::endl;
}