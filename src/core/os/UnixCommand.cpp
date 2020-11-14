//
// Created by zhanglei on 2020/11/10.
//

#include "os/UnixCommand.h"


/*==================解析命令行参数======================*/
bool OS::UnixCommand::parse() {
    //遍历map组装结果
    cmdMapType::iterator cmdMapIterator;
    std::string cmdOptionString;
    std::string option;

    //检查argc
    if (cmdArgC == 0) {
        std::cerr << "Please set argc" << std::endl;
        exit(-1);
    }

    if (!cmdArgV) {
        std::cerr << "Please set argv" << std::endl;
        exit(-1);
    }

    for (cmdMapIterator = cmdMap.begin(); cmdMapIterator != cmdMap.end(); cmdMapIterator++) {
        cmdOptionString.append(&cmdMapIterator->first);
    }

    int opt = 0;
    char charOpt;
    while ((opt = getopt(cmdArgC, cmdArgV, cmdOptionString.c_str())) != -1) {
        charOpt = static_cast<char>(opt);
        cmdMapIterator = cmdMap.find(charOpt);
        if (cmdMapIterator != cmdMap.end()) {
            cmdMapIterator->second(cmdArgC, cmdArgV, charOpt);
        } else {
            std::cerr << "args" << charOpt - '0' << " not found" << std::endl;
            exit(-1);
        }
    }
    return true;
}