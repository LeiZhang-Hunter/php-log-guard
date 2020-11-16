//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_PHPFPMERROR_H
#define PHPLOGGUARD_PHPFPMERROR_H
#include <string>
#include <regex>
#include <memory>
#include "Noncopyable.h"
namespace App {
    class PHPFpmError : Noncopyable ,std::enable_shared_from_this<PHPFpmError> {
    public:
        /**
         * 收到函数的时候触发的函数地址
         */
        void onReceive(const std::string &);

        /**
         * 文件被关闭的时候触发的函数
         */
        void onClose(const std::string &message);
    };
}
#endif //PHPLOGGUARD_PHPFPMERROR_H
