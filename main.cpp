//
// Created by zhanglei on 19-8-9.
//
//#include "include/CThread.h"
#include "getopt.h"
#include <iostream>
#include "NodeManager.h"
#define DEFAULT_PATH "/etc/php-log-guard.ini"

std::string test(int argc, char **argv, const char &cmd) {
    return "a";
}

std::string test2(int argc, char **argv, const char &cmd) {
    return "b";
}
using namespace std::placeholders;

int main(int argc, char **argv) {
    std::shared_ptr<Node::NodeManager> manager(new Node::NodeManager);
    manager->setCommandArgc(argc);
    manager->setCommandArgv(argv);
    manager->run();
    manager.reset();
}
