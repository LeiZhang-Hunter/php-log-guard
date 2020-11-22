//
// Created by zhanglei on 2020/11/11.
//

#ifndef PHPLOGGUARD_EVENTLOOP_H
#define PHPLOGGUARD_EVENTLOOP_H
#include <sys/epoll.h>
#include <unistd.h>
#include <syscall.h>

#include <iostream>
#include <cstring>
#include <vector>
#include <map>

#include "Noncopyable.h"
#include "Channel.h"

namespace Event {
    typedef std::vector<struct epoll_event> EventListType;
    typedef std::map<int, Channel*> ChannelMap;
    class EventLoop : public Noncopyable {
    public:
        EventLoop();

        bool set(int operation, Channel* channel);

        bool stop() {
            return quit = true;
        }

        /**
         * 被环形
         */
        void onWakeUp();

        bool start();

        bool updateChannel(Channel* channel);

        bool wakeup();

        ~EventLoop() {
            ::close(eventPollFd);
        }

    private:
        int eventPollFd;
        bool quit = false;
        EventListType eventList;
        ChannelMap channels;
        //创建用来wakeup的channel
        std::shared_ptr<Event::Channel> wakeUpChannel;
    };
}

#endif //PHPLOGGUARD_EVENTLOOP_H
