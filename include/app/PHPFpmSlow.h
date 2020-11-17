//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_PHPFPMSLOW_H
#define PHPLOGGUARD_PHPFPMSLOW_H
#include <string>
#include <regex>
#include <memory>
#include "Noncopyable.h"
namespace App {
    class PHPFpmSlow : Noncopyable ,std::enable_shared_from_this<PHPFpmSlow> {
    public:
        /**
         * 收到函数的时候触发的函数地址
         */
        void onReceive(const std::string &);

        /**
         * 文件被关闭的时候触发的函数
         */
        void onClose(const std::string &message);

        void setRegEx(const std::string& regexRule) {
            rule = regexRule;
        }

    private:
        std::string phpFpmSlowBuffer;
        std::string rule;
    };
}

#endif //PHPLOGGUARD_PHPFPMSLOW_H
