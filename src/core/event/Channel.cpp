//
// Created by zhanglei on 2020/11/11.
//

#include "Channel.h"
#include "EventLoop.h"

void Event::Channel::update() {
    loop->updateChannel(this);
}