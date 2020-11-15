//
// Created by zhanglei on 2020/11/14.
//

#ifndef PHPLOGGUARD_PHPERROR_H
#define PHPLOGGUARD_PHPERROR_H
#include <string>
#include <regex>
#include <memory>
#include "Noncopyable.h"
namespace App {
    class PHPError : Noncopyable ,std::enable_shared_from_this<PHPError> {
    public:
        void onReceive(const std::string &);
    };
}
#endif //PHPLOGGUARD_PHPERROR_H
