//
// Created by zhanglei on 2020/11/14.
//

#include "os/UnixThreadProc.h"

void OS::UnixThreadProc::runThread() {
    //初始化线程id
    threadTid = syscall(SYS_gettid);

    //完成初始化继续运行
    latch->down();

    //进行事件循环
    loop->start();
}