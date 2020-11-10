//
// Created by zhanglei on 2020/11/10.
//

#include "NodeManager.h"

/**
 * 运行主程序
 */
void Node::NodeManager::run() {
    command->reg('c', std::bind(&NodeManager::getConfigPath, shared_from_this(), _1, _2, _3));
    command->parse();
}