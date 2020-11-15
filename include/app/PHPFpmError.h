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
        void onReceive(const std::string &);
    };
}
#endif //PHPLOGGUARD_PHPFPMERROR_H
