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
        void onReceive(const std::string &);
    };
}

#endif //PHPLOGGUARD_PHPFPMSLOW_H
