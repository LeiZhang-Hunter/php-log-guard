//
// Created by zhanglei on 2020/11/10.
//

#ifndef PHPLOGGUARD_INICONFIG_H
#define PHPLOGGUARD_INICONFIG_H

#include <fcntl.h>
#include <unistd.h>

#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include "Noncopyable.h"

#define MAXLINE 1024 * 8

namespace Config {
    typedef std::map<std::string,std::map <std::string, std::string>> configMapType;

    class IniConfig : public Noncopyable{
    public:
        struct unit{
            std::string key;
            std::string value;
        };

        configMapType readConfig(std::string &filename);

    private:
        ssize_t readLine(int fd,char* buf,size_t manxLine);
    };
}

#endif //PHPLOGGUARD_INICONFIG_H
