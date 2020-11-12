//
// Created by zhanglei on 2020/11/12.
//

#ifndef PHPLOGGUARD_UNIXSIGNALDESCRIPTION_H
#define PHPLOGGUARD_UNIXSIGNALDESCRIPTION_H

#include <csignal>
#include <cerrno>
#include <string>
#include <cstring>
#include <sys/signalfd.h>

namespace OS {
    class UnixSignalDescription {
    public:
        UnixSignalDescription() {

        }

        //添加信号到mask信号集
        bool addMask(int sigNo) {
            int res = sigaddset(&mask, sigNo);
            if (res == 0)
                return true;
            return false;
        }

        //删除信号从信号集中
        bool removeMask(int sigNo) {
            int res = sigdelset(&mask, sigNo);
            if (res == 0)
                return true;
            return false;
        }

        bool lockProcMask() {
            if (sigprocmask(SIG_BLOCK, &mask, &oldMask) == 0) {
                return true;
            }
            return false;
        }

        bool unLockProcMask() {
            if (sigprocmask(SIG_UNBLOCK, &mask, &oldMask) == 0) {
                return true;
            }
            return false;
        }

        int getSignalFd() {
            lockProcMask();
            signalFd = signalfd(-1, &mask, SFD_NONBLOCK);
            if (signalFd == -1) {
                return false;
            }
            return signalFd;
        }

        int getErrorno() {
            return errno;
        }

        std::string getErrorMsg() {
            return strerror(errno);
        }

        ~UnixSignalDescription() {
            if(signalFd > 0) {
                close(signalFd);
            }
        }
    private:
        int signalFd = 0;
        sigset_t mask;
        sigset_t oldMask;
    };
}
#endif //PHPLOGGUARD_UNIXSIGNALDESCRIPTION_H
