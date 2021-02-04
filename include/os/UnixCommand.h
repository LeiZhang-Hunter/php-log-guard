//
// Created by zhanglei on 2020/11/10.
//

#ifndef PHPLOGGUARD_UNIXCOMMAND_H
#define PHPLOGGUARD_UNIXCOMMAND_H

#include <getopt.h>

#include <map>
#include <functional>
#include <iostream>

namespace OS {
    class UnixCommand {
        typedef std::function<bool(int argc, char **argv, const char &)> cmdClosureType;
        typedef std::map<char, cmdClosureType> cmdMapType;
        typedef std::map<const char, const std::string &> cmdResultMapType;
    public:
        void setCmdArgC(int argc) {
            cmdArgC = argc;
        }

        void setCmdArgV(char **argv) {
            cmdArgV = argv;
        }

        /**
         * 将注册的命令加入到Map中
         * @param cmd
         */
        void reg(const char &cmd, const cmdClosureType &closure) {
            cmdMap[cmd] = closure;
        }

        char **getCmdArgv() {
            return cmdArgV;
        }

        int getCmdArgC() {
            return cmdArgC;
        }

        bool parse();

    private:
        cmdMapType cmdMap;
        cmdResultMapType cmdResultMap;
        int cmdArgC = 0;
        char **cmdArgV = nullptr;
    };
}


#endif //PHPLOGGUARD_UNIXCOMMAND_H
