//
// Created by zhanglei on 2020/11/11.
//

#ifndef PHPLOGGUARD_FILEPERSISTENCELOCK_H
#define PHPLOGGUARD_FILEPERSISTENCELOCK_H

#include <fcntl.h>

#include <cstring>
#include <string>

#include "Noncopyable.h"

namespace Lock {
    /**
     * 这个类是禁止复制的，因为这个类含有描述符，为了避免被复制出现副本close 关闭不掉
     */
    class FilePersistenceLock{
    public:
        FilePersistenceLock() {};
        /**
         * 锁住文件
         * @param fileDescription
         * @param type
         * @return
         */
        bool lock(int fileDescription, short int type);

        /**
         * 解锁整个文件
         * @param fileDescription
         * @param type
         * @return
         */
        bool unlock(int fileDescription, short int type);

        int getErrno() {
            return errno;
        }

        std::string getErrorMsg() {
            return strerror(errno);
        }

    private:
        struct flock fileLock;
    };
}

#endif //PHPLOGGUARD_FILEPERSISTENCELOCK_H
