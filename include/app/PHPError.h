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
#include "os/UnixUtil.h"
namespace App {
    class PHPError : Noncopyable ,std::enable_shared_from_this<PHPError> {
    public:
        PHPError() : util(new OS::UnixUtil()){

        }
        /**
         * 收到函数的时候触发的函数地址
         */
        void onReceive(const std::string &);

        /**
         * 设置正则表达式规则
         * @param regexRule
         */
        void setRegEx(const std::string& regexRule) {
            rule = regexRule;
        }

        /**
         * 文件被关闭的时候触发的函数
         */
        void onClose(const std::string &message);

        //缓冲区
        std::string errorBuffer;

        //正则表达式规则
        std::string rule;

        //通用工具
        std::unique_ptr<OS::UnixUtil> util;
    };
}
#endif //PHPLOGGUARD_PHPERROR_H
