//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_UNIXCOUNTDOWNLATCH_H
#define PHPLOGGUARD_UNIXCOUNTDOWNLATCH_H

namespace OS {
    class UnixCountDownLatch {

    public:
        void wait();

        void down();

    private:
    };
}



#endif //PHPLOGGUARD_UNIXCOUNTDOWNLATCH_H
