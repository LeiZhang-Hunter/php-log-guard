//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_PHPERROR_H
#define PHPLOGGUARD_PHPERROR_H
#include <string>
#include <regex>
#include <memory>
#include <iostream>
#include <regex>

#include "Noncopyable.h"
namespace App {
    class PHPError : Noncopyable ,std::enable_shared_from_this<PHPError> {
    public:
        /**
         * 收到函数的时候触发的函数地址
         */
        void onReceive(const std::string &);

        /**
         * 文件被关闭的时候触发的函数
         */
        void onClose(const std::string &message);

        std::string errorBuffer;
    };
}
#endif //PHPLOGGUARD_PHPERROR_H
