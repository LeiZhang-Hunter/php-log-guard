//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_UNIXCOUNTDOWNLATCH_H
#define PHPLOGGUARD_UNIXCOUNTDOWNLATCH_H

#include "UnixMutex.h"
#include "UnixConditionMutex.h"
#include "UnixAutoMutex.h"
namespace OS {
    class UnixCountDownLatch {

    public:
        /**
         * 初始化
         * @param count
         */
        UnixCountDownLatch(int count) :count(count), mutex(), condition(mutex){}

        /**
         * 等待完成
         */
        void wait();

        /**
         * 完成
         */
        void down();

        ~UnixCountDownLatch();

    private:
        int count;
        UnixMutex mutex;
        UnixConditionMutex condition;
    };
}



#endif //PHPLOGGUARD_UNIXCOUNTDOWNLATCH_H
