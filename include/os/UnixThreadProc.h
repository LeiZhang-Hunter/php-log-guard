//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_UNIXTHREADPROC_H
#define PHPLOGGUARD_UNIXTHREADPROC_H
#include "event/EventLoop.h"
#include "os/UnixCountDownLatch.h"
#include "os/UnixSignalDescription.h"
namespace OS {
    class UnixThreadProc {
    public:
        /**
         * 初始化线程
         * @param loop_
         */
        UnixThreadProc(Event::EventLoop* loop_, std::shared_ptr<UnixCountDownLatch>& latch_) : latch(latch_) {
            loop = loop_;
            signalDescription = std::make_shared<OS::UnixSignalDescription>();
        }

        void onStop();

        //运行线程空间主程序
        void runThread();

        pid_t getThreadTid() {
            return threadTid;
        }

    private:
        //事件循环
        Event::EventLoop* loop;

        std::shared_ptr<UnixCountDownLatch>& latch;
        //线程id
        pid_t threadTid;
        std::shared_ptr<OS::UnixSignalDescription> signalDescription;;
    };
}

#endif //PHPLOGGUARD_UNIXTHREADPROC_H
