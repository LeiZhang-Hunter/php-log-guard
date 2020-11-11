#include "lock/FilePersistenceLock.h"

bool Lock::FilePersistenceLock::lock(int fileDescription, short int type) {
    fileLock.l_type = type;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    int res = fcntl(fileDescription, F_SETLK, &fileLock);
    if (res == 0)
        return true;
    return false;
}

bool Lock::FilePersistenceLock::unlock(int fileDescription, short int type) {
    return true;
}