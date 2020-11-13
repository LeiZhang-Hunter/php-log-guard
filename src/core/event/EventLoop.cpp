//
// Created by zhanglei on 2020/11/11.
//

#include "EventLoop.h"

bool Event::EventLoop::set(int operation, Event::Channel* channel) {
    struct epoll_event event;
    event.events = channel->getEvents();
    event.data.ptr = channel;
    int res;
    res = epoll_ctl(eventPollFd, EPOLL_CTL_ADD, channel->getChannelFd(), &event);
    if (res == 0) {
        return true;
    } else {
        return false;
    }
}

bool Event::EventLoop::start() {

    if (eventPollFd == -1) {
        std::cerr << "epoll create error , error msg :" << strerror(errno) << std::endl;
        exit(-1);
    }

    quit = false;
    std::cout << syscall(SYS_gettid) << std::endl;

    //事件循环
    while (!quit) {
        int eventNumber = epoll_wait(eventPollFd, &*eventList.begin(),
                static_cast<int>(eventList.size()),
                -1);


        //循环事件池，定位channel
        for(int i = 0; i < eventNumber; i++) {
            Channel* channel = static_cast<Channel*>(eventList[i].data.ptr);
            channel->setEvents(eventList[i].events);
            channel->handelEvent();
        }
    }

    quit = true;
    return false;
}

bool Event::EventLoop::updateChannel(Channel* channel) {
    ChannelMap::iterator it;

    it = channels.find(channel->getChannelFd());

    if (it != channels.end()) {
        channels[channel->getChannelFd()] = channel;
        set(EPOLL_CTL_MOD, channel);
    } else {
        channels[channel->getChannelFd()] = channel;
        set(EPOLL_CTL_ADD, channel);
    }
    return true;
}