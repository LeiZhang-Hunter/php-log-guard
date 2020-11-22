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

#include "Noncopyable.h"

namespace OS {
    class UnixSignalDescription : public Noncopyable{
    public:
        UnixSignalDescription() {
            sigemptyset(&mask);
        }

        //添加信号到mask信号集
        bool addMask(int sigNo) {
            int res = sigaddset(&mask, sigNo);
            if (res == 0) {
                return true;
            }
            return false;
        }

        //删除信号从信号集中
        bool removeMask(int sigNo) {
            int res = sigdelset(&mask, sigNo);
            if (res == 0)
                return true;
            return false;
        }

        /**
         * 建立信号临界区
         * @return
         */
        bool lockProcMask() {
            if (sigprocmask(SIG_BLOCK, &mask, &oldMask) == 0) {
                return true;
            }
            return false;
        }

        /**
         * 取消信号临界区
         * @return
         */
        bool unLockProcMask() {
            if (sigprocmask(SIG_UNBLOCK, &mask, &oldMask) == 0) {
                return true;
            }
            return false;
        }

        /**
         * 获取signalfd
         * @return
         */
        int getSignalFd() {
            if (!signalFd) {
                signalFd = signalfd(-1, &mask, SFD_NONBLOCK);
                if (signalFd == -1) {
                    return false;
                }
            }
            return signalFd;
        }

        /**
         * 获取最后的错误码
         * @return
         */
        int getErrorno() {
            return errno;
        }

        /**
         * 获取错误详细信息
         * @return
         */
        std::string getErrorMsg() {
            return strerror(errno);
        }

        /**
         * 析构函数关闭signalFd
         */
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
